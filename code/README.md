# Tarea 1 - Algoritmos y Complejidad: Análisis Experimental

Felipe Luengo Bascuñán (Rol: 202473509-8)

Este directorio contiene el código fuente, los scripts de automatización y los datos para el análisis de complejidad teórica versus el tiempo de ejecución en la práctica de algoritmos.

## Estructura del Código

Está dividido en dos grandes partes, cada uno con su propio entorno y generación de datos:

1. **`sorting/`**: Implementación y medición de algoritmos de ordenamiento (MergeSort, QuickSort, PatienceSort y `std::sort`).
2. **`matrix_multiplication/`**: Implementación y medición de algoritmos de multiplicación de matrices (Naive y Strassen).

## Dependencias

Se utilizó, se recomienda, el siguiente entorno:
* **Sistema Operativo:** Fedora 44
* **Compilador:** `g++` GCC versión 16.2.1 o superior con soporte C++11/C++14.
* **Intérprete:** Python 3.
* **Librerías de Python:** `pandas`, `matplotlib`, `seaborn`

## Instrucciones de Ejecución

Cada módulo está automatizado a través de su respectivo `makefile`. Para ejecutar un módulo, navegue hacia su directorio correspondiente a través de la terminal:

```bash
cd sorting/
# o bien
cd matrix_multiplication/
```
Una vez ahí ejecutar `make all`