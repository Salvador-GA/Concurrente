/**
 * @file Ej4SumaN.c
 * @brief Cada hilo calcula una parte de la suma de los primeros N enteros.
 * Los resultados parciales se almacenan en un arreglo global y luego se suman.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <pthread.h>

#define N 10000          // Valor máximo a sumar
#define NUM_HILOS 4     // Número de hilos a utilizar

int parciales[NUM_HILOS]; // Arreglo global para almacenar los resultados parciales

/**
 * @brief Función que calcula la suma de un rango determinado por el índice del hilo.
 * @param arg Puntero a entero (índice del hilo).
 * @return NULL
 */
void *sumar_parcial(void *arg) {
    int indice = *(int *)arg;
    int paso = N / NUM_HILOS;
    int inicio = indice * paso + 1;
    int fin = (indice == NUM_HILOS - 1) ? N : (indice + 1) * paso;
    int suma = 0;

    for (int i = inicio; i <= fin; i++) {
        suma += i;
    }

    parciales[indice] = suma;

    printf("Hilo %d: suma [%d - %d] = %d\n", indice, inicio, fin, suma);
    return NULL;
}

/**
 * @brief Función principal que lanza los hilos y suma los resultados parciales.
 * @return int Código de salida.
 */
int main() {
    pthread_t hilos[NUM_HILOS];
    int indices[NUM_HILOS];  // Se requiere un arreglo separado para evitar condiciones de carrera

    for (int i = 0; i < NUM_HILOS; i++) {
        indices[i] = i;
        pthread_create(&hilos[i], NULL, sumar_parcial, &indices[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    int suma_total = 0;
    for (int i = 0; i < NUM_HILOS; i++) {
        suma_total += parciales[i];
    }

    printf("Suma total de 1 a %d = %d\n", N, suma_total);
    return 0;
}
