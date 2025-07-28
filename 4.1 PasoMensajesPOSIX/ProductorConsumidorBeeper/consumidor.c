/**
 * @file consumidor.c
 * @brief Consumidor de tareas usando buzón beeper para evitar bloqueo directo.
 *
 * Este programa espera en el buzón `/beeper` y, cuando recibe una notificación,
 * consulta si hay tareas disponibles en `/buzon1` o `/buzon2`. Esto evita
 * que el proceso se bloquee esperando directamente en los buzones de trabajo.
 * 
 * Se debe proporcionar un nombre de consumidor como argumento.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BEEPER "/beeper"    // Nombre del buzón de notificación
#define BUZON1 "/buzon1"    // Nombre del primer buzón de tareas
#define BUZON2 "/buzon2"    // Nombre del segundo buzón de tareas
#define PRODUCCIONES 25     // Numero de mensajes a consumir
#define MAX_MSG 10          // Maxima cantidad de mensajes
#define MAX_MSG_SIZE 128    // Tamaño máximo de cada mensaje

// Atributos para las colas de mensajes
struct mq_attr attr = {
    .mq_flags = 0,
    .mq_maxmsg = MAX_MSG,
    .mq_msgsize = MAX_MSG_SIZE
};

/**
 * @brief Función principal del consumidor.
 * 
 * Espera mensajes en el beeper y redirige la atención al buzón correspondiente
 * para procesar la tarea.
 * 
 * @param argc Número de argumentos (se espera 2)
 * @param argv Nombre del ejecutable y nombre del consumidor
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre_consumidor>\n", argv[0]);
        exit(1);
    }

    const char *nombre = argv[1];

    // Abrir colas de mensajes para lectura
    mqd_t mq_beeper = mq_open(BEEPER, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq1 = mq_open(BUZON1, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    mqd_t mq2 = mq_open(BUZON2, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);

    if (mq_beeper == -1 || mq1 == -1 || mq2 == -1) {
        perror("Error abriendo colas");
        exit(1);
    }

    char buffer[MAX_MSG_SIZE];  // buffer para el mensaje del beeper
    char tarea[MAX_MSG_SIZE];   // buffer para el mensaje de alguno de los buzones

    printf("🧑‍🔧 %s esperando en beeper...\n", nombre);

    for (int i=0; i<PRODUCCIONES; i++) {
        // simulamos el tipo de mensaje que se quiere recibir
        const char *esperado = (rand()%2 == 0) ? BUZON1 : BUZON2;
        // Esperar mensaje del beeper
        ssize_t bytes = mq_receive(mq_beeper, buffer, sizeof(buffer), NULL);
        if (bytes >= 0) {
            printf("🔔 %s: beeper indica mensaje en %s\n", nombre, buffer);
            // verificamos si el mensaje en el beeper es del tipo esperado
            if (strcmp(esperado,buffer) == 0) {
                ssize_t leido = 0;
                // Leer del buzón correspondiente
                if (strcmp(buffer, BUZON1) == 0){
                    leido = mq_receive(mq1, tarea, sizeof(tarea), NULL);
                } else {
                    leido = mq_receive(mq2, tarea, sizeof(tarea), NULL);
                }

                if (leido >= 0) {
                    tarea[leido] = '\0';
                    printf("✅ %s: procesa -> %s\n", nombre, tarea);
                }
            } else {
                // devolvemos el mensaje si no es del tipo esperado
                mq_send(mq_beeper, buffer, strlen(buffer) + 1, 0);
                i--;
            }
        }
    }

    // Revisión de colas al final (si se diera una condición de salida)
    struct mq_attr estado_beeper, estado_buzon1, estado_buzon2;
    mq_getattr(mq_beeper, &estado_beeper);
    mq_getattr(mq1, &estado_buzon1);
    mq_getattr(mq2, &estado_buzon2);

    mq_close(mq_beeper);
    mq_close(mq1);
    mq_close(mq2);

    if (estado_beeper.mq_curmsgs == 0 &&
        estado_buzon1.mq_curmsgs == 0 &&
        estado_buzon2.mq_curmsgs == 0) {
        // Este consumidor se encarga de liberar los recursos
        mq_unlink(BEEPER);
        mq_unlink(BUZON1);
        mq_unlink(BUZON2);
    }
    return 0;
}
