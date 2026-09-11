/*
Archivo de cabecera que define las interfaces o wrappers de los algoritmos.
Encapsula las funciones auxiliares internas y expone un punto de acceso para el programa principal: sorting.cpp
*/

#ifndef SORT_ALGORITHMS_HPP
#define SORT_ALGORITHMS_HPP

#include <vector>

void runMergeSort(std::vector<int> &arr);
void runQuickSort(std::vector<int> &arr);
void runPatienceSort(std::vector<int> &arr);
void runStdSort(std::vector<int> &arr);

#endif