/**
 * @file PuenteEstrechoSem.c
 * @brief Simulación del puente estrecho usando semáforos y mutex. 
 *        Puede causar inanición.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_VEHICULOS_A 50
#define NUM_VEHICULOS_B 50

typedef struct {
    int adentroA;
    int adentroB;
    int esperandoA;
    int esperandoB;
} EstructuraPuente;

EstructuraPuente puente;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semA; // Semáforo para vehículos del lado A
sem_t semB; // Semáforo para vehículos del lado B

/**
 * @brief Vehículo que cruza del lado A al B
 */
void* cruzarAB(void* arg) {
    int indice = *(int*)arg;
    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente
    // Solicita entrar
    pthread_mutex_lock(&mutex);
    if (puente.adentroB > 0) {
        puente.esperandoA++;
        pthread_mutex_unlock(&mutex);
        printf("Vehiculo %d del lado A esperando entrar al puente\n", indice);
        sem_wait(&semA);
        pthread_mutex_lock(&mutex);
        puente.esperandoA--;
    }
    puente.adentroA++;
    printf("Vehículo %d del lado A entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);
    usleep(100000); // Cruza
    // Termina de cruzar
    pthread_mutex_lock(&mutex);
    puente.adentroA--;
    printf("Vehículo %d del lado A saliendo del puente\n", indice);
    if (puente.adentroA == 0 && puente.esperandoB > 0) {
        for (int i = 0; i < puente.esperandoB; i++) {
            sem_post(&semB);
        }
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/**
 * @brief Vehículo que cruza del lado B al A
 */
void* cruzarBA(void* arg) {
    int indice = *(int*)arg;
    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente
    // Solicita entrar
    pthread_mutex_lock(&mutex);
    if (puente.adentroA > 0) {
        puente.esperandoB++;
        pthread_mutex_unlock(&mutex);
        printf("Vehiculo %d del lado B esperando entrar al puente\n", indice);
        sem_wait(&semB);
        pthread_mutex_lock(&mutex);
        puente.esperandoB--;
    }
    puente.adentroB++;
    printf("Vehículo %d del lado B entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);
    usleep(100000); // Cruza
    // Termina de cruzar
    pthread_mutex_lock(&mutex);
    puente.adentroB--;
    printf("Vehículo %d del lado B saliendo del puente\n", indice);
    if (puente.adentroB == 0 && puente.esperandoA > 0) {
        for (int i = 0; i < puente.esperandoA; i++) {
            sem_post(&semA);
        }
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t vehiculosA[NUM_VEHICULOS_A], vehiculosB[NUM_VEHICULOS_B];
    int indicesA[NUM_VEHICULOS_A], indicesB[NUM_VEHICULOS_B];
    puente.adentroA = 0;
    puente.adentroB = 0;
    puente.esperandoA = 0;
    puente.esperandoB = 0;
    sem_init(&semA, 0, 0);
    sem_init(&semB, 0, 0);
    srand(time(NULL));
    for (int i = 0; i < NUM_VEHICULOS_A; i++) {
        indicesA[i] = i;
        pthread_create(&vehiculosA[i], NULL, cruzarAB, &indicesA[i]);
    }
    for (int i = 0; i < NUM_VEHICULOS_B; i++) {
        indicesB[i] = i;
        pthread_create(&vehiculosB[i], NULL, cruzarBA, &indicesB[i]);
    }
    for (int i = 0; i < NUM_VEHICULOS_A; i++) {
        pthread_join(vehiculosA[i], NULL);
    }
    for (int i = 0; i < NUM_VEHICULOS_B; i++) {
        pthread_join(vehiculosB[i], NULL);
    }
    sem_destroy(&semA);
    sem_destroy(&semB);
    pthread_mutex_destroy(&mutex);
    return 0;
}
