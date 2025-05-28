/**
 * Operaciones básicas con enteros
 * Aritméticos: +, -, *, /, %
 * Relacionales: ==, !=, <, >, <=, >=
 * Lógicos: &&, ||, !
 * En C, cualquier valor distinto de cero se considera verdadero (true) 
 * en una expresión lógica, mientras que el valor 0 se considera falso (false).
 * Ejemplo 4
 */
#include <stdio.h>

int main() {
    int a = 5, b = 3;
    printf("Suma: %d\n", a + b);
    printf("Es mayor: %d\n", a > b);
    printf("Ambos son positivos: %d\n", a > 0 && b > 0);
    return 0;
}