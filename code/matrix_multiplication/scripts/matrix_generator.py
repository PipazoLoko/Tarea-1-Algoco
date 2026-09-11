import numpy as np
import os
import random
from itertools import product

def generar_matriz(n, tipo, dominio):
    """
    Genera una matriz de tamaño n x n según el tipo y dominio especificados.
    - tipo: 'dispersa', 'diagonal', 'densa'
    - dominio: 'D0' (valores en {0,1}) o 'D10' (valores en {0..9})
    """
    if dominio == 'D0':
        valores = [0, 1]
    elif dominio == 'D10':
        valores = list(range(10))
    else:
        raise ValueError("Dominio no válido. Usa 'D0' o 'D10'.")

    if tipo == 'densa':
        matriz = np.random.choice(valores, size=(n, n))
    elif tipo == 'diagonal':
        matriz = np.zeros((n, n), dtype=int)
        diag_vals = np.random.choice(valores, size=n)
        np.fill_diagonal(matriz, diag_vals)
    elif tipo == 'dispersa':
        matriz = np.zeros((n, n), dtype=int)
        cantidad = max(1, (n * n) // 10)  # solo 10% elementos diferentes de 0
        for _ in range(cantidad):
            i = random.randint(0, n-1)
            j = random.randint(0, n-1)
            matriz[i, j] = random.choice([v for v in valores if v != 0])
    else:
        raise ValueError("Tipo no válido. Usa 'densa', 'diagonal' o 'dispersa'.")
    
    return matriz

def guardar_matriz(matriz, nombre_archivo):
    """
    Guarda una matriz en un archivo de texto.
    """
    with open(nombre_archivo, 'w') as f:
        for fila in matriz:
            f.write(' '.join(map(str, fila)) + '\n')

def generar_y_guardar(n, t, d, m, carpeta="../data/matrix_input"):
    """
    Genera dos matrices y las guarda en archivos con nombres formateados.
    """

    M1 = generar_matriz(n, t, d)
    M2 = generar_matriz(n, t, d)

    base = f"{n}_{t}_{d}_{m}"
    archivo1 = os.path.join(carpeta, f"{base}_1.txt")
    archivo2 = os.path.join(carpeta, f"{base}_2.txt")

    guardar_matriz(M1, archivo1)
    guardar_matriz(M2, archivo2)

    print(f"Archivos guardados: {archivo1}, {archivo2}")

def generar_todos():
    Ns = [2**4, 2**6, 2**8, 2**10]
    Ts = ["dispersa", "diagonal", "densa"]
    Ds = ["D0", "D10"]
    Ms = ["a", "b", "c"]

    total = len(Ns) * len(Ts) * len(Ds) * len(Ms)
    print(f"Generando {total * 2} archivos de matrices...")

    for n, t, d, m in product(Ns, Ts, Ds, Ms):
        generar_y_guardar(n, t, d, m)

    print("Todas las matrices han sido generadas.")

if __name__ == "__main__":
    generar_todos()