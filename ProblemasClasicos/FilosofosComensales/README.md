## 🍽️ Problema de los Filósofos Comensales

El problema de los **filósofos comensales** modela a cinco filósofos sentados alrededor de una mesa circular. Cada filósofo alterna entre pensar y comer. Para comer, un filósofo necesita dos tenedores: el de su izquierda y el de su derecha. Sin embargo, entre cada dos filósofos hay un solo tenedor.

### Objetivos:
- Evitar el **interbloqueo** (deadlock), en el que todos los filósofos toman un tenedor y esperan el otro indefinidamente.
- Evitar la **inanición** (starvation), en la que uno o más filósofos nunca logran comer.
- Coordinar el acceso concurrente a los recursos (tenedores) de forma segura.

### Aplicaciones comunes:
- Acceso a recursos limitados en sistemas distribuidos
- Planificación de tareas con múltiples dependencias
