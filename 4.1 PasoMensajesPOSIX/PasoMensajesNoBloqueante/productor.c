/**
 * @file productor.c
 * @brief Productor que envía tareas urgentes o normales a colas POSIX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

#define URGENTE "/urgentes"
#define NORMAL "/normales"
#define MAX_MSG 10
#define TAM_MSG 128

int main() {
    struct mq_attr attr = {
        .mq_maxmsg = MAX_MSG,
        .mq_msgsize = TAM_MSG
    };

    // Se abren las colas de mensajes, por default bloqueante
    mqd_t mq_urg = mq_open(URGENTE, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq_norm = mq_open(NORMAL, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &attr);

    if (mq_urg == -1 || mq_norm == -1) {
        perror("Error al abrir colas");
        exit(1);
    }

    srand(time(NULL));
    int i=0;
    do {
        char msg[TAM_MSG];
        int tipo = rand() % 10;
        snprintf(msg, TAM_MSG, "Tarea #%d [%s]", i + 1, tipo > 6 ? "URGENTE" : "NORMAL");

        if (tipo > 6) {
            mq_send(mq_urg, msg, strlen(msg) + 1, 0);
        } else {
            mq_send(mq_norm, msg, strlen(msg) + 1, 0);
        }

        printf("📤 Enviado: %s\n", msg);
        sleep(1);
        i++;
    } while (1);

    // en realidad nunca se ejecutan, se tendría que cerrar las colas de mensajes manualmente desde la terminal
    mq_close(mq_urg);
    mq_close(mq_norm);
    return 0;
}
