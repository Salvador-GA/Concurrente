/**
 * @file Ej5_1mutexATTR.c
 * @brief Ejemplo de uso de mutex recursivo para exclusión mutua entre hilos.
 * @author Salvador Gonzalez Arellano
 */
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_mutexattr_t attr;

/**
 * @brief Función recursiva que entra en una sección crítica varias veces
 * 
 * @param n Nivel de profundidad
 */
void seccion_critica_recursiva(int n) {
    pthread_mutex_lock(&mutex);
    printf("  [Nivel %d] Hilo en sección crítica recursiva\n", n);

    if (n > 0) {
        seccion_critica_recursiva(n - 1);
    }

    pthread_mutex_unlock(&mutex);
}

/**
 * @brief Función que ejecuta cada hilo
 * 
 * @param arg ID del hilo (entero convertido a void*)
 * @return void* 
 */
void* tarea(void* arg) {
    long id = (long)arg;
    printf("Hilo %ld comenzando...\n", id);
    
    seccion_critica_recursiva(3);  // entra recursivamente con mutex

    printf("Hilo %ld terminó.\n", id);
    return NULL;
}

int main() {
    pthread_t hilos[2];

    // Inicializar atributos
    pthread_mutexattr_init(&attr);

    // Establecer atributo: mutex recursivo
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    // Crear el mutex con esos atributos
    pthread_mutex_init(&mutex, &attr);

    // Usar los hilos normalmente
    for (long i = 0; i < 2; i++) {
        pthread_create(&hilos[i], NULL, tarea, (void*)i);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);

    return 0;
}

/**
 * Atributo	                    Descripción
 * PTHREAD_MUTEX_RECURSIVE      Permite múltiples lock por el mismo hilo
 * PTHREAD_MUTEX_ERRORCHECK	    Detecta errores como desbloquear sin haber bloqueado
 * PTHREAD_MUTEX_DEFAULT	    Mutex normal (el predeterminado)
 * PTHREAD_PROCESS_SHARED 	    Permite que el mutex sea compartido entre procesos distintos (si está en memoria compartida)
 */