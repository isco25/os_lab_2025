#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        printf("Дочерний процесс завершается...\n");
        exit(0); // Завершаем дочерний процесс
    } else if (pid > 0) {
        // Родительский процесс
        printf("Родительский процесс: PID = %d, создал дочерний с PID = %d\n", getpid(), pid);
        printf("Родительский процесс спит 30 секунд...\n");
        printf("В это время выполните: 'ps aux | grep %d'\n", pid);
        
        sleep(30); // Родитель спит, не собирая статус дочернего процесса
        
        printf("Родительский процесс просыпается и собирает статус дочернего...\n");
        wait(NULL); // Теперь собираем статус
        printf("Зомби процесс убран!\n");
    } else {
        perror("Ошибка при создании процесса");
        exit(1);
    }
    
    return 0;
}