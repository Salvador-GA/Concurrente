/**
 * @file Ej3AtomicInt.c
 * @brief Uso de atomic_int para incremento seguro entre hilos.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h> // biblioteca para las instrucciones atomicas

#define NUM_HILOS 4
#define REPETICIONES 100000

atomic_int contador = 0; // contador es de tipo atomic_int

void *trabajo(void *arg) {
    for (int i = 0; i < REPETICIONES; i++) {
        atomic_fetch_add(&contador, 1);  // Incremento seguro y atómico
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
    return 0;
}
