/**
 * @file ejercicio2_cajeros.c
 * @brief Simulación de cajeros concurrentes accediendo a una cuenta compartida.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5                 // Numero de hilos
#define OPERACIONES 100     // Operaciones a realizar

int saldo = 1000;           // Saldo inicial de la cuenta
int level[N];               // Nivel en el que se encuentra cada hilo
int last_to_enter[N - 1];   // Ultimo hilo que entro a cada nivel

/**
 * @brief Comprobar si existe k != i, tal que level[k] >= l
 * @param i indice del hilo
 * @param l nivel a revisar
 * @return 1 si la condición se cumple para alguna k, 0 en otro caso
 */
int compNivSup(int i, int l) {
    for (int k=0; k<N; k++) {
        if((k != i) && level[k] >= l) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Metodo con la parte que corresponde al algoritmo de filtro para entrar a 
 *        la region critica
 * @param i indice del hilo
 */
void entrar_region_critica(int i) {
    for (int l=0; l<(N-1); l++) {
        level[i] = l;
        last_to_enter[l] = i;
        while ( (last_to_enter[l] == i) && compNivSup(i,l));
    }
}

/**
 * @brief Metodo con la parte que corresponde al algoritmo de filtro para salir de 
 *        la region critica
 * @param i indice del hilo
 */
void salir_region_critica(int i){
    level[i] = -1;
}

/**
 * @brief Función simulada de cajero que accede a saldo compartido.
 * @param arg apuntador al indice del hilo.
 */
void *cajero(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < OPERACIONES; i++) {
        int operacion = rand() % 2;         // 0: retiro, 1: depósito
        int cantidad = (rand() % 50) + 1;   // monto de la operacion

        entrar_region_critica(id);
        // Region critica
        if (operacion == 0) { 
            if(saldo >= cantidad) {
                saldo -= cantidad;
                printf("Hilo %d retiró $%d. Saldo: %d\n", id, cantidad, saldo);
            } else {
                printf("Hilo %d no se retiró $%d. Saldo insuficiente: %d\n", id, cantidad, saldo);
            }
        } else {
            saldo += cantidad;
            printf("Hilo %d depositó $%d. Saldo: %d\n", id, cantidad, saldo);
        }
        salir_region_critica(id);
        // Region no critica
        usleep(10000); // duerme 10,000 microsegundos = 10 milisegundos = 0.01 segundos
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[N];
    int indices[N];

    srand(time(NULL)); // Para cambiar la semilla de rand en cada ejecucion

    /** Inicializamos los niveles del algoritmo de filtro a -1, inicialmente no hay nadie */
    for (int i = 0; i < N; i++) {
        level[i] = -1;
    }

    /** Creamos los hilos */
    for (int i = 0; i < N; i++) {
        indices[i] = i;
        pthread_create(&hilos[i], NULL, cajero, &indices[i]);
    }

    /** Esperamos a que los hilos terminen */
    for (int i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Saldo final: %d\n", saldo);
    return 0;
}