/**
 * Declaración de variables de distintos tipos
 * Ejemplo 3
 */
#include <stdio.h>

int main() {
    int entero = 10;
    float decimal = 3.14;
    char letra = 'A';
    double grande = 2.718281828;

    printf("Entero: %d\n", entero);
    printf("Decimal: %.2f\n", decimal); // Imprime 2 decimales
    printf("Letra: %c\n", letra);
    printf("Doble: %.6f\n", grande);    // Imprime 6 decimales

    return 0;
}