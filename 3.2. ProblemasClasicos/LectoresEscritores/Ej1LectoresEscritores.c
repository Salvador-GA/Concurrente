/**
 * @file Ej1LectoresEscritores.c
 * @brief Solución al problema de los lectores-escritores.
 *        Lectores pueden leer concurrentemente si no hay escritor activo.
 *        Escritores tienen acceso exclusivo.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LECTORES 15    // Número de hilos lectores
#define NUM_ESCRITORES 5   // Número de hilos escritores
#define TAM_BUFFER 100     // Tamaño del buffer compartido

char buffer[TAM_BUFFER];   // Buffer compartido de lectura/escritura

/**
 * @struct Estado
 * @brief Estructura que representa el estado global de la sala de lectura/escritura.
 */
typedef struct {
    int la; // Lectores activos (esperando o trabajando)
    int lt; // Lectores trabajando (ya tienen permiso)
    int ea; // Escritores activos (esperando o trabajando)
    int et; // Escritores trabajando (ya tienen permiso)
} Estado;

Estado estado = {0, 0, 0, 0}; // Estado inicializado a cero

sem_t leyendo;           // Semáforo que controla entrada de lectores
sem_t escribiendo;       // Semáforo que controla entrada de escritores
pthread_mutex_t estado_mutex = PTHREAD_MUTEX_INITIALIZER; // Protege acceso al estado
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER; // Protege acceso al buffer

/**
 * @brief Otorga permisos de lectura si no hay escritores activos.
 *        Despierta a los lectores en espera.
 */
void permisoLectura() {
    if (estado.ea == 0) {
        while (estado.lt < estado.la) {
            estado.lt++;               // Un lector más puede pasar
            sem_post(&leyendo);        // Libera un lector
        }
    }
}

/**
 * @brief Otorga permisos de escritura si no hay lectores trabajando.
 *        Despierta a los escritores en espera.
 */
void permisoEscritura() {
    if (estado.lt == 0) {
        while (estado.et < estado.ea) {
            estado.et++;               // Un escritor más puede pasar
            sem_post(&escribiendo);    // Libera un escritor
        }
    }
}

/**
 * @brief Función ejecutada por cada hilo lector.
 * @param arg Puntero al identificador del lector.
 * @return NULL
 */
void* lector(void* arg) {
    int id = *(int*)arg;

    usleep(200000 + rand() % 400001);   // Simula tiempo antes de intentar leer

    pthread_mutex_lock(&estado_mutex);
    estado.la++;                        // Se registra como lector activo
    permisoLectura();                   // Verifica si puede leer ahora
    pthread_mutex_unlock(&estado_mutex);

    sem_wait(&leyendo);                 // Espera permiso para leer

    pthread_mutex_lock(&buffer_mutex);
    printf("Lector %d leyendo...\n", id);
    usleep(100000);                     // Simula tiempo de lectura
    printf("Lector %d leyó: \"%s\"\n", id, buffer);
    pthread_mutex_unlock(&buffer_mutex);

    pthread_mutex_lock(&estado_mutex);
    estado.la--;                        // Deja de ser lector activo
    estado.lt--;                        // Deja de ser lector trabajando
    permisoEscritura();                 // Si hay escritores esperando, podrían pasar
    pthread_mutex_unlock(&estado_mutex);

    printf("Lector %d terminó de leer.\n", id);
    return NULL;
}

/**
 * @brief Función ejecutada por cada hilo escritor.
 * @param arg Puntero al identificador del escritor.
 * @return NULL
 */
void* escritor(void* arg) {
    int id = *(int*)arg;

    usleep(100000 + rand() % 500001);   // Simula tiempo antes de intentar escribir

    pthread_mutex_lock(&estado_mutex);
    estado.ea++;                        // Se registra como escritor activo
    permisoEscritura();                 // Verifica si puede escribir ahora
    pthread_mutex_unlock(&estado_mutex);

    sem_wait(&escribiendo);             // Espera permiso para escribir

    pthread_mutex_lock(&buffer_mutex);
    printf("Escritor %d escribiendo...\n", id);
    usleep(100000);                     // Simula tiempo de escritura
    snprintf(buffer, TAM_BUFFER, "Mensaje escrito por el escritor %d", id);
    pthread_mutex_unlock(&buffer_mutex);

    pthread_mutex_lock(&estado_mutex);
    estado.ea--;                        // Deja de ser escritor activo
    estado.et--;                        // Deja de ser escritor trabajando
    permisoLectura();                   // Si hay lectores esperando, podrían pasar
    pthread_mutex_unlock(&estado_mutex);

    printf("Escritor %d terminó de escribir.\n", id);
    return NULL;
}

/**
 * @brief Función principal. Inicializa recursos, crea hilos y espera su finalización.
 * @return int Código de salida.
 */
int main() {
    pthread_t hilos_lectores[NUM_LECTORES], hilos_escritores[NUM_ESCRITORES];
    int indices_lectores[NUM_LECTORES], indices_escritores[NUM_ESCRITORES];

    buffer[0] = '\0';  // Inicializa el buffer con cadena vacía

    // Inicialización de semáforos
    sem_init(&leyendo, 0, 0);
    sem_init(&escribiendo, 0, 0);

    // Crear hilos lectores
    for (int i = 0; i < NUM_LECTORES; i++) {
        indices_lectores[i] = i + 1;
        pthread_create(&hilos_lectores[i], NULL, lector, &indices_lectores[i]);
    }

    // Crear hilos escritores
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        indices_escritores[i] = i + 1;
        pthread_create(&hilos_escritores[i], NULL, escritor, &indices_escritores[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_LECTORES; i++) {
        pthread_join(hilos_lectores[i], NULL);
    }
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        pthread_join(hilos_escritores[i], NULL);
    }

    // Liberar recursos
    sem_destroy(&leyendo);
    sem_destroy(&escribiendo);
    pthread_mutex_destroy(&estado_mutex);
    pthread_mutex_destroy(&buffer_mutex);

    return 0;
}