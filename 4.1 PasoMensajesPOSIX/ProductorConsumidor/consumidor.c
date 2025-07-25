/**
 * @file consumidor.c
 * @brief Simula un proceso consumidor usando colas de mensajes POSIX.
 * Este programa abre o crea una cola POSIX en modo lectura, y recibe
 * 25 mensajes usando mq_receive(). Imprime cada mensaje recibido
 * y simula el tiempo de consumo.
 * @author Salvador Gonzalez Arellano
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define N 25                // Número total de mensajes a producir
#define COLA "/buzon"       // Nombre simbólico de la cola POSIX
#define MAX_MSG 10          // Maximo numero de mensajes en la cola
#define MAX_MSG_SIZE 128    // Tamaño maximo de cada mensaje

/**
 * @brief Función principal del consumidor.
 *
 * Abre la cola de mensajes en modo lectura (O_RDONLY) y recibe N mensajes.
 * Imprime cada mensaje recibido y luego elimina la cola del sistema con `mq_unlink`.
 */
int main() {
    char buffer[MAX_MSG_SIZE];      // para almacenar los mensajes recibidos
    mqd_t mq;                       // Descriptor de la cola de mensajes
    struct mq_attr attr = {         // Configurar los atributos de la cola
        .mq_maxmsg = MAX_MSG,       // Máximo número de mensajes en cola
        .mq_msgsize = MAX_MSG_SIZE  // Tamaño máximo de cada mensaje
    };

    // Abrir o crear la cola en modo lectura
    mq = mq_open(COLA, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("mq_open consumidor");
        exit(1);
    }

    printf("👀 Consumidor esperando mensajes...\n");

    // Recibir mensajes uno por uno
    for (int i = 0; i < N; i++) {
        ssize_t bytes = mq_receive(mq, buffer, sizeof(buffer), NULL);
        if (bytes >= 0) {
            printf("💻 Consumidor: Recibido -> %s\n", buffer);
        } else {
            perror("mq_receive");
        }
        sleep(2);  // Simula tiempo de consumo
    }

    mq_close(mq);       // Cerrar y eliminar la cola
    mq_unlink(COLA);    // Borra la cola del sistema
    return 0;
}