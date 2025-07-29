/**
 * @file emisor.c
 * @brief Ejemplo para enviar un mensaje usando colas POSIX
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
    char mensaje[MAX_MSG_SIZE];
    struct mq_attr attr = {
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE
    };

    // Abrir o crear la cola con atributos en modo no bloqueante
    mq = mq_open(COLA_NOMBRE, O_WRONLY | O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("Error al abrir la cola");
        exit(1);
    }

    printf("Escribe un mensaje para enviar: ");
    fgets(mensaje, MAX_MSG_SIZE, stdin);
    mensaje[strcspn(mensaje, "\n")] = '\0'; // eliminar salto de línea

    // Enviar mensaje, si la cola de mensajes estuviera llena, el proceso no se bloquea
    // no completa la operacion y regresa
    if (mq_send(mq, mensaje, strlen(mensaje) + 1, 0) == -1) {
        perror("Error al enviar mensaje");
    } else {
        printf("Mensaje enviado: %s\n", mensaje);
    }

    // Cerrar la cola
    mq_close(mq);
    return 0;
}
