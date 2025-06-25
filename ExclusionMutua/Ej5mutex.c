/**
 * @file mutex_ejemplo.c
 * @brief Ejemplo de uso de mutex para exclusión mutua entre hilos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4
#define REPETICIONES 100000

int contador = 0;
// Declara e inicializa un mutex con los valores por defecto
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void *trabajo(void *arg) {
    for (int i = 0; i < REPETICIONES; i++) {
        pthread_mutex_lock(&mutex);   // Adquiere el mutex
        contador++;                   // Región crítica
        pthread_mutex_unlock(&mutex); // Libera el mutex
    }
    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_create(&hilos[i], NULL, trabajo, NULL);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Valor esperado: %d\n", NUM_HILOS * REPETICIONES);
    printf("Valor real: %d\n", contador);
    pthread_mutex_destroy(&mutex);  // Libera el recurso del sistema
    return 0;
}
