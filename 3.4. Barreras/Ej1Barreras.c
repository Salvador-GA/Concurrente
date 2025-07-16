/**
 * @file Ej1Barrera.c
 * @brief Ejemplo de uso de barrera con hilos POSIX (pthread_barrier_t)
 * @author Salvador Gonzalez Arellano
 * 
 * Este programa demuestra cómo utilizar una barrera para sincronizar
 * múltiples hilos. Cada hilo realiza una tarea simulada y espera
 * en la barrera hasta que todos hayan llegado, momento en el cual 
 * pueden continuar con su ejecución todos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Número de hilos a crear y sincronizar
#define NUM_HILOS 4

// Barrera global usada para sincronizar los hilos
pthread_barrier_t barrera;

/**
 * @brief Función que representa el trabajo de cada hilo.
 * 
 * Cada hilo realiza una simulación de trabajo (con sleep aleatorio),
 * luego espera en la barrera hasta que todos los hilos lleguen,
 * y finalmente continúa con su ejecución.
 * 
 * @param arg Puntero a un entero que representa el ID del hilo.
 * @return NULL
 */
void* trabajo(void* arg) {
    int id = *(int*)arg;

    printf("Hilo %d realizando trabajo previo a la barrera...\n", id);
    sleep(rand() % 10);  // Simula trabajo con duración variable

    printf("Hilo %d esperando en la barrera...\n", id);
    pthread_barrier_wait(&barrera);  // Punto de sincronización

    printf("Hilo %d continúa después de la barrera.\n", id);
    return NULL;
}

/**
 * @brief Función principal del programa.
 * 
 * Crea los hilos, inicializa la barrera, y espera a que todos los hilos
 * terminen. Destruye la barrera al finalizar.
 * 
 * @return int Código de salida del programa (0 si todo salió bien).
 */
int main() {
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Inicializa la barrera para NUM_HILOS hilos
    pthread_barrier_init(&barrera, NULL, NUM_HILOS);

    // Crea los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, trabajo, &ids[i]);
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destruye la barrera
    pthread_barrier_destroy(&barrera);

    return 0;
}
