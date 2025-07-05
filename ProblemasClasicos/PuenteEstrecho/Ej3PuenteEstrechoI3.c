/**
 * @file Ej3PuenteEstrechoI3.c
 * @brief Solución al problema del puente estrecho utilizando turnos y contadores para 
 *        evitar inanición.
 *        Los vehículos de un lado ceden el paso al otro lado cada 10 cruces al menos.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_VEHICULOS_A 50
#define NUM_VEHICULOS_B 50
#define MAX_CRUZANDO_MISMO_LADO 10

/**
 * @struct EstructuraPuente
 * @brief Contiene toda la información compartida sobre el estado del puente.
 */
typedef struct {
    int adentroA;       // Cantidad de vehículos actualmente cruzando de A a B
    int adentroB;       // Cantidad de vehículos actualmente cruzando de B a A
    int esperandoA;     // Número de vehículos esperando del lado A
    int esperandoB;     // Número de vehículos esperando del lado B
    int cruzadosTurno;  // Número de vehículos que han cruzado en el turno actual
    int turno;          // 1 si es turno del lado A, 0 si es turno del lado B
} EstructuraPuente;

EstructuraPuente puente; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger las variables compartidas
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;     // Variable condición para sincronizar el cruce

/**
 * @brief Simula el cruce del puente por vehículos del lado A al B.
 * @param arg Puntero al índice del vehículo.
 * @return NULL
 */
void* cruzarAB(void* arg) {
    int indice = *(int *)arg;

    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente

    pthread_mutex_lock(&mutex);
    puente.esperandoA++;
    printf("Vehiculo %d del lado A esperando entrar al puente\n", indice);

    // Espera hasta que no haya vehículos del lado B y sea su turno (o del otro lado no hay vehiculos)
    while (puente.adentroB > 0 || (puente.turno == 0 && puente.esperandoB > 0)) {
        pthread_cond_wait(&cond, &mutex);
    }

    puente.esperandoA--;
    puente.adentroA++;
    puente.cruzadosTurno++;

    printf("Vehículo %d del lado A entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);

    // Simula el cruce
    usleep(100000);

    pthread_mutex_lock(&mutex);
    puente.adentroA--;

    printf("Vehículo %d del lado A saliendo del puente\n", indice);

    // Si ya cruzaron al menos 10 vehiculos del otro lado, se debe cambiar el turno
    if (puente.cruzadosTurno >= MAX_CRUZANDO_MISMO_LADO ) {
        puente.turno = 0; // Cede el turno a B
    }
    // Cuando el último vehiculo de este lado ha salido del puente
    if (puente.adentroA == 0) {
        puente.cruzadosTurno = 0;       // reiniciamos a 0 el contador
        pthread_cond_broadcast(&cond);  // Notifica a todos los que esperan
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

/**
 * @brief Simula el cruce del puente por vehículos del lado B al A.
 * @param arg Puntero al índice del vehículo.
 * @return NULL
 */
void* cruzarBA(void* arg) {
    int indice = *(int *)arg;

    usleep(200000 + rand() % 400001); // tiempo que le tomara llegar al puente

    pthread_mutex_lock(&mutex);
    puente.esperandoB++;
    printf("Vehiculo %d del lado B esperando entrar al puente\n", indice);

    // Espera hasta que no haya vehículos del lado A y sea su turno (o el otro lado no tiene espera)
    while (puente.adentroA > 0 || (puente.turno == 1 && puente.esperandoA > 0)) {
        pthread_cond_wait(&cond, &mutex);
    }

    puente.esperandoB--;
    puente.adentroB++;
    puente.cruzadosTurno++;

    printf("Vehículo %d del lado B entrando al puente\n", indice);
    pthread_mutex_unlock(&mutex);

    // Simula el cruce
    usleep(100000);

    pthread_mutex_lock(&mutex);
    puente.adentroB--;

    printf("Vehículo %d del lado B saliendo del puente\n", indice);

    // Si ya cruzaron al menos 10 vehiculos del otro lado, se debe cambiar el turno
    if (puente.cruzadosTurno >= MAX_CRUZANDO_MISMO_LADO ) {
        puente.turno = 1; // Cede el turno a B
    }
    // Cuando el último vehiculo de este lado ha salido del puente
    if (puente.adentroB == 0) {
        puente.cruzadosTurno = 0;       // reiniciamos a 0 el contador
        pthread_cond_broadcast(&cond);  // Notifica a todos los que esperan
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

/**
 * @brief Función principal: crea los hilos de ambos lados y espera a su finalización.
 */
int main() {
    pthread_t vehiculosA[NUM_VEHICULOS_A], vehiculosB[NUM_VEHICULOS_B];
    int indicesA[NUM_VEHICULOS_A], indicesB[NUM_VEHICULOS_B];
    puente.adentroA = 0;
    puente.adentroB = 0;
    puente.esperandoA = 0;
    puente.esperandoB = 0;
    puente.cruzadosTurno = 0;
    puente.turno = 1;
    srand(time(NULL));

    // Crear hilos del lado A
    for (int i = 0; i < NUM_VEHICULOS_A; i++) {
        indicesA[i] = i;
        pthread_create(&vehiculosA[i], NULL, cruzarAB, &indicesA[i]);
    }

    // Crear hilos del lado B
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

    // Liberar recursos
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
