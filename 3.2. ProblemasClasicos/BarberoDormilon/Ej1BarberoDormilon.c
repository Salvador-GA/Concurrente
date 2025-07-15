/**
 * @file Ej1BarberoDormilon.c
 * @brief Solución al problema del barbero dormilón utilizando semáforos.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CLIENTES 10    // Número total de clientes
#define NUM_ASIENTOS 3     // Número de asientos disponibles en la sala de espera

int asientosLibres = NUM_ASIENTOS; // Asientos libres en la sala de espera

sem_t barberoL;    // Semáforo que indica si el barbero está listo para cortar
sem_t clienteL;    // Semáforo que indica si hay un cliente esperando
pthread_mutex_t sala_mutex = PTHREAD_MUTEX_INITIALIZER;  // Exclusión mutua al acceder a la sala de espera

/**
 * @brief Función que representa al barbero. Atiende clientes mientras haya.
 * @param arg No se utiliza.
 * @return NULL
 */
void* barbero(void* arg) {
    while (1) {
        sem_wait(&clienteL);            // Espera a que llegue un cliente
        pthread_mutex_lock(&sala_mutex);     // Entra a la sala de espera
        asientosLibres++;               // Libera un asiento en la sala
        sem_post(&barberoL);            // Señala que el barbero está listo
        pthread_mutex_unlock(&sala_mutex);   // Sale de la sala de espera
        printf("Barbero: cortando el cabello...\n");
        usleep(100000);                 // Simula el tiempo del corte
        printf("Barbero: terminó el corte.\n");
    }
    return NULL;
}

/**
 * @brief Función que representa a un cliente. Intenta obtener un corte.
 * @param arg Puntero al identificador del cliente.
 * @return NULL
 */
void* cliente(void* arg) {
    int id = *(int*)arg;

    usleep(rand() % 2000000);   // Simula el tiempo en que llega el cliente
    pthread_mutex_lock(&sala_mutex); // Entra a la sala de espera (sección crítica)
    if (asientosLibres > 0) {
        asientosLibres--;       // Ocupa un asiento
        printf("Cliente %d: se sienta a esperar. Asientos disponibles: %d\n", id, asientosLibres);
        sem_post(&clienteL);    // Notifica al barbero
        pthread_mutex_unlock(&sala_mutex);  // Sale de la sección crítica
        sem_wait(&barberoL);    // Espera a ser atendido
        printf("Cliente %d: está recibiendo el corte.\n", id);
    } else {
        pthread_mutex_unlock(&sala_mutex);   // Sala llena, libera acceso
        printf("Cliente %d: se va sin corte (sala llena).\n", id);
    }
    return NULL;
}

/**
 * @brief Función principal: inicializa semáforos, crea hilos de barbero y clientes.
 */
int main() {
    pthread_t hilo_barbero;
    pthread_t hilos_clientes[NUM_CLIENTES];
    int ids[NUM_CLIENTES];

    // Inicializar semáforos
    sem_init(&barberoL, 0, 0);
    sem_init(&clienteL, 0, 0);

    // Crear hilo del barbero
    pthread_create(&hilo_barbero, NULL, barbero, NULL);

    // Crear hilos de clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos_clientes[i], NULL, cliente, &ids[i]);
    }

    // Esperar a que terminen los clientes (opcional, según simulación)
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(hilos_clientes[i], NULL);
    }

    // Nota: el barbero corre indefinidamente

    // Destruir semáforos
    sem_destroy(&barberoL);
    sem_destroy(&clienteL);
    pthread_mutex_destroy(&sala_mutex);

    return 0;
}
