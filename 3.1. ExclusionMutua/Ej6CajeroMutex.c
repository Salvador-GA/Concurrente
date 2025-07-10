/**
 * @file Ej6CajeroMutex.c
 * @brief Simulación de cajeros concurrentes accediendo a una cuenta compartida usando pthread_mutex.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 5                 // Numero de hilos
#define OPERACIONES 100     // Operaciones a realizar

int saldo = 1000;           // Saldo inicial de la cuenta
pthread_mutex_t mutex;      // Mutex para sincronizar acceso a saldo

/**
 * @brief Función simulada de cajero que accede a saldo compartido.
 * @param arg apuntador al índice del hilo.
 */
void *cajero(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < OPERACIONES; i++) {
        int operacion = rand() % 2;         // 0: retiro, 1: depósito
        int cantidad = (rand() % 50) + 1;   // monto de la operación

        // Entrar a la región crítica
        pthread_mutex_lock(&mutex);

        if (operacion == 0) {
            if (saldo >= cantidad) {
                saldo -= cantidad;
                printf("Hilo %d retiró $%d. Saldo: %d\n", id, cantidad, saldo);
            } else {
                printf("Hilo %d no pudo retirar $%d. Saldo insuficiente: %d\n", id, cantidad, saldo);
            }
        } else {
            saldo += cantidad;
            printf("Hilo %d depositó $%d. Saldo: %d\n", id, cantidad, saldo);
        }

        // Salir de la región crítica
        pthread_mutex_unlock(&mutex);

        usleep(10000); // duerme 10 ms
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[N];
    int indices[N];

    srand(time(NULL)); // Semilla para rand
    pthread_mutex_init(&mutex, NULL); // Inicializa el mutex con atributos por defecto

    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&hilos[i], NULL, cajero, &indices[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // Libera recursos asociados al mutex

    printf("Saldo final: %d\n", saldo);
    return 0;
}