/**
 * @file Ej2FuncParam.c
 * @brief Ejemplo de creación de un hilo que calcula una potencia.
 * @author Salvador Gonzalez Arellano
 *
 * Este programa muestra cómo pasar argumentos a un hilo y cómo recuperar
 * un valor de retorno usando pthreads.
 */

#include <stdio.h>      // printf()
#include <pthread.h>    // pthread_create(), pthread_join(), pthread_t
#include <stdlib.h>     // malloc(), free()
#include <unistd.h>     // getpid(), sleep()

/**
 * @struct Parametros
 * @brief Estructura para pasar argumentos al hilo.
 */
typedef struct {
    int base;        ///< Base de la potencia.
    int exponente;   ///< Exponente de la potencia.
} Parametros;

/**
 * @brief Función que calcula la potencia.
 *
 * @param arg Puntero a estructura Parametros.
 * @return Puntero al resultado calculado (debe ser liberado por el hilo principal).
 */
void *calcular_potencia(void *arg) {
    /**
     * Hacemos un cast para interpretar el puntero arg que es void como
     * un puntero a estructura Parametros
     */
    Parametros *p = (Parametros *)arg;
    int resultado = 1;

    for (int i = 0; i < p->exponente; i++) {
        resultado *= p->base;
    }

    printf("Hilo (ID=%lu, PID=%d): Calculé %d^%d = %d\n",
           pthread_self(), getpid(), p->base, p->exponente, resultado);

    // Liberar memoria de los argumentos si ya no se usa
    free(p);

    // Reservar memoria para el resultado
    int *res_ptr = malloc(sizeof(int));
    *res_ptr = resultado;
    pthread_exit(res_ptr);  // Retorna puntero al resultado.
}

/**
 * @brief Función principal del programa.
 * Crea un hilo para calcular una potencia y recupera el valor.
 *
 * @return 0 si todo va bien.
 */
int main() {
    pthread_t hilo;

    // Reservar memoria para la estructura de argumentos
    Parametros *param = malloc(sizeof(Parametros));
    param->base = 2;
    param->exponente = 5;

    /**
     * @brief Creación del hilo.
     *
     * @param &hilo      Puntero a pthread_t donde se guarda el identificador del hilo creado.
     * @param NULL       Atributos (NULL usa atributos por defecto).
     * @param calcular_potencia Función que ejecutará el nuevo hilo.
     * @param param      Argumento pasado a la función (estructura con base y exponente).
     * La firma de la función de un hilo debe ser siempre:
     * void *nombre_funcion(void *arg);
     */
    if (pthread_create(&hilo, NULL, calcular_potencia, param) != 0) {
        perror("Error al crear hilo");
        free(param);
        return 1;
    }

    printf("Soy el hilo principal (proceso), PID=%d\n", getpid());

    /**
     * @brief Espera a que el hilo termine y recupera el valor de retorno.
     *
     * @param hilo     Identificador del hilo a esperar.
     * @param &resultado_ptr Puntero a void* donde se guardará el valor retornado por el hilo.
     */
    void *resultado_ptr;
    pthread_join(hilo, &resultado_ptr);

    // Mostrar el resultado calculado
    int resultado = *(int *)resultado_ptr;
    printf("Hilo principal: El resultado recibido del hilo es %d\n", resultado);

    // Liberar la memoria del resultado
    free(resultado_ptr);

    return 0;
}
