/**
 * Ejemplo de if-else y switch-case
 * Ejemplo 5
 */
#include <stdio.h>

int main() {
    int entero = 6;
    if (entero > 5) {
        printf("Mayor que 5\n");
    } else {
        printf("Menor o igual a 5\n");
    }

    int opcion = 2;
    switch(opcion) {
        case 1:
            printf("Elegiste 1\n");
            break;
        case 2:
            printf("Elegiste 2\n");
            break;
        default:
            printf("Opción no válida\n");
            break;
    }
    return 0;
}