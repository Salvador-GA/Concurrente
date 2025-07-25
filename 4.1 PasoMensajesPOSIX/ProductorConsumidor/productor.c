/**
 * @file productor.c 
 * @brief Simula un proceso productor usando colas de mensajes POSIX.
 * Este programa crea una cola de mensajes y envía 25 mensajes de texto,
 * simulando la producción de datos. Usa mq_send() para enviar mensajes
 * y sleep() para simular tiempo de procesamiento.
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
 * @brief Función principal del productor.
 *
 * Crea (o abre) una cola POSIX y envía N mensajes.
 * Cada mensaje es una cadena indicando el número de dato producido.
 * Usa mq_open con los flags O_WRONLY | O_CREAT, lo que significa
 * que abrira la cola en modo escritura y la creara si no existe.
 */
int main() {
    mqd_t mq;                       // Descriptor de la cola de mensajes
    struct mq_attr attr = {         // Configurar los atributos de la cola
        .mq_maxmsg = MAX_MSG,       // Máximo número de mensajes en cola
        .mq_msgsize = MAX_MSG_SIZE  // Tamaño máximo de cada mensaje
    };

    // Crear o abrir la cola en modo escritura
    mq = mq_open(COLA, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1) {
        perror("mq_open productor");
        exit(1);
    }

    // Bucle de producción: enviar N mensajes al buzon
    for (int i = 0; i < N; i++) {
        char mensaje[MAX_MSG_SIZE];
        snprintf(mensaje, sizeof(mensaje), "Dato %d producido", i + 1);

        // Enviar mensaje a la cola
        if (mq_send(mq, mensaje, strlen(mensaje) + 1, 0) == -1) {
            perror("mq_send");
        } else {
            printf("✉ Productor: Enviado -> %s\n", mensaje);
        }

        sleep(1);  // Simula tiempo de producción
    }

    mq_close(mq);   // Cerrar descriptor de cola
    return 0;
}