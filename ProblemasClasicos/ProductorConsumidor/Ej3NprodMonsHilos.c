/**
 * @file Ej3NprodMconsHilos.c
 * @brief Solución del problema productor-consumidor con N productores y M consumidores.
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
#define N 5                     // Cantidad de productores 
#define M 5                     // Cantidad de consumidores

int buffer[TAM_BUFFER];         // Bufer circular para los productos
int insercion = 0;              // Indice de insercion
int extraccion = 0;             // Indice de extraccion

sem_t espacios_libres;          // Semoforo para controlar espacios libres
sem_t elementos_disponibles;    // Semoforo para controlar productos disponibles
pthread_mutex_t mutexBuffer, mutexProd, mutexCons; // Candados para las secciones criticas

/**
 * @brief Función del productor. Produce un total de PRODUCCIONES.
 * @param arg No utilizado.
 * @return NULL
 */
void* productor(void* arg) {
    int producto, indice, sigPos;
    indice = *(int *)arg;
    for (int i = 0; i < PRODUCCIONES; i++) {
        producto = (rand() % 50) + 1;
        sem_wait(&espacios_libres);                 // Esperar un espacio libre
        pthread_mutex_lock(&mutexProd);             // Entrar a la sección crítica de los productores
        sigPos = insercion;                         // Obtenemos la siguiente posicion a insertar
        insercion = (insercion + 1) % TAM_BUFFER;   // Avanzar circularmente
        pthread_mutex_unlock(&mutexProd);           // Salir de la sección crítica de los productores
        pthread_mutex_lock(&mutexBuffer);           // Entrar a la sección crítica del buffer
        buffer[sigPos] = producto;                  // Insertar producto
        pthread_mutex_unlock(&mutexBuffer);         // Salir de la sección crítica del buffer
        sem_post(&elementos_disponibles);           // Indicar que hay un nuevo producto
        printf("Productor %d: inserto %d en posicion %d\n", indice, producto, sigPos);
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
    int producto, indice, sigPos;
    indice = *(int *)arg;
    for (int i = 0; i < PRODUCCIONES; i++) {
        sem_wait(&elementos_disponibles);           // Esperar un producto disponible
        pthread_mutex_lock(&mutexCons);             // Entrar a la sección crítica de los consumiddores
        sigPos = extraccion;                        // Obtenemos la siguiente posicion a leer
        extraccion = (extraccion + 1) % TAM_BUFFER; // Avanzar circularmente
        pthread_mutex_unlock(&mutexCons);           // Salir de la sección crítica de los consumiddores
        pthread_mutex_lock(&mutexBuffer);           // Entrar a la sección crítica
        producto = buffer[sigPos];                  // Extraer producto
        pthread_mutex_unlock(&mutexBuffer);         // Salir de la sección crítica
        sem_post(&espacios_libres);                 // Indicar que hay un nuevo espacio libre
        printf("Consumidor %d: consumio %d de posicion %d\n", indice, producto, sigPos);
        sleep(1);   // Simula trabajo
    }
    return NULL;
}

/**
 * @brief Función principal. Inicializa semáforos, crea hilos y los espera.
 */
int main() {
    pthread_t hProductores[N], hConsumidores[M];
    int indProd[N], indCons[M];
    srand(time(NULL));
    
    // Inicializar semáforos y mutex
    sem_init(&espacios_libres, 0, TAM_BUFFER);
    sem_init(&elementos_disponibles, 0, 0);
    pthread_mutex_init(&mutexBuffer, NULL);
    pthread_mutex_init(&mutexProd, NULL);
    pthread_mutex_init(&mutexCons, NULL);

    // Crear hilos
    for (int i=0; i<TAM_BUFFER; i++){
        indProd[i]=i;
        pthread_create(&hProductores[i], NULL, productor, &indProd[i]);
    }
    for (int i=0; i<M; i++){
        indCons[i]=i;
        pthread_create(&hConsumidores[i], NULL, consumidor, &indCons[i]);
    }

    // Esperar a que los hilos terminen
    for (int i=0; i<TAM_BUFFER; i++){
        pthread_join(hProductores[i], NULL);
    }
    for (int i=0; i<M; i++){
        indCons[i]=i;
        pthread_join(hConsumidores[i], NULL);
    }

    // Limpiar recursos
    sem_destroy(&espacios_libres);
    sem_destroy(&elementos_disponibles);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
