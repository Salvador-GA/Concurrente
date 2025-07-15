/**
 * @file Ej1FumadoresCigarrosI1.c
 * @brief Intento de solución al problema de los fumadores con semáforos y hilos.
 *        Tres fumadores esperan por los dos ingredientes que no tienen. El agente
 *        coloca dos ingredientes aleatorios sobre la mesa y uno de los fumadores
 *        los toma, forma su cigarro y fuma. Tiene interbloqueo.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t fosforo;
sem_t papel;
sem_t tabaco;
sem_t producir;  // Controla cuándo el agente debe producir más ingredientes

/**
 * @brief Función del fumador que tiene tabaco (T).
 *        Espera a tener fósforo y papel para formar su cigarro.
 * @param arg No usado.
 * @return NULL
 */
void* fumadorT(void* arg) {
    while (1) {
        sem_wait(&fosforo);
        sem_wait(&papel);
        printf("Fumador T (tiene tabaco) arma el cigarro y fuma.\n");
        usleep(500000);  // Simula el tiempo de fumar
        sem_post(&producir);
    }
    return NULL;
}

/**
 * @brief Función del fumador que tiene papel (P).
 *        Espera a tener fósforo y tabaco para formar su cigarro.
 * @param arg No usado.
 * @return NULL
 */
void* fumadorP(void* arg) {
    while (1) {
        sem_wait(&fosforo);
        sem_wait(&tabaco);
        printf("Fumador P (tiene papel) arma el cigarro y fuma.\n");
        usleep(500000);
        sem_post(&producir);
    }
    return NULL;
}

/**
 * @brief Función del fumador que tiene fósforo (F).
 *        Espera a tener papel y tabaco para formar su cigarro.
 * @param arg No usado.
 * @return NULL
 */
void* fumadorF(void* arg) {
    while (1) {
        sem_wait(&papel);
        sem_wait(&tabaco);
        printf("Fumador F (tiene fósforo) arma el cigarro y fuma.\n");
        usleep(500000);
        sem_post(&producir);
    }
    return NULL;
}

/**
 * @brief Función del agente que produce ingredientes aleatorios.
 *        Coloca dos ingredientes y espera que un fumador termine de fumar.
 * @param arg No usado.
 * @return NULL
 */
void* agente(void* arg) {
    while (1) {
        int r = rand() % 3;

        switch (r) {
            case 0:
                printf("Agente coloca fósforo y papel.\n");
                sem_post(&fosforo);
                sem_post(&papel);
                break;
            case 1:
                printf("Agente coloca fósforo y tabaco.\n");
                sem_post(&fosforo);
                sem_post(&tabaco);
                break;
            case 2:
                printf("Agente coloca papel y tabaco.\n");
                sem_post(&papel);
                sem_post(&tabaco);
                break;
        }

        sem_wait(&producir);  // Espera a que un fumador termine
    }
    return NULL;
}

/**
 * @brief Función principal. Inicializa semáforos, lanza hilos y espera su ejecución.
 */
int main() {
    pthread_t t_agente, t_fumadorT, t_fumadorP, t_fumadorF;

    srand(time(NULL));

    // Inicialización de semáforos
    sem_init(&fosforo, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&tabaco, 0, 0);
    sem_init(&producir, 0, 0);

    // Crear hilos
    pthread_create(&t_agente, NULL, agente, NULL);
    pthread_create(&t_fumadorT, NULL, fumadorT, NULL);
    pthread_create(&t_fumadorP, NULL, fumadorP, NULL);
    pthread_create(&t_fumadorF, NULL, fumadorF, NULL);

    // Esperar hilos (opcional: en este caso, ejecutan infinitamente)
    pthread_join(t_agente, NULL);
    pthread_join(t_fumadorT, NULL);
    pthread_join(t_fumadorP, NULL);
    pthread_join(t_fumadorF, NULL);

    // Liberar recursos
    sem_destroy(&fosforo);
    sem_destroy(&papel);
    sem_destroy(&tabaco);
    sem_destroy(&producir);

    return 0;
}
