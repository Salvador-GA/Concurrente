/**
 * @file nReinasConcurrente.c
 * @brief Solución concurrente al problema de las 8 reinas usando procesos y fork().
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 8    // Número de reinas y tamaño del tablero (N x N)

/**
 * @brief Imprime una solución del tablero con las posiciones de las reinas.
 *
 * @param tablero Arreglo que contiene la posición de las reinas.
 */
void imprimirTablero(int tablero[N]) {
    printf("\n--- Solución encontrada por proceso %d ---\n", getpid());
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
        exit(0);  // Finaliza el proceso al encontrar una solución completa
    }

    for (int col = 0; col < N; col++) {
        if (esValido(tablero, fila, col)) {
            int nuevo_tablero[N];
            for (int i = 0; i < N; i++) nuevo_tablero[i] = tablero[i];
            nuevo_tablero[fila] = col;

            pid_t pid = fork();
            if (pid == 0) {
                // Proceso hijo continúa con la siguiente fila
                colocarReinas(nuevo_tablero, fila + 1);
                exit(0);  // El hijo debe terminar después de la búsqueda
            }
        }
    }

    // Solo el proceso original espera por sus hijos
    while (wait(NULL) > 0);
}

/**
 * @brief Función principal: genera 8 procesos, cada uno con una reina en la primera fila.
 *
 * @return int Devuelve 0 al terminar correctamente.
 */
int main() {
    for (int col = 0; col < N; col++) {
        pid_t pid = fork();
        if (pid == 0) {
            int tablero[N] = {0};
            tablero[0] = col;
            colocarReinas(tablero, 1);  // Empezamos desde la fila 1 (segunda)
            exit(0);
        }
    }

    // El proceso padre espera a que todos los hijos terminen
    while (wait(NULL) > 0);
    return 0;
}


/**
 *  En el caso del problema de las N reinas concurrente:
 *  - En cada paso se estan creando multiples procesos con fork (o similar múltiples hilos con pthread_create).
 *  - Cada proceso tiene su propia copia del contexto, lo que implica overhead.
 *  - Muchos procesos pueden competir por CPU si no hay suficientes núcleos.
 *  - Además, si las soluciones se exploran rápidamente (como pasa con N=8), el tiempo de vida útil de cada proceso 
 *    es pequeño, y el costo de crearlo se vuelve significativo comparado con su utilidad real.
 *  
 *  La Ley de Amdahl establece que:
 *  La mejora potencial del rendimiento mediante el uso de procesamiento paralelo está limitada por la fracción del 
 *  programa que debe ejecutarse secuencialmente.
 *  Se expresa comúnmente como:
 *      S = 1/((1−𝑃)+(𝑃/𝑁))
 *  P: proporción del programa que puede paralelizarse.
 *  N: número de procesadores o unidades de ejecución.
 *  1−P: proporción que debe ejecutarse secuencialmente.
 *  
 *  Aplicación al problema de las N reinas con procesos
 *  Aunque el problema parece altamente paralelizable, en la versión concurrente con procesos sucede lo siguiente:
 *  - Sobrecarga de creación de procesos: Cada vez que creas un nuevo proceso, el sistema operativo incurre en un 
 *    costo significativo (comparado con una simple llamada recursiva). Esto es especialmente evidente en niveles 
 *    profundos del árbol de búsqueda.
 *  - Pequeñas cargas de trabajo: Muchos procesos mueren muy pronto al detectar conflictos (colisiones entre reinas), 
 *    lo que hace que el trabajo realizado por cada proceso sea mínimo. La sobrecarga de crear el proceso supera los 
 *    beneficios de ejecutar esa pequeña parte en paralelo.
 *  - No se aprovecha todo el paralelismo: Solo los primeros niveles tienen 8 procesos (una por cada columna), pero 
 *    más abajo la paralelización no se escala igual de bien. Gran parte del trabajo sigue siendo secuencial o tiene 
 *    sincronización implícita (esperas con wait()), lo cual limita el speedup.
 *  - El código no reutiliza resultados parciales ni agrupa el trabajo eficientemente. No hay batching ni agrupamiento 
 *    inteligente para minimizar overhead.
 *  Por lo tanto Sí, la Ley de Amdahl explica por qué la versión concurrente con procesos no supera a la versión 
 *  secuencial recursiva:
 *  - Hay gran parte del tiempo consumido en tareas no paralelizables (crear procesos, esperar hijos).
 *  - El overhead de la paralelización supera el beneficio, especialmente cuando el trabajo es pequeño por proceso.
 *  - La parte verdaderamente paralelizable es menor en la práctica de lo que parece teóricamente.
 *  - Para este tipo de problema, usar hilos en lugar de procesos o usar técnicas como dividir el árbol de búsqueda en 
 *    tareas más gruesas y balanceadas puede ayudar a obtener un mejor rendimiento en paralelo.
 * 
 *  Resultado:
 *  - Para problemas pequeños como el de 8 reinas, la versión recursiva es más rápida, porque:
 *  - No hay costos de creación de procesos.
 *  - El compilador suele optimizar las llamadas recursivas.
 *  - El número total de exploraciones es manejable en un solo hilo.
 * 
 *  ¿Cuándo usar concurrencia?
 *  - Cuando el problema es grande.
 *  - Cuando las ramas a explorar son más gruesas y balanceadas.
 *  - Cuando tienes muchos núcleos disponibles y el costo de proceso/hilo se compensa con el paralelismo real.
 */