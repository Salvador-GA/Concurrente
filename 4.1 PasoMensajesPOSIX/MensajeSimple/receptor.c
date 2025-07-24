/**
 * @file receptor.c
 * @brief Ejemplo para recibir un mensaje usando colas POSIX
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <fcntl.h>      // Para O_* constantes
#include <sys/stat.h>   // Para permisos

#define COLA_NOMBRE "/buzon"
#define MAX_MSG_SIZE 128

int main() {
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];
    struct mq_attr attr = {
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE
    };

    // Abrir o crear la cola con atributos
    mq = mq_open(COLA_NOMBRE, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("Error al abrir la cola");
        exit(1);
    }

    printf("Esperando mensaje...\n");

    // Recibir mensaje
    if (mq_receive(mq, buffer, MAX_MSG_SIZE, NULL) == -1) {
        perror("Error al recibir mensaje");
    } else {
        printf("Mensaje recibido: %s\n", buffer);
    }

    // Cerrar la cola
    mq_close(mq);

    // Eliminar la cola al terminar
    mq_unlink(COLA_NOMBRE);

    return 0;
}
