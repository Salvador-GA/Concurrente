/**
 * @file Ej1Pthread.c
 * @brief Ejemplo básico de creación de un hilo usando pthreads.
 * @author Salvador Gonzalez Arellano
 * 
 * Este programa muestra cómo usar pthreads para crear un nuevo hilo.
 * El hilo principal (proceso) y el nuevo hilo creado imprimen sus identificadores
 * para ilustrar la diferencia entre procesos e hilos en C.
 */

#include <stdio.h>      // printf()
#include <pthread.h>    // pthread_create(), pthread_join(), pthread_t
#include <unistd.h>     // getpid(), sleep()

/**
 * @brief Función que ejecuta el hilo secundario.
 * @param arg Argumento genérico (no se usa en este ejemplo).
 * @return Siempre NULL (no retorna datos).
 */
void *hilo_func(void *arg) {
    printf("Soy el hilo, mi id (pthread_self) es %lu y mi proceso es %d\n",
            pthread_self(), getpid());
    pthread_exit(NULL);     // Finaliza el hilo y libera sus recursos.
}

/**
 * @brief Función principal del programa.
 *        Crea un hilo secundario y espera a que termine.
 * @return 0 si todo va bien.
 */
int main() {
    /**
     * @var hilo
     * @brief Identificador del hilo creado.
     */
    pthread_t hilo;

    /**
     * @brief Creación del hilo.
     * @param hilo Puntero a pthread_t donde se guarda el identificador del hilo creado.
     * @param NULL Atributos (NULL usa atributos por defecto).
     * @param hilo_func Función que ejecutará el nuevo hilo.
     * @param NULL Argumento pasado a la función (no se usa en este ejemplo).
     * Otras opciones para atributos:
     * pthread_attr_t para configurar stack size, scheduling policy, etc.
     * La firma de la función de un hilo debe ser siempre:
     * void *nombre_funcion(void *arg);
     */
    if (pthread_create(&hilo, NULL, hilo_func, NULL) != 0) {
        perror("Error al crear hilo");
        return 1;
    }

    printf("Soy el hilo principal (proceso), PID=%d\n", getpid());

    /**
     * @brief Espera a que el hilo secundario termine.
     * @param hilo Identificador del hilo a esperar.
     * @param NULL Valor de retorno del hilo (no se usa en este ejemplo).
     * Nota: pthread_join es análogo a wait() para hilos.
     */
    pthread_join(hilo, NULL);

    printf("Soy el hilo principal el hilo secundario termino\n");

    return 0;
}