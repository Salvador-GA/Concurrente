/**
 * @file Ej3Execv.c
 * @brief En este ejemplo el padre crea un hijo con fork() y el hijo usa execv() para 
 * ejecutar /bin/ls.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>      // printf, perror
#include <unistd.h>     // fork, execv
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid
#include <stdlib.h>     // exit
 
int main() {
    pid_t pid = fork();
 
    if (pid < 0) {
        /**
         * Cuando una llamada al sistema (como fork(), open(), read(), etc.) falla, establece la 
         * variable global errno con un código de error. perror() toma una cadena de texto que tu le 
         * das como argumento y luego imprime:
         * <string que le pasas>: <descripción del error almacenado en errno>
         */
        perror("Error en fork");
        exit(1);
    } else if (pid == 0) {
        // Proceso hijo
        printf("Soy el hijo (PID: %d). Voy a ejecutar /bin/ls.\n", getpid());
        
        /**
         * Los argumentos dependen del programa que se va a ejecutar
         * Pero siempre comienzan con el nombre del programa y terminan con NULL  
         */
        char *args[] = {"/bin/ls", "-l", "./", NULL};
 
        // Reemplazar el hijo con el programa /bin/ls
        execv("/bin/ls", args);
 
        // Si execv falla
        perror("Error en execv");
        exit(1);
    } else {
        // Proceso padre
        printf("Soy el padre (PID: %d). Espero que el hijo termine.\n", getpid());
 
        int status;
        waitpid(pid, &status, 0);
 
        /**
         * WIFEXITED(status) responde:
         *    true  → (valor distinto de cero) → El hijo terminó con exit() o return del main().
         *    false → El hijo terminó de forma anormal (por ejemplo, con kill -9, una señal, etc.).
         */
        if (WIFEXITED(status)) {
            printf("El hijo terminó con código: %d\n", WEXITSTATUS(status));
        } else {
            printf("El hijo terminó de forma anormal.\n");
        }
 
        printf("El padre ha terminado.\n");
    }
 
    return 0;
 }