/**
 * @file filosofos_semaforos.c
 * @brief Solución de Dijkstra al problema de los filósofos comensales, usa estados.
 *        Cada filósofo solo puede comer si sus vecinos no están comiendo.
 * @author 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define NUM_CICLOS 50

#define PENSANDO 0
#define HAMBRIENTO 1
#define COMIENDO 2

int estado[NUM_FILOSOFOS];  // Estado de cada filósofo
sem_t sem[NUM_FILOSOFOS];   // Semaforo privado para cada filósofo
pthread_mutex_t mutex;      // Mutex global para acceder a los estados

/**
 * @brief Devuelve el antecesor de indice en aritmetica modular
 * @return indice izquierdo
 */
int izquierda(int indice) { 
    return (indice + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS; 
}

/**
 * @brief Devuelve el sucesor de indice en aritmetica modular
 * @return indice derecho
 */
int derecha(int indice) { 
    return (indice + 1) % NUM_FILOSOFOS; 
}

/**
 * @brief Verifica si el filósofo puede comenzar a comer.
 * Si los vecinos no están comiendo, se le da permiso (sem_post).
 * @param indice del filosofo que va verificar si puede comer
 */
void probar(int indice) {
    if (estado[indice] == HAMBRIENTO &&             // Si quiere comer
        estado[izquierda(indice)] != COMIENDO &&    // y el izquierdo no esta comiendo
        estado[derecha(indice)] != COMIENDO) {      // y el derecho no esta comiendo
        
        estado[indice] = COMIENDO;  // Cambia su estado a PENSANDO
        sem_post(&sem[indice]);     // Permitir que el filósofo coma
    }
}

/**
 * @brief Toma los tenedores si es posible, o espera.
 * @param indice del filosofo que va a tomar los tenedores
 */
void tomar_tenedores(int indice) {
    pthread_mutex_lock(&mutex);
    estado[indice] = HAMBRIENTO;    // Cambia su estado a PENSANDO
    printf("Filósofo %d tiene hambre.\n", indice);
    probar(indice);                 // Se verifica si el mismo puede comer
    pthread_mutex_unlock(&mutex);
    sem_wait(&sem[indice]);         // Espera si no puede comer todavía
}

/**
 * @brief Suelta los tenedores y permite que los vecinos coman si pueden.
 * @param indice del filosofo que va a dejar los tenedores
 */
void soltar_tenedores(int indice) {
    pthread_mutex_lock(&mutex);
    estado[indice] = PENSANDO;  // Cambia su estado a PENSANDO
    printf("Filósofo %d deja de comer y se pone a pensar.\n", indice);
    probar(izquierda(indice));  // Se verifica si el vecino izquierdo puede comer
    probar(derecha(indice));    // Se verifica si el vecino derecho puede comer
    pthread_mutex_unlock(&mutex);
}

/**
 * @brief Función del filosofo. repite su comportamiento un total de NUM_CICLOS.
 * @param arg indice del filosofo.
 * @return NULL
 */
void *filosofo(void *arg) {
    int indice = *(int *)arg;
    int tiempoDormir;

    for (int i = 0; i < NUM_CICLOS; i++) {
        printf("Filosofo %d esta pensando\n",indice);
        tiempoDormir = 100000 + rand() % 400001;    // Aleatorio entre 0.1 y 0.5 segundos
        usleep(tiempoDormir);
        tomar_tenedores(indice);                    // Intentar comer
        printf("Filosofo %d esta comiendo\n",indice);
        tiempoDormir = 100000 + rand() % 400001;    // Aleatorio entre 0.1 y 0.5 segundos
        usleep(tiempoDormir);
        soltar_tenedores(indice);
    }
    return NULL;
}

/**
 * @brief Función principal. Inicializa semáforos, crea hilos y los espera.
 */
int main() {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];
    srand(time(NULL));

    // Inicialización del mutex, semaforos y estados
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&sem[i], 0, 0);
        estado[i] = PENSANDO;
    }

    // Crear hilos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, filosofo, &ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Limpiar recursos
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_destroy(&sem[i]);
    }

    return 0;
}
