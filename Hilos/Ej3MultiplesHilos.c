/**
 * @file EjMultiplesHilos.c
 * @brief Crear varios hilos con diferentes tiempos de vida (equivalente a Ej7ArbProc.c con procesos).
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>      // printf()
#include <pthread.h>    // pthread_create(), pthread_join()
#include <unistd.h>     // sleep(), getpid()

/**
 * @brief Función que ejecuta cada hilo.
 * @param arg Puntero a entero que indica el número del hilo.
 * @return NULL
 */
void *hilo_func(void *arg) {
    int num = *(int *)arg;
    printf("Hilo %d (ID=%lu) creado, PID=%d\n", num, pthread_self(), getpid());
    sleep(num + 1);
    printf("Hilo %d (ID=%lu) termina\n", num, pthread_self());
    return NULL;
}

int main() {
    pthread_t hilos[5];
    int numeros[5] = {1, 2, 3, 4, 5};

    /**
     * Crear los hilos.
     */
    for (int i = 0; i < 5; i++) {
        pthread_create(&hilos[i], NULL, hilo_func, &numeros[i]);
    }

    /**
     * Esperar a que terminen los hilos.
     */
    for (int i = 0; i < 5; i++) {
        pthread_join(hilos[i], NULL);
        printf("Hilo principal: El hilo %d terminó\n", i + 1);
    }

    printf("Hilo principal: Todos los hilos terminaron.\n");
    return 0;
}