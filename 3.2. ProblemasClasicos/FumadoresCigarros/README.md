# 🚬 Problema de los Fumadores

Este proyecto implementa una solución al problema de concurrencia conocido como **El problema de los fumadores**, usando **hilos POSIX (`pthread`) y semáforos (`sem_t`)** en C.

## Descripción

Hay tres fumadores y un agente. Cada fumador tiene un suministro infinito de uno de los tres ingredientes necesarios para armar un cigarro:

- Fósforos
- Papel
- Tabaco

El agente coloca dos ingredientes sobre la mesa. El fumador que tiene el tercer ingrediente toma los ingredientes, arma un cigarro, fuma y notifica al agente para que coloque más.

El reto está en **sincronizar la selección del fumador correcto sin que haya conflictos**, condiciones de carrera o interbloqueo.

## Objetivos de aprendizaje

- Modelar exclusión mutua y coordinación entre múltiples hilos.
- Usar correctamente semáforos para espera condicional.
- Implementar una solución robusta con posibilidad de mejora (evitar interbloqueo, `sem_trywait`, etc.).

## Archivos disponibles

- `Ej1FumadoresCigarrosI1.c`: Versión con posibilidad de interbloqueo.
- `Ej2FumadoresCigarros.c`: Versión mejorada usando `sem_trywait()` para evitar bloqueos.
