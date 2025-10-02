#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s seed arraysize\n", argv[0]);
        return 1;
    }
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс - запускаем sequential_min_max
        printf("Дочерний процесс: Запуск sequential_min_max...\n");
        
        char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};
        
        execvp(args[0], args);
        
        // Если execvp вернул управление, значит произошла ошибка
        perror("Ошибка execvp");
        exit(1);
    } else if (pid > 0) {
        // Родительский процесс
        printf("Родительский процесс: Запущен дочерний процесс с PID: %d\n", pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Родительский процесс: Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Родительский процесс: Дочерний процесс завершен сигналом: %d\n", WTERMSIG(status));
        }
    } else {
        perror("Ошибка fork");
        return 1;
    }
    
    return 0;
}