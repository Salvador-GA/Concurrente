/**
 * Solicita la edad al usuario y la imprime
 * Ejemplo 2
 */
#include <stdio.h>

int main() {
    int edad;  // Variable para almacenar la edad del usuario
    printf("Ingresa tu edad: ");
    scanf("%d", &edad);  // Lee un entero desde la entrada estándar
    printf("Entiendo tienes %d años.\n", edad);
    return 0;
}