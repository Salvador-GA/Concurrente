/**
 * @file Ej1FilosofosComensalesI1.c
 * @brief Primer intento de resolver el problema de los filodofos comensales.
 *        tiene el problema de interbloqueo.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define NUM_CICLOS 50

sem_t tenedores[NUM_FILOSOFOS]; // Semoforo para simular los tenedores

/**
 * @brief Función del filosofo. repite su comportamiento un total de NUM_CICLOS.
 * @param arg indice del filosofo.
 * @return NULL
 */
void* filosofo(void* arg) {
    int indice = *(int *)arg;
    int tiempoDormir;
    int izq = indice;
    int der = (indice+1) % NUM_FILOSOFOS;
    for (int i = 0; i < NUM_CICLOS; i++) {
        printf("Filosofo %d esta pensando\n",indice);
        tiempoDormir = 100000 + rand() % 400001;    // Aleatorio entre 0.1 y 0.5 segundos
        usleep(tiempoDormir);
        sem_wait(&tenedores[izq]);                  // Intenta topmar el tenedor izquierdo
        printf("Filosofo %d toma el tenedor izquierdo\n",indice);
        sem_wait(&tenedores[der]);                  // Intenta topmar el tenedor derecho
        printf("Filosofo %d toma el tenedor derecho\n",indice);
        printf("Filosofo %d esta comiendo\n",indice);
        tiempoDormir = 100000 + rand() % 400001;    // Aleatorio entre 0.1 y 0.5 segundos
        usleep(tiempoDormir);
        sem_post(&tenedores[izq]);                  // Libera el tenedor izquierdo
        printf("Filosofo %d deja el tenedor izquierdo\n",indice);
        sem_post(&tenedores[der]);                  // Libera el tenedor izquierdo
        printf("Filosofo %d deja el tenedor derecho\n",indice);
    }
    return NULL;
}

/**
 * @brief Función principal. Inicializa semáforos, crea hilos y los espera.
 */
int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int indices[NUM_FILOSOFOS];
    srand(time(NULL));
    
    // Inicializar semáforos
    for(int i=0; i<NUM_FILOSOFOS; i++){
        sem_init(&tenedores[i], 0, 1);
    }

    // Crear hilos
    for (int i=0; i<NUM_FILOSOFOS; i++){
        indices[i]=i;
        pthread_create(&filosofos[i], NULL, filosofo, &indices[i]);
    }

    // Esperar a que los hilos terminen
    for (int i=0; i<NUM_FILOSOFOS; i++){
        pthread_join(filosofos[i], NULL);
    }

    // Limpiar recursos
    for(int i=0; i<NUM_FILOSOFOS; i++){
        sem_destroy(&tenedores[i]);
    }

    return 0;
}