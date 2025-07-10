/**
 * @file nReinasHilos.c
 * @brief Solución concurrente al problema de las 8 reinas usando solamente 8 hilos.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 13               // Número de reinas y tamaño del tablero (N x N)
#define NUM_THREADS 13     // Número de hilos (uno por cada posición en la primera fila)

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger la salida estándar

/**
 * @brief Imprime una solución del tablero con las posiciones de las reinas.
 *
 * @param tablero Arreglo que contiene la posición de las reinas.
 */
void imprimirTablero(int tablero[N]) {
    pthread_mutex_lock(&print_mutex);
    printf("\n--- Solución ---\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tablero[i] == j) {
                printf(" R ");
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
    pthread_mutex_unlock(&print_mutex);
}

/**
 * @brief Verifica si se puede colocar una reina en la posición (fila, col).
 *
 * @param tablero Arreglo que representa la posición de las reinas.
 * @param fila Fila actual.
 * @param col Columna que se quiere verificar.
 * @return 1 si la posición es válida, 0 si no lo es.
 */
int esValido(int tablero[N], int fila, int col) {
    for (int i = 0; i < fila; i++) {
        if (tablero[i] == col || abs(tablero[i] - col) == abs(i - fila)) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Función recursiva que intenta colocar reinas en el tablero.
 *
 * @param tablero Arreglo donde tablero[i] indica la columna donde se colocó la reina en la fila i.
 * @param fila Fila actual donde se intenta colocar una reina.
 */
void colocarReinas(int tablero[N], int fila) {
    if (fila == N) {
        imprimirTablero(tablero);
        return;
    }

    for (int col = 0; col < N; col++) {
        if (esValido(tablero, fila, col)) {
            tablero[fila] = col;
            colocarReinas(tablero, fila + 1);
        }
    }
}

/**
 * @brief Función ejecutada por cada hilo.
 *        Coloca una reina en la fila 0 en la columna especificada y resuelve recursivamente desde la fila 1.
 *
 * @param arg Puntero a un entero que indica la columna inicial donde se coloca la primera reina.
 * @return void* No devuelve ningún valor. Solo se usa con pthread.
 */
void* hilo_worker(void* arg) {
    int col = *(int*)arg;
    int tablero[N];
    tablero[0] = col;
    colocarReinas(tablero, 1);  // Comenzar desde fila 1
    return NULL;
}

/**
 * @brief Función principal. Crea un hilo por cada posible posición de la primera reina.
 *
 * @return int Devuelve 0 al terminar correctamente.
 */
int main() {
    pthread_t hilos[NUM_THREADS];
    int columnas[NUM_THREADS];

    // Crear hilos con diferentes posiciones para la reina en la fila 0
    for (int i = 0; i < NUM_THREADS; i++) {
        columnas[i] = i;
        pthread_create(&hilos[i], NULL, hilo_worker, &columnas[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&print_mutex);
    return 0;
}
