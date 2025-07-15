# 🪒 Problema del Barbero Dormilón

Este proyecto implementa una solución al clásico problema de sincronización conocido como **El barbero dormilón**, usando **hilos POSIX (`pthread`) y semáforos (`sem_t`)** en lenguaje C.

## Descripción

En una barbería hay un barbero, una silla de barbero y varias sillas de espera. Cuando no hay clientes, el barbero duerme. Cuando un cliente llega:

- Si el barbero está dormido, lo despierta.
- Si el barbero está ocupado y hay sillas disponibles, espera.
- Si no hay sillas disponibles, el cliente se va.

El objetivo es sincronizar correctamente la interacción entre **el barbero y múltiples clientes concurrentes**, evitando condiciones de carrera y asegurando que el barbero duerma solo cuando debe.

## Objetivos de aprendizaje

- Comprender la sincronización de múltiples hilos con semáforos.
- Modelar correctamente escenarios con recursos limitados y espera activa/pasiva.
- Implementar una solución sin interbloqueos ni inanición.
