/**
 * Funciones con parámetros por valor y referencia
 * Ejemplo 8
 */
#include <stdio.h>

/**
 * Función que calcula la suma de dos numeros
 * pasados por valor
 * 
 * @param a primer valor a sumar
 * @param b segundo valor a sumar
 * @return la suma de a y b
 */
int suma(int a, int b) {
    return a + b;
}

/**
 * Función que imprime un saludo
 */
void saluda() {
    printf("Hola desde una función!\n");
}

/**
 * Función que duplica el valor de un numero
 * pasado por referencia
 * 
 * @param numero apuntador al valor a duplicar
 */
void duplicar(int *numero) {
    *numero = *numero * 2;
    printf("Valor duplicado dentro de la función: %d\n", *numero);
}

int main() {
    printf("Suma: %d\n", suma(3, 4));
    saluda();
    int valor = 5;
    duplicar(&valor);
    printf("Valor después de llamar a duplicar: %d\n", valor);
    return 0;
}