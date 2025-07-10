/**
 * Ejemplo de ciclos for y while
 * Ejemplo 6
 */
#include <stdio.h>

int main() {
    for (int i = 0; i < 5; i++) {
        printf("For: %d\n", i);
    }

    int i = 0;
    while (i < 5) {
        printf("While: %d\n", i);
        i++;
    }

    i=0;
    do {
        printf("Do While: %d\n", i);
        i++;
    } while(i<5);
    return 0;
}