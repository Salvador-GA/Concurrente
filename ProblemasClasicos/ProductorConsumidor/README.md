## 🧺 Problema del Productor-Consumidor

El problema del **productor-consumidor** describe una situación en la que uno o varios *productores* generan elementos y los insertan en un *buffer compartido*, mientras uno o varios *consumidores* retiran elementos del buffer para procesarlos.

### Objetivos:
- Evitar que los productores escriban en el buffer cuando está lleno.
- Evitar que los consumidores lean del buffer cuando está vacío.
- Proteger el acceso concurrente al buffer mediante mecanismos de sincronización.

### Aplicaciones comunes:
- Buffers de red
- Colas de impresión
- Tareas en segundo plano (background jobs)
