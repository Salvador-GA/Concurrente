/**
 * @file Ej5ProcZombie.c
 * @brief Ejemplo de un proceso zombie
 * @author Salvador Gonzalez Arellano
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Hijo termina rápido
        printf("Hijo (PID=%d) termina\n", getpid());
        exit(0);
    } else {
        // Padre no espera inmediatamente
        sleep(5);
        printf("Padre (PID=%d): revisa con 'ps' y verás al hijo como zombie\n", getpid());
    }

    return 0;
}
