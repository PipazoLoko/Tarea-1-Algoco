/*
Fuente del código:
https://www.geeksforgeeks.org/dsa/patience-sorting/
Se adaptó la lógica para integrarla con "sort_algorithms.hpp", se reemplazó el paso por valor por referencias en los parámetros y se eliminó la función main() original.
*/

#include "sort_algorithms.hpp"
#include <bits/stdc++.h>

using namespace std;

vector<int> merge_piles(vector<vector<int>> &v) {

  vector<int> ans;

  while (1) {

    int minu = INT_MAX;
    int index = -1;
    for (size_t i = 0; i < v.size(); i++) {
      if (minu > v[i][v[i].size() - 1]) {
        minu = v[i][v[i].size() - 1];
        index = i;
      }
    }

    ans.push_back(minu);
    v[index].pop_back();
    if (v[index].empty()) {
      v.erase(v.begin() + index);
    }

    if (v.size() == 0)
      break;
  }
  return ans;
}

void patienceSorting(vector<int> &arr) {
  vector<vector<int>> piles;
  for (size_t i = 0; i < arr.size(); i++) {
    if (piles.empty()) {
      vector<int> temp;
      temp.push_back(arr[i]);
      piles.push_back(temp);
    } else {
      int flag = 1;
      for (size_t j = 0; j < piles.size(); j++) {
        if (arr[i] < piles[j][piles[j].size() - 1]) {
          piles[j].push_back(arr[i]);
          flag = 0;
          break;
        }
      }
      if (flag) {
        vector<int> temp;
        temp.push_back(arr[i]);
        piles.push_back(temp);
      }
    }
  }

  vector<int> ans;
  ans = merge_piles(piles);
  arr = ans;
}

void runPatienceSort(vector<int> &arr) { patienceSorting(arr); }