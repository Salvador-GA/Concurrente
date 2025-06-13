/**
 * @file Ej6CondCarrera.c
 * @brief Demuestra una condición de carrera al incrementar una variable global sin sincronización.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 4     // Número de hilos a utilizar

int contador = 0;       // Variable compartida no protegida

/**
 * @brief Incrementa el contador muchas veces sin sincronización.
 * @param arg No se usa.
 * @return NULL
 */
void *incrementar(void *arg) {
    for (int i = 0; i < 100000; i++) {
        contador++;
    }
    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_create(&hilos[i], NULL, incrementar, NULL);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Valor esperado: %d\n",100000*NUM_HILOS);
    printf("Valor real: %d\n", contador); ///< Este valor usualmente será incorrecto
    return 0;
}
