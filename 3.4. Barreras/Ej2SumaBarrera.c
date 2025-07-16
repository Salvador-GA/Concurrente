/**
 * @file Ej2SumaBarrera.c
 * @brief Ejemplo de suma de arreglo usando hilos POSIX y sincronización con barrera
 * @author Salvador Gonzalez Arellano
 *
 * El arreglo se divide en bloques y cada hilo calcula la suma parcial de su bloque.
 * Después de la sincronización con barrera, el hilo 0 suma todos los resultados parciales
 * y muestra la suma total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4             // Número de hilos
#define TAM_ARREGLO 1000        // Tamaño total del arreglo

int arreglo[TAM_ARREGLO];       // Arreglo de entrada
int sumas_parciales[NUM_HILOS]; // Arreglo para guardar la suma de cada hilo

pthread_barrier_t barrera;      // Barrera para sincronización de hilos

/**
 * @brief Función que ejecuta cada hilo.
 *
 * Calcula la suma parcial de una sección del arreglo, la almacena en un arreglo global
 * y espera en la barrera. Después de la barrera, el hilo 0 imprime la suma total.
 *
 * @param arg Puntero a entero que representa el ID del hilo.
 * @return NULL
 */
void* sumar_bloque(void* arg) {
    int indice = *(int*)arg;
    int paso = TAM_ARREGLO / NUM_HILOS;
    int inicio =  (indice == 0) ? 0 : indice * paso + 1;
    int fin = (indice == NUM_HILOS - 1) ? TAM_ARREGLO : (indice + 1) * paso;

    int suma = 0;
    for (int i = inicio; i < fin; i++) {
        suma += arreglo[i];
    }
    sumas_parciales[indice] = suma;
    printf("Hilo %d: suma parcial [%d - %d] = %d\n", indice, inicio, fin, suma);

    // Sincronización con barrera
    pthread_barrier_wait(&barrera);

    int suma_total = 0;
    for (int i = 0; i < NUM_HILOS; i++) {
        suma_total += sumas_parciales[i];
    }
    printf("Suma total calculada por hilo %d: %d\n", indice, suma_total);

    return NULL;
}

/**
 * @brief Función principal.
 *
 * Inicializa el arreglo, crea los hilos, espera su finalización y libera recursos.
 *
 * @return int Código de salida del programa.
 */
int main() {
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Inicializa el arreglo con valores de 1 a 16
    for (int i = 0; i < TAM_ARREGLO; i++) {
        arreglo[i] = i + 1;
    }

    pthread_barrier_init(&barrera, NULL, NUM_HILOS);

    // Crea los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, sumar_bloque, &ids[i]);
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_barrier_destroy(&barrera);
    return 0;
}
