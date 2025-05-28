/**
 * Uso básico de apuntadores
 * Ejemplo 9
 */
#include <stdio.h>

int main() {
    int x = 10;
    int *ptr = &x;
    printf("Valor: %d, Direccion: %p\n", *ptr, ptr);
    *ptr = 20;
    printf("Nuevo valor de x: %d\n", x);
    return 0;
}