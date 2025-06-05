/**
 * @file Ej2Waitpid.c
 * @brief Ejemplo de creación de dos procesos hijos y espera de finalización con waitpid().
 * @author Salvador Gonzalez Arellano
 */

#include <sys/types.h>  // Define tipos estándar como pid_t
#include <unistd.h>     // Proporciona fork(), getpid(), getppid()
#include <stdio.h>      // Funciones de entrada/salida estándar como printf()
#include <sys/wait.h>   // Incluye waitpid() y macros relacionadas
 
int main() {
    pid_t pid1, pid2;
    int status;
    pid_t pid_terminado;
 
    /**
     * El proceso padre crea el primer proceso hijo.
     */
    pid1 = fork();
 
    if (pid1 == 0) {
        printf("Soy el primer hijo, mi id es %d y mi padre es %d\n", getpid(), getppid());
        sleep(5);
        printf("Primer hijo ha terminado.\n");
        return 0;
    } else if (pid1 < 0) {
        perror("Error al crear el primer hijo");
        return 1;
    }
 
    /**
     * El proceso padre crea el segundo proceso hijo.
     */
    pid2 = fork();
 
    if (pid2 == 0) {
        printf("Soy el segundo hijo, mi id es %d y mi padre es %d\n", getpid(), getppid());
        sleep(6);
        printf("Segundo hijo ha terminado.\n");
        return 0;
    } else if (pid2 < 0) {
        perror("Error al crear el segundo hijo");
        return 1;
    }
 
    /**
     * El proceso padre espera explícitamente a que terminen sus dos hijos.     *
     * Se utiliza waitpid() para esperar de forma específica por cada hijo.
     */
    // Esperar primero al primer hijo
    pid_terminado = waitpid(pid1, &status, 0);
    if (pid_terminado > 0) {
        printf("El padre detectó que terminó el primer hijo (PID: %d)\n", pid_terminado);
    }
 
    // Esperar luego al segundo hijo
    pid_terminado = waitpid(pid2, &status, 0);
    if (pid_terminado > 0) {
        printf("El padre detectó que terminó el segundo hijo (PID: %d)\n", pid_terminado);
    }
 
    printf("El proceso padre ha terminado.\n");
    return 0;
}