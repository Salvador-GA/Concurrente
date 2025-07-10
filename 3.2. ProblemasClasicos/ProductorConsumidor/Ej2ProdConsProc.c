/**
 * @file Ej2ProdConsProc.c
 * @brief Problema productor-consumidor usando procesos, memoria compartida y semáforos POSIX.
 *        Usa mmap para compartir el búfer y variables entre procesos.
 * @author Salvador Gonzalez Arellano
 * 
 * Para la memoria compartida usamos mmap, esta funcion necesita los siguientes parametros:
 *  void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 *  1. addr (void *)
 *      - Dirección sugerida donde iniciar el mapeo en la memoria del proceso.
 *      - Si se pasa NULL, el sistema elige automáticamente una dirección adecuada.
 *        Es lo más recomendado
 *  2. length (size_t)
 *      - Tamaño del área de memoria a mapear, en bytes.
 *      - Recomendable que sea múltiplo del tamaño de página (comúnmente 4096 bytes).
 *  3. prot (int)
 *      - Especifica las protecciones de la memoria mapeada. 
 *      - Se pueden combinar usando el operador OR (|):
 *          + PROT_READ Permite leer desde la región de memoria.
 *          + PROT_WRITE Permite escribir en la región de memoria.
 *          + PROT_EXEC Permite ejecutar código en la región (usualmente desaconsejado por 
 *                      seguridad).
 *          + PROT_NONE No se permite ningún acceso a la memoria (se puede usar para proteger 
 *                      temporalmente una región).
 *  4. flags (int)
 *      - Indica el comportamiento del mapeo. También se combinan con OR (|):
 *          + MAP_SHARED Los cambios hechos por un proceso son visibles por otros procesos que 
 *                      comparten la región.
 *          + MAP_PRIVATE Se crea una copia privada. Los cambios no se reflejan en el archivo 
 *                      ni en otros procesos.
 *          + MAP_ANONYMOUS (o MAP_ANON en algunos sistemas) El mapeo no está respaldado por 
 *                      un archivo; el contenido se inicializa en cero. En este caso, fd debe 
 *                      ser -1 y offset debe ser 0.
 *        Otros posibles (menos usados):
 *          + MAP_FIXED: fuerza a usar exactamente la dirección indicada en addr.
 *          + MAP_POPULATE: precarga páginas en memoria.
 *          + MAP_NORESERVE: no reserva espacio de swap para la región.
 *  5. fd (int)
 *      - Descriptor de archivo que se quiere mapear. 
 *      - Si se usa MAP_ANONYMOUS, se debe poner -1.
 *  6. offset (off_t)
 *      - Desplazamiento (en bytes) dentro del archivo desde donde comienza el mapeo.
 *      - Debe ser múltiplo del tamaño de página.
 *      - Si usas MAP_ANONYMOUS, se debe poner 0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#ifndef MAP_ANONYMOUS       // En algunos sistemas puede no estar definido asi
#define MAP_ANONYMOUS 0x20  // Valor común en Linux
#endif

#define TAM_BUFFER 5
#define PRODUCCIONES 20

int main() {
    // Crear búfer circular compartido
    int *buffer = mmap(NULL, sizeof(int) * TAM_BUFFER, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *insercion = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *extraccion = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Crear semáforos compartidos
    sem_t *espacios_libres = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *elementos_disponibles = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Inicializar semáforos y variables
    sem_init(espacios_libres, 1, TAM_BUFFER);
    sem_init(elementos_disponibles, 1, 0);
    sem_init(mutex, 1, 1);
    *insercion = *extraccion = 0;

    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo: Productor
        for (int i = 0; i < PRODUCCIONES; i++) {
            sem_wait(espacios_libres);
            sem_wait(mutex);
            buffer[*insercion] = i;
            sem_post(mutex);
            printf("Productor: insertó %d en posición %d\n", i, *insercion);
            *insercion = (*insercion + 1) % TAM_BUFFER;
            sem_post(elementos_disponibles);
            sleep(1);
        }
        exit(0);
    } else {
        // Proceso padre: Consumidor
        for (int i = 0; i < PRODUCCIONES; i++) {
            sem_wait(elementos_disponibles);
            sem_wait(mutex);
            int item = buffer[*extraccion];
            sem_post(mutex);
            printf("Consumidor: consumió %d de posición %d\n", item, *extraccion);
            *extraccion = (*extraccion + 1) % TAM_BUFFER;
            sem_post(espacios_libres);
            sleep(2);
        }

        wait(NULL);  // Espera al productor
        // Limpieza de semáforos y variables compartidas
        sem_destroy(espacios_libres);
        sem_destroy(elementos_disponibles);
        sem_destroy(mutex);
        munmap(buffer, sizeof(int) * TAM_BUFFER);
        munmap(insercion, sizeof(int));
        munmap(extraccion, sizeof(int));
        munmap(espacios_libres, sizeof(sem_t));
        munmap(elementos_disponibles, sizeof(sem_t));
        munmap(mutex, sizeof(sem_t));
    }

    return 0;
}
