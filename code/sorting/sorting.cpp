#include "algorithms/sort_algorithms.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

long getMemoryUsageKB() {
  std::ifstream file("/proc/self/status");
  std::string line;
  while (std::getline(file, line)) {
    if (line.compare(0, 6, "VmRSS:") == 0) {
      long mem = 0;
      std::string unit;
      sscanf(line.c_str(), "VmRSS: %ld %s", &mem, &unit[0]);
      return mem;
    }
  }
  return 0;
}

std::vector<int> loadArrayFromFile(const std::string &filepath) {
  std::vector<int> arr;
  std::ifstream file(filepath);
  int num;
  while (file >> num) {
    arr.push_back(num);
  }
  return arr;
}

void saveArrayToFile(const std::vector<int> &arr, const std::string &filepath) {
  std::ofstream file(filepath);
  for (size_t i = 0; i < arr.size(); i++) {
    file << arr[i] << (i == arr.size() - 1 ? "" : " ");
  }
}

void measureAlgorithm(
    const std::string &alg_name, void (*sort_func)(std::vector<int> &),
    std::vector<int> arr,
    std::ofstream &csv_file, const std::string &n, const std::string &t,
    const std::string &d, const std::string &m) {

  long mem_before = getMemoryUsageKB();
  auto start_time = std::chrono::high_resolution_clock::now();

  sort_func(arr);

  auto end_time = std::chrono::high_resolution_clock::now();
  long mem_after = getMemoryUsageKB();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
  long mem_used = mem_after - mem_before;
  if (mem_used < 0)
    mem_used = 0;

  csv_file << alg_name << "," << n << "," << t << "," << d << "," << m << "," << duration << "," << mem_used << "\n"
           << std::flush;
}

int main() {
  const bool TEST_MERGE_FAMILY = true;
  const bool TEST_PATIENCE = true;
  const bool TEST_QUICKSORT = true;
  const bool TEST_STD_SORT = true;

  std::vector<std::string> N_vals = {"10", "1000", "100000", "10000000"};
  std::vector<std::string> T_vals = {"ascendente", "descendente", "aleatorio"};
  std::vector<std::string> D_vals = {"D1", "D7"};
  std::vector<std::string> M_vals = {"a", "b", "c"};

  std::ofstream csv_file("data/measurements/sorting_results.csv");

  csv_file << "Algorithm,N,Type,Domain,Sample,Time_us,Memory_KB\n";

  std::cout << "--- Iniciando Procesamiento Modulado ---\n";

  for (const auto &n : N_vals) {
    for (const auto &t : T_vals) {
      for (const auto &d : D_vals) {
        for (const auto &m : M_vals) {

          std::string base_name = n + "_" + t + "_" + d + "_" + m;
          std::string in_filepath = "data/array_input/" + base_name + ".txt";
          std::string out_filepath = "data/array_output/" + base_name + "_out.txt";

          std::vector<int> base_arr = loadArrayFromFile(in_filepath);
          if (base_arr.empty())
            continue;

          std::cout << "Procesando " << base_name << "...\n";

          if (TEST_MERGE_FAMILY) {
            measureAlgorithm("MergeSort", runMergeSort, base_arr, csv_file, n, t, d, m);
          }

          if (TEST_PATIENCE) {
            if (n != "10000000") {
              measureAlgorithm("PatienceSort", runPatienceSort, base_arr, csv_file, n, t, d, m);
            } else {
              std::cout << "  -> PatienceSort saltado por límite de seguridad (N=" << n << ")\n";
            }
          }

          if (TEST_QUICKSORT) {

            if (n != "10000000") {
              measureAlgorithm("QuickSort", runQuickSort, base_arr, csv_file, n, t, d, m);
            } else {
              std::cout << "  -> QuickSort saltado por límite de seguridad " << "(N=" << n << ")\n";
            }
          }

          if (TEST_STD_SORT) {
            measureAlgorithm("StdSort", runStdSort, base_arr, csv_file, n, t, d, m);
          }

          if (TEST_STD_SORT) {
            runStdSort(base_arr);
            saveArrayToFile(base_arr, out_filepath);
          }
        }
      }
    }
  }

  csv_file.close();
  std::cout << "¡Iteración completada! Datos añadidos a sorting_results.csv\n";
  return 0;
}