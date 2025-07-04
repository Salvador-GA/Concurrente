El problema del productor-consumidor describe un escenario donde uno o varios productores generan datos (o productos) y los colocan en un buffer compartido, mientras uno o varios consumidores retiran esos datos del buffer para procesarlos.
El desafío principal es asegurar que:

Los productores no escriban en el buffer si está lleno.

Los consumidores no lean del buffer si está vacío.

Ambos tipos de procesos accedan de forma segura al buffer compartido, sin interferencia ni condiciones de carrera.

Este problema modela muchos sistemas reales como colas de trabajos, servidores de impresión o buffers de red.