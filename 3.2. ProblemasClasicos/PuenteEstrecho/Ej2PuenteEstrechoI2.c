/**
 * @file Ej2PuenteEstrechoI2.c
 * @brief Primer intento del problema del puente estrecho. 
 *        Puede haber inanición, esta vez cambiando los semaforos 
 *        por una variable condicion.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_VEHICULOS_A 50  // Número de vehículos del lado A
#define NUM_VEHICULOS_B 50  // Número de vehículos del lado B

/**
 * @struct EstructuraPuente
 * @brief Estructura para simular el puente y saber cuántos vehículos hay dentro en cada dirección.
 */
typedef struct {
    int adentroA; // Vehículos del lado A actualmente en el puente
    int adentroB; // Vehículos del lado B actualmente en el puente
} EstructuraPuente;

EstructuraPuente puente; // Estado compartido del puente

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el acceso al puente
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    // Variable de condición para coordinar el cruce

/**
 * @brief Función para que crucen el puente los vehículos del lado A hacia B.
 * @param arg Índice del vehículo (cast a int*).
 * @return NULL
 */
void* cruzarAB(void* arg) {
    int indice = *(int *)arg;

    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente

    // Esperar hasta que no haya vehículos del lado B en el puente
    pthread_mutex_lock(&mutex);
    while (puente.adentroB > 0) {
        printf("Vehiculo %d del lado A esperando entrar al puente\n", indice);
        pthread_cond_wait(&cond, &mutex); // Esperar hasta que se libere el puente
    }
    puente.adentroA++; // Entrar al puente
    printf("Vehiculo %d del lado A entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);

    usleep(200000); // Simula el tiempo de cruce

    // Al salir del puente, notificar si es el último
    pthread_mutex_lock(&mutex);
    puente.adentroA--;
    if (puente.adentroA == 0) {
        pthread_cond_broadcast(&cond); // Despertar a todos los hilos en espera
    }
    printf("Vehiculo %d del lado A saliendo del puente\n", indice);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/**
 * @brief Función para que crucen el puente los vehículos del lado B hacia A.
 * @param arg Índice del vehículo (cast a int*).
 * @return NULL
 */
void* cruzarBA(void* arg) {
    int indice = *(int *)arg;

    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente

    // Esperar hasta que no haya vehículos del lado A en el puente
    pthread_mutex_lock(&mutex);
    while (puente.adentroA > 0) {
        printf("Vehiculo %d del lado B esperando entrar al puente\n", indice);
        pthread_cond_wait(&cond, &mutex); // Esperar hasta que se libere el puente
    }
    puente.adentroB++; // Entrar al puente
    printf("Vehiculo %d del lado B entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);

    usleep(200000); // Simula el tiempo de cruce

    // Al salir del puente, notificar si es el último
    pthread_mutex_lock(&mutex);
    puente.adentroB--;
    if (puente.adentroB == 0) {
        pthread_cond_broadcast(&cond); // Despertar a todos los hilos en espera
    }
    printf("Vehiculo %d del lado B saliendo del puente\n", indice);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/**
 * @brief Función principal. Inicializa estructuras y crea los hilos.
 */
int main() {
    pthread_t vehiculosA[NUM_VEHICULOS_A], vehiculosB[NUM_VEHICULOS_B];
    int indicesA[NUM_VEHICULOS_A], indicesB[NUM_VEHICULOS_B];
    // Inicializar los contadores del puente
    puente.adentroA = 0;
    puente.adentroB = 0;
    srand(time(NULL)); // Semilla para valores aleatorios (espaciado entre hilos)
    // Crear hilos para vehículos del lado A
    for (int i = 0; i < NUM_VEHICULOS_A; i++) {
        indicesA[i] = i;
        pthread_create(&vehiculosA[i], NULL, cruzarAB, &indicesA[i]);
    }
    // Crear hilos para vehículos del lado B
    for (int i = 0; i < NUM_VEHICULOS_B; i++) {
        indicesB[i] = i;
        pthread_create(&vehiculosB[i], NULL, cruzarBA, &indicesB[i]);
    }
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_VEHICULOS_A; i++) {
        pthread_join(vehiculosA[i], NULL);
    }
    for (int i = 0; i < NUM_VEHICULOS_B; i++) {
        pthread_join(vehiculosB[i], NULL);
    }
    // Destruir mutex y condición
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}