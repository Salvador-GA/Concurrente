## 📚 Problema de Lectores y Escritores
El problema de los lectores y escritores es un problema clásico de sincronización en sistemas concurrentes. Modela una situación en la que múltiples procesos (lectores y escritores) desean acceder simultáneamente a un recurso compartido (por ejemplo, una base de datos o archivo). Los lectores solo necesitan leer el recurso (sin modificarlo), mientras que los escritores requieren acceso exclusivo para modificarlo.

## Objetivos
- Permitir que múltiples lectores accedan al recurso compartido al mismo tiempo cuando no haya escritores.
- Garantizar que los escritores tengan acceso exclusivo al recurso, bloqueando tanto a otros escritores como a los lectores.
- Evitar condiciones de carrera, asegurando la integridad de los datos.
- Prevenir la inanición, asegurando que tanto lectores como escritores eventualmente obtengan acceso al recurso.
- Coordinar el acceso de forma eficiente en escenarios de alta concurrencia.

## Variantes comunes
- **Prioridad a lectores**: los lectores que llegan mientras hay otros leyendo pueden continuar, lo que puede hacer que los escritores esperen indefinidamente.
- **Prioridad a escritores**: un escritor en espera bloquea la entrada de nuevos lectores, garantizando que obtenga acceso pronto. ESTA ES LA VARIANTE PROGRAMADA EN ESTE EJEMPLO
- **Acceso justo**: alterna el turno entre lectores y escritores, evitando la inanición para ambos grupos.

## Aplicaciones reales
- Bases de datos donde múltiples usuarios leen información, pero solo unos pocos actualizan registros.
- Sistemas de archivos compartidos por múltiples procesos.
- Memorias cacheadas donde es frecuente la lectura, pero ocasionalmente se requiere actualización.
- Registros de monitoreo de sensores, donde se recolectan datos (escritura) y se consultan (lectura) constantemente.
