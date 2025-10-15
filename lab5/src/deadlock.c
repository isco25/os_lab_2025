#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1_function(void* arg) {
    printf("Поток 1: пытается захватить мьютекс 1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Поток 1: захватил мьютекс 1\n");
    
    // Имитация работы
    sleep(1);
    
    printf("Поток 1: пытается захватить мьютекс 2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Поток 1: захватил мьютекс 2\n");
    
    // Критическая секция
    printf("Поток 1: работает в критической секции\n");
    sleep(1);
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    
    printf("Поток 1: завершил работу\n");
    return NULL;
}

void* thread2_function(void* arg) {
    printf("Поток 2: пытается захватить мьютекс 2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Поток 2: захватил мьютекс 2\n");
    
    // Имитация работы
    sleep(1);
    
    printf("Поток 2: пытается захватить мьютекс 1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Поток 2: захватил мьютекс 1\n");
    
    // Критическая секция
    printf("Поток 2: работает в критической секции\n");
    sleep(1);
    
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    
    printf("Поток 2: завершил работу\n");
    return NULL;
}

// Версия без deadlock (правильный порядок захвата мьютексов)
void* thread1_safe_function(void* arg) {
    printf("Поток 1 (без deadlock): пытается захватить мьютекс 1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Поток 1 (без deadlock): захватил мьютекс 1\n");
    
    sleep(1);
    
    printf("Поток 1 (без deadlock): пытается захватить мьютекс 2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Поток 1 (без deadlock): захватил мьютекс 2\n");
    
    printf("Поток 1 (без deadlock): работает в критической секции\n");
    sleep(1);
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    
    printf("Поток 1 (без deadlock): завершил работу\n");
    return NULL;
}

void* thread2_safe_function(void* arg) {
    printf("Поток 2 (без deadlock): пытается захватить мьютекс 1...\n");
    pthread_mutex_lock(&mutex1);
    printf("Поток 2 (без deadlock): захватил мьютекс 1\n");
    
    sleep(1);
    
    printf("Поток 2 (без deadlock): пытается захватить мьютекс 2...\n");
    pthread_mutex_lock(&mutex2);
    printf("Поток 2 (без deadlock): захватил мьютекс 2\n");
    
    printf("Поток 2 (без deadlock): работает в критической секции\n");
    sleep(1);
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    
    printf("Поток 2 (без deadlock): завершил работу\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int choice;
    
    printf("=== Демонстрация Deadlock ===\n");
    printf("1 - Показать deadlock\n");
    printf("2 - Показать работу без deadlock\n");
    printf("Выберите вариант: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("\n--- Запуск с DEADLOCK ---\n");
        printf("Ожидаемое поведение: программа зависнет!\n\n");
        
        pthread_create(&thread1, NULL, thread1_function, NULL);
        pthread_create(&thread2, NULL, thread2_function, NULL);
        
        // Ждем завершения потоков (но они никогда не завершатся)
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        
    } else {
        printf("\n--- Запуск БЕЗ DEADLOCK ---\n");
        printf("Ожидаемое поведение: программа завершится корректно\n\n");
        
        pthread_create(&thread1, NULL, thread1_safe_function, NULL);
        pthread_create(&thread2, NULL, thread2_safe_function, NULL);
        
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        
        printf("\nОба потока успешно завершили работу!\n");
    }
    
    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    
    return 0;
}