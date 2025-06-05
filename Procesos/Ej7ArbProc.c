/**
 * @file Ej5ProcZombie.c
 * @brief Árbol de procesos con diferentes tiempos de vida
 * @author Salvador Gonzalez Arellano
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Hijo %d (PID=%d) creado por padre %d\n", i+1, getpid(), getppid());
            sleep(i+2);  // Cada hijo vive diferente tiempo
            printf("Hijo %d (PID=%d) termina\n", i+1, getpid());
            exit(i+1);
        }
    }

    // Padre espera a todos los hijos
    int status;
    while ((pid = wait(&status)) > 0) {
        printf("Padre: hijo con PID=%d terminó con código %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}