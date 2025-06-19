/**
 * @file Ej1AlgoritmoFiltro.c
 * @brief Demuestra el uso del algoritmo de filtro en una condición de carrera al 
 *        incrementar una variable global sin sincronización.
 * @author Salvador Gonzalez Arellano
 *
 * Aunque el algoritmo lógico sea correcto, el hardware y el compilador pueden no 
 * respetarlo sin un mecanismo que garantice la visibilidad entre hilos. Esto es lo 
 * que justifica el uso de primitivas como mutexes, semáforos, y atómicos.
 * Las escrituras de una variable compartida (level[i], last_to_enter[l]) pueden no 
 * ser visibles inmediatamente para los otros hilos.
 * El compilador y la CPU pueden reordenar instrucciones o almacenar en cachés privadas 
 * que no se sincronizan automáticamente. 
 * Esto provoca que dos hilos entren simultáneamente a la región crítica a pesar de 
 * que el algoritmo debería impedirlo.
 */

#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 4     // Número de hilos a utilizar

int contador = 0;                   // Variable compartida
int level[NUM_HILOS];               // Nivel en el que se encuentra cada hilo
int last_to_enter[NUM_HILOS - 1];   // Ultimo hilo que entro a cada nivel

/**
 * @brief Comprobar si existe k != i, tal que level[k] >= l
 * @param i indice del hilo
 * @param l nivel a revisar
 * @return 1 si la condición se cumple para alguna k, 0 en otro caso
 */
int compNivSup(int i, int l) {
    for (int k=0; k<NUM_HILOS; k++) {
        if((k != i) && level[k] >= l) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Incrementa el contador muchas veces usando el algoritmo.
 *        de filtro
 * @param arg apuntador al indice del hilo.
 */
void *incrementar(void *arg) {
    int i = *(int *)arg; // indice del hilo

    for (int j = 0; j < 100000; j++) {
        /** Inicia el algoritmo de filtro */
        for (int l=0; l<(NUM_HILOS-1); l++) {
            level[i] = l;           // El hilo i indica que se encuentra en el nivel l
            last_to_enter[l] = i;   // el hilo i avisa que es el ultimo en entrar al nivel l
            while ( (last_to_enter[l] == i) && compNivSup(i,l));
            /** 
             * Mientras el hilo i sea el ultimo en entrar al nivel l y exista otro hilo
             * en un nivel superior o en el mismo que i, entonces el hilo i se queda en una 
             * espera ocupada en el while
            */
        }
        /** Region critica */
        contador++;
        /** Saliendo de la region critica */
        level[i] = -1;
        /** Region no critica */
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int indices[NUM_HILOS];  // Se requiere un arreglo separado para evitar condiciones de carrera

    /** Inicializamos los niveles del algoritmo de filtro a -1, inicialmente no hay nadie */
    for (int i = 0; i < NUM_HILOS; i++) {
        level[i] = -1;
    }

    /** Creamos los hilos */
    for (int j = 0; j < NUM_HILOS; j++) {
        indices[j] = j;
        pthread_create(&hilos[j], NULL, incrementar, &indices[j]);
    }

    /** Esperamos a que los hilos terminen */
    for (int j = 0; j < NUM_HILOS; j++) {
        pthread_join(hilos[j], NULL);
    }

    printf("Valor esperado: %d\n",100000*NUM_HILOS);
    printf("Valor real: %d\n", contador);
    return 0;
}
