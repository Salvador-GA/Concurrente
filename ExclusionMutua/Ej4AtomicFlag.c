/**
 * @file Ej4AtomicFlag.c
 * @brief Simula una región crítica protegida con atomic_flag (spinlock).
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h> // biblioteca para las instrucciones atomicas

#define NUM_HILOS 4
#define REPETICIONES 100000

int contador = 0;
atomic_flag lock = ATOMIC_FLAG_INIT;

void *trabajo(void *arg) {
    for (int i = 0; i < REPETICIONES; i++) {
        while (atomic_flag_test_and_set(&lock)) {
            // Espera activa mientras el lock esté en uso
        }

        contador++;  // Región crítica

        atomic_flag_clear(&lock);  // Libera el lock
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
