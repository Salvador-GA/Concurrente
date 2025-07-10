/**
 * @file Ej5Archivos.c
 * @brief Cada hilo lee un archivo de texto y cuenta cuántas líneas contiene.
 * Los resultados se recogen al final para sumar el total.
 * @author Salvador Gonzalez Arellano
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_ARCHIVOS 3

/**
 * @struct ArchivoParam
 * @brief Estructura para pasar el nombre del archivo al hilo.
 */
typedef struct {
    char nombre[100];  ///< Nombre del archivo a procesar
} ArchivoParam;

/**
 * @brief Función que cuenta las líneas de un archivo.
 * @param arg Puntero a ArchivoParam con el nombre del archivo.
 * @return Puntero a entero que representa el número de líneas (debe liberarse).
 */
void *contar_lineas(void *arg) {
    ArchivoParam *param = (ArchivoParam *)arg;
    FILE *archivo = fopen(param->nombre, "r");
    int *contador = malloc(sizeof(int));
    *contador = 0;

    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return contador;  // Retorna 0 si falla
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        (*contador)++;
    }

    fclose(archivo);
    printf("Archivo '%s': %d líneas\n", param->nombre, *contador);
    return contador;
}

/**
 * @brief Función principal que lanza hilos para leer archivos y suma el total de líneas.
 * @return int Código de salida.
 */
int main() {
    const char *nombres_archivos[NUM_ARCHIVOS] = {"file1.txt", "file2.txt", "file3.txt"};
    pthread_t hilos[NUM_ARCHIVOS];
    ArchivoParam params[NUM_ARCHIVOS];

    for (int i = 0; i < NUM_ARCHIVOS; i++) {
        strncpy(params[i].nombre, nombres_archivos[i], sizeof(params[i].nombre));
        pthread_create(&hilos[i], NULL, contar_lineas, &params[i]);
    }

    int total_lineas = 0;
    for (int i = 0; i < NUM_ARCHIVOS; i++) {
        int *resultado;
        pthread_join(hilos[i], (void **)&resultado);
        total_lineas += *resultado;
        free(resultado);
    }

    printf("Total de líneas en todos los archivos: %d\n", total_lineas);
    return 0;
}
