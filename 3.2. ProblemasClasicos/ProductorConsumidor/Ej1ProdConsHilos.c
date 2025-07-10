/**
 * @file Ej1ProdConsHilos.c
 * @brief Solución del problema productor-consumidor con 1 productor y 1 consumidor.
 *        Utiliza pthreads, mutex y semáforos POSIX.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define PRODUCCIONES 20

int buffer[TAM_BUFFER];         // Bufer circular para los productos
int insercion = 0;              // Indice de insercion
int extraccion = 0;             // Indice de extraccion

sem_t espacios_libres;          // Semoforo para controlar espacios libres
sem_t elementos_disponibles;    // Semoforo para controlar productos disponibles
pthread_mutex_t mutexBuffer;    // Candado para la sección crítica

/**
 * @brief Función del productor. Produce un total de PRODUCCIONES.
 * @param arg No utilizado.
 * @return NULL
 */
void* productor(void* arg) {
    int producto;
    for (int i = 0; i < PRODUCCIONES; i++) {
        producto = (rand() % 50) + 1;
        sem_wait(&espacios_libres);         // Esperar un espacio libre
        pthread_mutex_lock(&mutexBuffer);   // Entrar a la sección crítica
        buffer[insercion] = producto;       // Insertar producto
        pthread_mutex_unlock(&mutexBuffer); // Salir de la sección crítica
        sem_post(&elementos_disponibles);   // Indicar que hay un nuevo producto
        printf("Productor: inserto %d en posicion %d\n", producto, insercion);
        insercion = (insercion + 1) % TAM_BUFFER;   // Avanzar circularmente
        sleep(1);   // Simula trabajo
    }
    return NULL;
}

/**
 * @brief Función del consumidor. Consume un total de PRODUCCIONES.
 * @param arg No utilizado.
 * @return NULL
 */
void* consumidor(void* arg) {
    int producto;
    for (int i = 0; i < PRODUCCIONES; i++) {
        sem_wait(&elementos_disponibles);   // Esperar un producto disponible
        pthread_mutex_lock(&mutexBuffer);   // Entrar a la sección crítica
        producto = buffer[extraccion];      // Extraer producto
        pthread_mutex_unlock(&mutexBuffer); // Salir de la sección crítica
        sem_post(&espacios_libres);         // Indicar que hay un nuevo espacio libre
        printf("Consumidor: consumio %d de posicion %d\n", producto, extraccion);
        extraccion = (extraccion + 1) % TAM_BUFFER; // Avanzar circularmente
        sleep(1);   // Simula trabajo
    }
    return NULL;
}

/**
 * @brief Función principal. Inicializa semáforos, crea hilos y los espera.
 */
int main() {
    pthread_t hProductor, hConsumidor;
    srand(time(NULL));
    
    // Inicializar semáforos y mutex
    sem_init(&espacios_libres, 0, TAM_BUFFER);
    sem_init(&elementos_disponibles, 0, 0);
    pthread_mutex_init(&mutexBuffer, NULL);

    // Crear hilos
    pthread_create(&hProductor, NULL, productor, NULL);
    pthread_create(&hConsumidor, NULL, consumidor, NULL);

    // Esperar a que los hilos terminen
    pthread_join(hProductor, NULL);
    pthread_join(hConsumidor, NULL);

    // Limpiar recursos
    sem_destroy(&espacios_libres);
    sem_destroy(&elementos_disponibles);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
