/*
Fuente del cdgo
https://www.geeksforgeeks.org/dsa/quick-sort-algorithm/
Se integró con "sort_algorithms.hpp", se eliminó la función main() original y se agregó la función envolvente runQuickSort() que valida si el arreglo está vacío antes de ordenar.
 * Autor:
*/

#include "sort_algorithms.hpp"
#include <vector>

using namespace std;

int partition(vector<int> &arr, int low, int high) {
  int pivot = arr[high];
  int i = low - 1;

  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivot) {
      i++;
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quickSort(vector<int> &arr, int low, int high) {

  if (low < high) {

    int pi = partition(arr, low, high);

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void runQuickSort(vector<int> &arr) {
  if (!arr.empty()) {
    quickSort(arr, 0, arr.size() - 1);
  }
}