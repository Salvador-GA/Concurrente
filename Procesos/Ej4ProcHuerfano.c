/**
 * @file Ej4ProcHuerfano.c
 * @brief Proceso huérfano: el hijo sobrevive mas tiempo que el padre
 * @author Salvador Gonzalez Arellano
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        // Padre muere rápido
        printf("Padre (PID=%d) muere\n", getpid());
        exit(0);
    } else if (pid == 0) {
        // Hijo espera un momento
        sleep(2);
        printf("Hijo (PID=%d) ahora tiene nuevo padre: %d\n", getpid(), getppid());
    } else {
        perror("fork");
    }

    return 0;
}
