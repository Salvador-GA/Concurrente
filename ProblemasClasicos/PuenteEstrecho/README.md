## 🌉 Problema del Puente Estrecho

El problema del **puente estrecho** modela una situación en la que vehículos provenientes de dos direcciones opuestas (lado A y lado B) desean cruzar un puente angosto que **solo permite el paso en un sentido a la vez**. Si vehículos de ambos lados intentan cruzar simultáneamente, puede ocurrir un choque o un interbloqueo.

### Objetivos

- Evitar que vehículos de direcciones opuestas entren al puente al mismo tiempo.
- Prevenir **interbloqueos**, donde vehículos de ambos lados se esperan mutuamente sin poder avanzar.
- Minimizar la **inanición**, asegurando que cada lado tenga oportunidad de cruzar.
- Coordinar el paso de los vehículos de manera **justa y ordenada**, incluso en presencia de alta concurrencia.

### Variantes comunes

- **Control de paso por turnos**: permitir el paso a un número fijo de vehículos de un lado antes de ceder el turno.
- **Políticas FIFO**: mantener una cola de llegada para respetar el orden de los vehículos.
- **Prioridad alternada**: cuando hay espera en ambos lados, se alterna el paso para evitar inanición.

### Aplicaciones reales

- Puentes o túneles de un solo carril en tráfico real.
- Acceso a enlaces de red compartidos con transmisión bidireccional.
- Coordinación de acceso a secciones críticas en sistemas de múltiples productores.
