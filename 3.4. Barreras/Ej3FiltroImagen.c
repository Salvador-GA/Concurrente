/**
 * @file Ej3FiltroImagen.c
 * @brief Ejemplo para aplicar un fiultro auna imagen usando hilos POSIX y sincronización con barrera
 * @author Salvador Gonzalez Arellano
 *
 * Carga una imagen RGB (JPG o PNG) usando stb_image.h.
 * Aplica un filtro promedio 3x3 por canal (R, G, B) usando múltiples hilos.
 * Sincroniza las fases usando pthread_barrier_t.
 * Guarda el resultado como imagen PNG usando stb_image_write.h.
 * 
 * Para compilar el programa: 
 *      gcc -o Ej3FiltroImagen Ej3FiltroImagen.c -lpthread -lm
 * Para ejecutarlo:
 *      ./Ej3FiltroImagen entrada.jpg salida.jpg 10
 *          - entrada.jpg, imagend e entrada, debe existir
 *          - salida.jpg nombre del archivo de salida (puede o no existir)
 *          - 10, numero de iteraciones del filtro
 */

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

/**
 * #include <...> — Para bibliotecas del sistema
 *  Le dice al compilador: "Busca este archivo solo en las rutas estándar del sistema"
 * #include "..." — Para archivos locales o personalizados
 *  Le dice al compilador: "Primero busca este archivo en el directorio actual, y si 
 *  no lo encuentra, usa las rutas estándar".
 */
#include "stb_image.h"          // Biblioteca para cargar la imagen
#include "stb_image_write.h"    // Biblioteca para escribir la imagen
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4 // Numero de hilos utilizados

// Variables globales para la imagen
unsigned char *imagen;          // Imagen original cargada
unsigned char *imagen_nueva;    // Imagen temporal donde se escribe el resultado

int ancho, alto, canales;       // Dimensiones y canales de la imagen

/**
 * Este valor indica cuantas veces se aplica el filtro promedio sobre la imagen.
 * Mayor numero de iteraciones ⇒ mayor desenfoque.
 * Cada pasada suaviza un poco mas los bordes y transiciones de color.
 * Aumentar este valor es la forma mas directa de intensificar el filtro.
 */
int iteraciones;

pthread_barrier_t barrera;      // Barrera para sincronización entre hilos

/**
 * @brief Aplica un filtro promedio 3x3 a un píxel RGB en la posición (i, j)
 * 
 * Se calcula por separado para cada canal (Rojo, Verde, Azul).
 * Se aplica sobre una vecindad de 3x3 (el píxel actual y sus vecinos inmediatos). 
 * Si quisieras un filtro más grande, podrías cambiar los límites a -2 a 2 para hacer un filtro de 5x5, o -3 a 3 para 7x7. 
 * Mayor vecindad ⇒ desenfoque más fuerte por iteracion. 
 * Aumenta el costo computacional.
 * 
 * @param i Fila del píxel
 * @param j Columna del píxel
 * @param canal Canal (0: R, 1: G, 2: B)
 * @return unsigned char Valor promedio del canal
 */
unsigned char aplicar_filtro(int i, int j, int canal) {
    int suma = 0;
    int conteo = 0;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int ni = i + di;
            int nj = j + dj;
            if (ni >= 0 && ni < alto && nj >= 0 && nj < ancho) {
                int indice = (ni * ancho + nj) * canales + canal;
                suma += imagen[indice];
                conteo++;
            }
        }
    }
    return (unsigned char)(suma / conteo);
}

/**
 * @brief Función que ejecuta cada hilo para aplicar el filtro a una región de la imagen.
 * 
 * Divide la imagen por bloques horizontales.
 * 
 * @param arg Puntero al ID del hilo
 * @return NULL 
 */
void* hilo_filtro(void* arg) {
    int id = *(int*)arg;
    int filas_por_hilo = alto / NUM_HILOS;
    int inicio = id * filas_por_hilo;
    int fin = (id == NUM_HILOS - 1) ? alto : inicio + filas_por_hilo;

    for (int iter = 0; iter < iteraciones; iter++) {
        for (int i = inicio; i < fin; i++) {
            for (int j = 0; j < ancho; j++) {
                for (int c = 0; c < 3; c++) { // Solo R, G, B (ignora canal 4 si hay)
                    int indice = (i * ancho + j) * canales + c;
                    imagen_nueva[indice] = aplicar_filtro(i, j, c);
                }
            }
        }

        // Sincronización: esperar a que todos terminen de escribir
        pthread_barrier_wait(&barrera);

        // Copiar imagen_nueva → imagen para la siguiente iteración
        for (int i = inicio; i < fin; i++) {
            for (int j = 0; j < ancho; j++) {
                for (int c = 0; c < 3; c++) {
                    int indice = (i * ancho + j) * canales + c;
                    imagen[indice] = imagen_nueva[indice];
                }
            }
        }

        pthread_barrier_wait(&barrera); // Esperar antes de siguiente iteración
    }

    return NULL;
}

/**
 * @brief Función principal.
 * 
 * Carga la imagen de entrada, lanza los hilos para procesarla y guarda el resultado.
 * 
 * @param argc Número de argumentos
 * @param argv Argumentos de línea de comandos: [entrada] [salida]
 * @return int Código de salida
 */
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s entrada.jpg salida.png 5\n", argv[0]);
        return 1;
    }

    iteraciones = (int) *argv[3];

    // Cargar imagen forzando a RGB (3 canales) al final del archivo una explicacion detallada
    imagen = stbi_load(argv[1], &ancho, &alto, &canales, 3);
    if (!imagen) {
        fprintf(stderr, "Error al cargar la imagen: %s\n", argv[1]);
        return 1;
    }

    canales = 3; // Aseguramos que trabajamos con RGB
    imagen_nueva = malloc(ancho * alto * canales);
    if (!imagen_nueva) {
        fprintf(stderr, "Error al asignar memoria para imagen nueva.\n");
        stbi_image_free(imagen);
        return 1;
    }

    pthread_barrier_init(&barrera, NULL, NUM_HILOS);

    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, hilo_filtro, &ids[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_barrier_destroy(&barrera);

    // Guardar imagen resultante
    if (!stbi_write_png(argv[2], ancho, alto, canales, imagen, ancho * canales)) {
        fprintf(stderr, "Error al guardar la imagen.\n");
    } else {
        printf("Imagen guardada en %s\n", argv[2]);
    }

    free(imagen_nueva);
    stbi_image_free(imagen);

    return 0;
}

/**
 * 1. ¿Qué devuelve imagen = stbi_load(argv[1], &ancho, &alto, &canales, 3);?
 *      Esto carga la imagen en un arreglo 1D lineal de bytes (unsigned char), donde cada píxel está representado por 
 *      canales valores consecutivos.
 *      Si usas canales = 3 (RGB), entonces:
 *          - Cada píxel usa 3 bytes: Rojo, Verde, Azul (en ese orden).
 *          - El arreglo tiene ancho × alto × 3 bytes.
 * 2. ¿Qué significan las opciones de canales?
 *      desired_channels	Resultado	            Significado de cada canal
 *      1	                Escala de grises	    [L] (luminosidad, 0–255)
 *      2	                Grises + transparencia	[L, A]
 *      3	                Color RGB	            [R, G, B]
 *      4	                Color RGBA	            [R, G, B, A] (con canal alfa de opacidad)
 * 3. ¿Qué pasa si NO forzas desired_channels (pasa 0)?
 *      La función cargará la imagen tal como está en disco, y almacenará el valor en *channels_in_file.
 *      Por ejemplo:
 *          - Si la imagen es JPG → probablemente tenga 3 canales (RGB).
 *          - Si la imagen es PNG con transparencia → probablemente tenga 4 canales (RGBA).
 *          - Si es una imagen en blanco y negro → puede tener solo 1 canal.
 * 4. ¿Qué pasa si fuerzas los canales?
 *      Si pasas un valor en desired_channels, stb_image hará una conversión automática:
 *          - Si fuerzas a 3, pero la imagen es en grises → duplica el canal en R, G y B.
 *          - Si fuerzas a 1, pero la imagen es RGB → convierte a escala de grises (luminosidad).
 *          - Si fuerzas a 4 → agrega canal alfa (1.0 si no existía).
 *      No pierdes la imagen Solo cambia su representación en memoria.
 * 5. ¿Cómo se accede a un píxel (i, j)?
 *      Supón que estás en la fila i y columna j, con 3 canales por píxel:
 *          int index = (i * ancho + j) * canales;
 *      Luego puedes acceder así:
 *          Canal	Cálculo	            Valor almacenado
 *          Rojo	imagen[index + 0]	Intensidad del rojo (0–255)
 *          Verde	imagen[index + 1]	Intensidad del verde
 *          Azul	imagen[index + 2]	Intensidad del azul
 * 6. Ejemplo concreto
 *      Supón una imagen de 4×2 píxeles (ancho = 4, alto = 2), RGB.
 *      Entonces:
 *          - Total de píxeles = 4 × 2 = 8
 *          - Total de bytes = 8 × 3 = 24
 *      El arreglo imagen tendrá 24 posiciones:
 *          - imagen[0] a imagen[23]
 *      Visualmente se vería así:
 *          (0,0): imagen[0], imagen[1], imagen[2]         → R, G, B
 *          (0,1): imagen[3], imagen[4], imagen[5]
 *          (0,2): imagen[6], imagen[7], imagen[8]
 *          (0,3): imagen[9], imagen[10], imagen[11]
 *          (1,0): imagen[12], imagen[13], imagen[14]
 *          (1,1): imagen[15], imagen[16], imagen[17]
 *          (1,2): imagen[18], imagen[19], imagen[20]
 *          (1,3): imagen[21], imagen[22], imagen[23]
 */
