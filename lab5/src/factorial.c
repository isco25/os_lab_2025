#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <getopt.h>

// Структура для передачи данных в потоки
typedef struct {
    int start;
    int end;
    long long mod;
    long long partial_result;
} thread_data_t;

// Глобальные переменные
long long global_result = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int k = 0;
int pnum = 1;
long long mod = 0;

// Функция для вычисления частичного произведения
void* compute_partial_factorial(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    data->partial_result = 1;
    
    for (int i = data->start; i <= data->end; i++) {
        data->partial_result = (data->partial_result * i) % data->mod;
    }
    
    // Синхронизированное обновление глобального результата
    pthread_mutex_lock(&mutex);
    global_result = (global_result * data->partial_result) % mod;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

// Функция для разбора аргументов командной строки
void parse_arguments(int argc, char* argv[]) {
    static struct option long_options[] = {
        {"k", required_argument, 0, 'k'},
        {"pnum", required_argument, 0, 'p'},
        {"mod", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "k:p:m:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'k':
                k = atoi(optarg);
                if (k < 0) {
                    fprintf(stderr, "Error: k must be non-negative\n");
                    exit(1);
                }
                break;
            case 'p':
                pnum = atoi(optarg);
                if (pnum <= 0) {
                    fprintf(stderr, "Error: pnum must be positive\n");
                    exit(1);
                }
                break;
            case 'm':
                mod = atoll(optarg);
                if (mod <= 0) {
                    fprintf(stderr, "Error: mod must be positive\n");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s -k <number> --pnum=<threads> --mod=<modulus>\n", argv[0]);
                exit(1);
        }
    }
    
    // Проверка обязательных параметров
    if (k == 0 || mod == 0) {
        fprintf(stderr, "Error: k and mod are required parameters\n");
        fprintf(stderr, "Usage: %s -k <number> --pnum=<threads> --mod=<modulus>\n", argv[0]);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    // Разбор аргументов командной строки
    parse_arguments(argc, argv);
    
    printf("Computing %d! mod %lld using %d threads\n", k, mod, pnum);
    
    // Особые случаи
    if (k == 0 || k == 1) {
        printf("Result: 1\n");
        return 0;
    }
    
    // Если количество потоков больше k, уменьшаем pnum
    if (pnum > k) {
        pnum = k;
        printf("Adjusted number of threads to %d (cannot exceed k)\n", pnum);
    }
    
    // Создание потоков и данных для них
    pthread_t threads[pnum];
    thread_data_t thread_data[pnum];
    
    // Распределение работы между потоками
    int numbers_per_thread = k / pnum;
    int remainder = k % pnum;
    int current_start = 1;
    
    for (int i = 0; i < pnum; i++) {
        int numbers_for_this_thread = numbers_per_thread;
        if (i < remainder) {
            numbers_for_this_thread++;
        }
        
        thread_data[i].start = current_start;
        thread_data[i].end = current_start + numbers_for_this_thread - 1;
        thread_data[i].mod = mod;
        
        current_start += numbers_for_this_thread;
    }
    
    // Создание и запуск потоков
    for (int i = 0; i < pnum; i++) {
        if (pthread_create(&threads[i], NULL, compute_partial_factorial, &thread_data[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    
    // Ожидание завершения всех потоков
    for (int i = 0; i < pnum; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }
    
    // Вывод результата
    printf("Result: %lld\n", global_result);
    
    // Уничтожение мьютекса
    pthread_mutex_destroy(&mutex);
    
    return 0;
}