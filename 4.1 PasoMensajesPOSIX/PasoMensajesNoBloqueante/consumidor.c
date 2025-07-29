/**
 * @file consumidor.c
 * @brief Consumidor que revisa colas de urgentes y normales sin bloqueo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

#define URGENTE "/urgentes"
#define NORMAL "/normales"
#define MAX_MSG 10
#define TAM_MSG 128

int main() {
    struct mq_attr attr = {
        .mq_maxmsg = MAX_MSG,
        .mq_msgsize = TAM_MSG,
    };

    // Se abren las colas de mensajes en modo no bloqueante
    mqd_t mq_urg = mq_open(URGENTE, O_RDONLY | O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq_norm = mq_open(NORMAL, O_RDONLY | O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr);

    if (mq_urg == -1 || mq_norm == -1) {
        perror("Error al abrir colas");
        exit(1);
    }

    char buffer[TAM_MSG];

    while (1) {
        // interntamos primero recibir del buzon urgente
        ssize_t len = mq_receive(mq_urg, buffer, TAM_MSG, NULL);
        if (len > 0) {
            printf("✅ Procesado URGENTE: %s\n", buffer);
        } else {
            // si no hay mensajes urgentes intentamos noramles
            len = mq_receive(mq_norm, buffer, TAM_MSG, NULL);
            if (len > 0) {
                printf("🟦 Procesado NORMAL: %s\n", buffer);
            }
        }
        // Si no hay mensajes en ninguna cola
        if (errno == EAGAIN) {
            printf("🕒 Nada que hacer, ejecutando mantenimiento...\n");
        } else {
            perror("mq_receive");
        }

        sleep(2);
    }

    mq_close(mq_urg);
    mq_close(mq_norm);
    return 0;
}
