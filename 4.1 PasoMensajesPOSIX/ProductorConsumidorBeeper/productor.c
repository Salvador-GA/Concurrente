/**
 * @file productor.c
 * @brief Productor con múltiples buzones y esquema beeper usando colas POSIX.
 *
 * Este programa implementa un productor que genera tareas aleatorias para dos buzones
 * (`/buzon1` y `/buzon2`) y notifica a través del buzón `/beeper` cuál buzón ha recibido una tarea.
 * Así, los consumidores pueden esperar eficientemente en el beeper en lugar de bloquearse en cada buzón.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BEEPER "/beeper"    // Nombre del buzón de notificación
#define BUZON1 "/buzon1"    // Nombre del primer buzón de tareas
#define BUZON2 "/buzon2"    // Nombre del segundo buzón de tareas
#define PRODUCCIONES 50     // Numero de mensajes a consumir
#define MAX_MSG 10          // Maxima cantidad de mensajes
#define MAX_MSG_SIZE 128    // Tamaño máximo de cada mensaje

/// Atributos de la cola de mensajes
struct mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = MAX_MSG,
    .mq_msgsize = MAX_MSG_SIZE
};

/**
 * @brief Función principal del productor.
 * 
 * Crea los buzones y envía tareas a buzón1 o buzón2 al azar, notificando en el beeper.
 */
int main() {
    srand(time(NULL));

    // Abrir colas de mensajes para escritura
    mqd_t mq_beeper = mq_open(BEEPER, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq1 = mq_open(BUZON1, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq2 = mq_open(BUZON2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    if (mq_beeper == -1 || mq1 == -1 || mq2 == -1) {
        perror("Error abriendo colas");
        exit(1);
    }

    for (int i = 0; i < PRODUCCIONES; i++) {
        char mensaje[MAX_MSG_SIZE];
        const char *destino = (rand()%2 == 0) ? BUZON1 : BUZON2;

        snprintf(mensaje, sizeof(mensaje), "Tarea #%d para %s \n", i + 1, destino);

        // Enviar tarea al buzón correspondiente
        if (strcmp(destino, BUZON1) == 0){
            mq_send(mq1, mensaje, strlen(mensaje) + 1, 0);
        } else {
            mq_send(mq2, mensaje, strlen(mensaje) + 1, 0);
        }

        // Notificar al beeper
        mq_send(mq_beeper, destino, strlen(destino) + 1, 0);

        printf("📤 Productor: tarea #%d enviada a %s \n", i + 1, destino);
        sleep(1);
    }

    // Cierre
    mq_close(mq_beeper);
    mq_close(mq1);
    mq_close(mq2);
    return 0;
}
