# Ejemplos de Colas POSIX No Bloqueantes

Este repositorio contiene ejemplos de programas escritos en C que utilizan colas de mensajes POSIX con un enfoque en la **comunicación no bloqueante** entre procesos. Se presentan dos ejemplos principales:

## 📦 Ejemplo 1: Emisor y Receptor (modo no bloqueante)

- `emisor.c`: Envía un mensaje a una cola POSIX.
- `receptor.c`: Intenta recibir un mensaje sin bloquearse, utilizando la bandera `O_NONBLOCK`. Si no hay mensajes, continúa realizando otras tareas o esperando activamente.

Este ejemplo simula un entorno donde el receptor **no puede quedarse bloqueado esperando** mensajes, útil en situaciones donde se desea mantener la capacidad de respuesta.

## ⚙️ Ejemplo 2: Productor y Consumidor con colas múltiples

- `productor.c`: Envía mensajes a dos colas diferentes: `/urgentes` y `/normales`, simulando tareas con distintas prioridades.
- `consumidor.c`: Intenta recibir de ambas colas sin bloquearse, alternando entre ellas. Si no hay mensajes disponibles, puede realizar otras tareas en lugar de quedar esperando pasivamente.

Este segundo ejemplo **resuelve el problema del bloqueo selectivo**, donde un consumidor no queda atado a una sola cola que podría estar vacía.

En este ejemplo ambos procesos estan en un ciclo infinito, para terminarlos puede presionar ctr+c en ambas terminales.

---

## 🔧 Compilación

Usa `gcc` para compilar cada archivo individualmente:

```bash
gcc emisor.c -o emisor -lrt
gcc receptor.c -o receptor -lrt
gcc productor.c -o productor -lrt
gcc consumidor.c -o consumidor -lrt
```

> ⚠️ Es necesario enlazar con `-lrt` porque las colas POSIX residen en la librería de tiempo real.

---

## ▶️ Ejecución

### Ejemplo 1

Primero ejecuta el receptor en una terminal:

```bash
./receptor
```

Luego, en otra terminal, ejecuta el emisor:

```bash
./emisor
```

### Ejemplo 2

En una terminal para el consumidor:

```bash
./consumidor 
```

Y en otra terminal, ejecuta el productor:

```bash
./productor
```

> El productor genera tareas aleatoriamente para `/urgentes` o `/normales`. El consumidor intenta atender ambas sin bloquearse, dando prioridad a las tareas urgentes primero.

---

## 🧹 Limpieza

Si alguna cola no se elimina automáticamente (como en el ejemplo del productor consumidor), puedes eliminarlas manualmente con:

```bash
mq_unlink("/buzon")
mq_unlink("/urgentes")
mq_unlink("/normales")
```

O desde un programa en C o script que use `mq_unlink`.
