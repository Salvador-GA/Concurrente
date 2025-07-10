## 📈 Comparación de rendimiento y la Ley de Amdahl

Este conjunto de programas permite ilustrar un principio clave en la programación concurrente: **la Ley de Amdahl**. Esta ley establece que la mejora máxima que se puede obtener al paralelizar un programa está limitada por la fracción secuencial del mismo. Incluso si se usan muchos procesos o hilos, hay partes del código que no pueden ser paralelizadas, lo que limita la ganancia total de rendimiento.

En estos ejemplos, aunque la versión concurrente explora ramas del árbol en paralelo, el costo de crear procesos o sincronizar hilos puede superar los beneficios, especialmente para tamaños pequeños de N. De hecho, en varios casos, la versión secuencial es más rápida debido a la ausencia de sobrecarga de creación de procesos o hilos. Por ello, **no siempre más concurrencia significa más velocidad**.

La clave está en identificar:

- Qué partes del algoritmo pueden paralelizarse.
- Cuánto tiempo se invierte en la sincronización o creación de procesos/hilos.
- Cuál es el tamaño del problema y la capacidad del hardware (número de núcleos, memoria, etc.).

Estos programas ayudan a reflexionar sobre **cuándo conviene aplicar concurrencia y cuándo no**, en función del costo-beneficio real de cada enfoque.

---

## 🧪 Medición de tiempo

Puedes medir el tiempo de ejecución usando la utilidad `time` en la terminal:

```bash
time ./nReinas
time ./nReinasConcurrente
time ./nReinasHilos
```

## 📚 Aprendizajes clave
- Comparación entre ejecución secuencial y concurrente.
- Análisis práctico de la Ley de Amdahl y su impacto en la programación paralela.
- Importancia de analizar la sobrecarga de concurrencia antes de implementarla.
