/**
 * @file prodcons.c 
 * @brief Simula un proceso de productor y consumidor usando colas de mensajes POSIX.
 * Este programa implementa la logica de productor.c y concumidor.c pero esta vez
 * en este mismo programa se crea otro proceso con fork para que cada uno
 * tenga uno de los comportamientos.
 * @author Salvador Gonzalez Arellano
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 25                // Número total de mensajes a producir
#define COLA "/buzon"       // Nombre simbólico de la cola POSIX
#define MAX_MSG 10          // Maximo numero de mensajes en la cola
#define MAX_MSG_SIZE 128    // Tamaño maximo de cada mensaje

int main() {
    mqd_t mq;                       // Descriptor de la cola de mensajes
    struct mq_attr attr = {         // Configurar los atributos de la cola
        .mq_maxmsg = MAX_MSG,       // Máximo número de mensajes en cola
        .mq_msgsize = MAX_MSG_SIZE  // Tamaño máximo de cada mensaje
    };

    mq_unlink(COLA);    // Asegurarse de que la cola no exista previamente

    // Crear la cola antes de hacer fork()
    mq = mq_open(COLA, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Creamos otro porceos
    pid_t pid = fork();

    if (pid < 0) {  // Si hay error al crear el proceso cerramos todo
        perror("fork");
        mq_close(mq);
        mq_unlink(COLA);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 👶 Proceso hijo: consumidor
        printf("👀 Consumidor esperando mensajes...\n");
        char buffer[MAX_MSG_SIZE];
        for (int i = 0; i < N; i++) {
            ssize_t bytes = mq_receive(mq, buffer, sizeof(buffer), NULL);
            if (bytes >= 0) {
                printf("💻 Consumidor: Recibido -> %s\n", buffer);
            } else {
                perror("mq_receive");
            }
            sleep(2); // Simula tiempo de consumo
        }
        mq_close(mq);
        exit(EXIT_SUCCESS);
    } else {
        // 👨‍👦 Proceso padre: productor
        for (int i = 0; i < N; i++) {
            char mensaje[MAX_MSG_SIZE];
            snprintf(mensaje, sizeof(mensaje), "Dato %d producido", i + 1);
            if (mq_send(mq, mensaje, strlen(mensaje) + 1, 0) == -1) {
                perror("mq_send");
            } else {
                printf("✉ Productor: Enviado -> %s\n", mensaje);
            }
            sleep(1); // Simula tiempo de producción
        }

        wait(NULL);         // Esperar al hijo
        mq_close(mq);       // Cerrar y eliminar la cola
        mq_unlink(COLA);    // Borra la cola del sistema
        exit(EXIT_SUCCESS);
    }
}