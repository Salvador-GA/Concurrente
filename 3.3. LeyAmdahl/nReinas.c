/**
 * @file nReinas.c
 * @brief Solución al problema de las N reinas utilizando backtracking.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>

#define N 8  // Número de reinas y tamaño del tablero (N x N)

/**
 * @brief Imprime una solución del tablero con las posiciones de las reinas.
 *
 * @param tablero Arreglo que contiene la posición de las reinas.
 */
void imprimirTablero(int tablero[N]) {
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
 * @brief Función principal. Llama al solucionador del problema iniciando desde la fila 0.
 *
 * @return int Devuelve 0 al terminar correctamente.
 */
int main() {
    int tablero[N] = {0};
    colocarReinas(tablero, 0);
    return 0;
}