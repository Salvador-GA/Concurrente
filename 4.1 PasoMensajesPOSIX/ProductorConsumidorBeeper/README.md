# 📨 Productor–Consumidor con Beeper (POSIX Message Queues)

Este programa implementa una versión del problema clásico **Productor-Consumidor**, pero con **dos tipos de tareas** y un mecanismo de sincronización basado en un **buzón central llamado `/beeper`**, utilizando **colas de mensajes POSIX**.

Cuando existen múltiples tipos de tareas (por ejemplo, `/buzon1` y `/buzon2`), si un consumidor se bloquea esperando en una cola que no recibe mensajes, **se desperdicia eficiencia**.

Para evitar este problema, se utiliza un buzón central llamado **`/beeper`**, que sirve como intermediario. El productor:

1. Envía la tarea a `/buzon1` o `/buzon2`.
2. Notifica en `/beeper` cuál buzón recibió la tarea.

Los consumidores:

- Esperan bloqueados en `/beeper`, no directamente en los buzones.
- Cuando llega una notificación, leen su contenido (por ejemplo, "`/buzon2`").
- Entonces leen la tarea correspondiente del buzón indicado.

Este enfoque **evita que los consumidores se bloqueen en buzones vacíos** mientras hay trabajo en otros.

---

## 🗂 Archivos del proyecto

- `productor.c`: Envia tareas a `/buzon1` o `/buzon2` y notifica en `/beeper`.
- `consumidor.c`: Espera mensajes en `/beeper` y redirige el procesamiento al buzón adecuado.

---

## 🛠️ Compilación

Desde la terminal, ejecuta:

```bash
gcc productor.c -o productor -lrt
gcc consumidor.c -o consumidor -lrt
```
---

## 🛠️ Ejecución

En 3 terminales, ejecuta:

```bash
./productor
./consumidor consumidor1
./consumidor consumidor2
```
