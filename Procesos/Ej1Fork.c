/**
 * @file Ej1Fork.c
 * @brief Ejemplo de uso básico de fork() para crear un proceso hijo.
 * @author Salvador Gonzalez Arellano
 *
 * Este programa muestra cómo usar fork() para crear un nuevo proceso en sistemas
 * compatibles con POSIX. El proceso padre y el proceso hijo imprimen sus 
 * respectivos identificadores de proceso (PID) y el del padre (PPID).
 */

#include <sys/types.h>  // Define tipos estándar como pid_t
#include <unistd.h>     // Proporciona fork(), getpid(), getppid()
#include <stdio.h>      // Funciones de entrada/salida estándar como printf()

/**
 * @brief Función principal que demuestra la creación de un proceso hijo.
 *
 * @return 0 en caso de éxito.
 */
int main() {
    /**
     * @var pid
     * @brief Variable para almacenar el PID del proceso creado por fork().
     */
    pid_t pid;

    /**
     * Llamada al sistema fork(). Crea un nuevo proceso hijo que es una copia
     * casi exacta del proceso padre.
     */
    pid = fork();

    if (pid == 0) {
        /**
         * Bloque ejecutado por el proceso hijo.
         * Se imprime el PID del hijo y el PID del padre del hijo.
         */
        printf("Soy el hijo, mi id es %d y mi padre es %d\n", getpid(), getppid());
    } else if (pid == -1) {
        /**
         * fork() ha fallado, se imprime un mensaje de error.
         */
        printf("fork devolvio error\n");
    } else {
        /**
         * Bloque ejecutado por el proceso padre.
         * Se imprime el PID del padre y el PID de su propio padre.
         */
        printf("Soy el padre, mi id es %d y mi padre es %d\n", getpid(), getppid());
    }
    
    return 0;
}