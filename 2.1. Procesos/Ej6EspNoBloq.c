/**
 * @file Ej5ProcZombie.c
 * @brief Ejemplo de waitpid con opción WNOHANG: no bloquear
 * @author Salvador Gonzalez Arellano
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        sleep(2);
        printf("Hijo (PID=%d) termina\n", getpid());
        exit(42);
        /**
         * Aunque en C puedes usar cualquier número (0–255), hay valores convencionales en algunos sistemas UNIX y Linux:
         * Código	Significado típico
         * 0	    Éxito
         * 1	    Error genérico / Fallo en ejecución
         * 2	    Error de uso incorrecto de la línea de comandos
         * 126	    No se puede ejecutar el comando
         * 127	    Comando no encontrado
         * 128+n	Terminación por señal n (ej. 128+9 = 137 para SIGKILL)
         * 255	    Error definido por el programa o desbordamiento
         */
    } else if (pid > 0) {
        int status;
        pid_t result;

        do {
            result = waitpid(pid, &status, WNOHANG);  // No bloquea
            if (result == 0) {
                printf("Padre (PID=%d): hijo aún no ha terminado\n", getpid());
                sleep(1);
            }
        } while (result == 0);

        if (WIFEXITED(status)) {
            printf("Padre: hijo terminó con código %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("fork");
    }

    return 0;
}
