/**
 * @file receptor.c
 * @brief Ejemplo para recibir un mensaje sin bloqueo usando colas POSIX
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <fcntl.h>      // Para O_* constantes
#include <sys/stat.h>   // Para permisos
#include <errno.h>
#include <unistd.h>

#define COLA_NOMBRE "/buzon"
#define MAX_MSG_SIZE 128

int main() {
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];
    struct mq_attr attr = {
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE
    };

    // Abrir o crear la cola con atributos en modo no bloqueante
    mq = mq_open(COLA_NOMBRE, O_RDONLY | O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("Error al abrir la cola");
        exit(1);
    }

    do {
        printf("Esperando mensaje...\n");

        // Intentar recibir sin bloqueo
        if (mq_receive(mq, buffer, MAX_MSG_SIZE, NULL) == -1) {
            if (errno == EAGAIN){ // La cola de mensajes esta vacia
                printf("No hay mensajes disponibles.\n");
            } else { // cualquier otro error ocurrio
                perror("mq_receive");
            }
        } else {
            printf("Mensaje recibido: %s\n", buffer);
            // Cerrar la cola
            mq_close(mq);
            // Eliminar la cola al terminar
            mq_unlink(COLA_NOMBRE);
            // Terminamos el programa
            return 0;
        }
        sleep(1);
    } while (1);
}
