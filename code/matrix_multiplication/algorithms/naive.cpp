/*
Fuente del código:
https://github.com/Dev-XYS/Algorithms/blob/master/Matrix-Multiplication(Naive).cpp
Se reemplazaron los arreglos estáticos globales por vectores bidimensionales pasados por referencia, y se eliminó la función main() original para encapsular la lógica en runNaive().

*/

#include <vector>

void runNaive(const std::vector<std::vector<int>> &A,
              const std::vector<std::vector<int>> &B,
              std::vector<std::vector<int>> &C) {
  int m = A.size();
  int p = A[0].size();
  int n = B[0].size();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      C[i][j] = 0;
      for (int k = 0; k < p; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}