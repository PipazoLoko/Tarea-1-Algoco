#include "algorithms/matrix_algorithms.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
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

std::vector<std::vector<int>> loadMatrixFromFile(const std::string &filepath) {
  std::vector<std::vector<int>> matrix;
  std::ifstream file(filepath);
  std::string line;

  while (std::getline(file, line)) {
    std::vector<int> row;
    std::stringstream ss(line);
    int num;
    while (ss >> num) {
      row.push_back(num);
    }
    if (!row.empty()) {
      matrix.push_back(row);
    }
  }
  return matrix;
}

void saveMatrixToFile(const std::vector<std::vector<int>> &matrix, const std::string &filepath) {
  std::ofstream file(filepath);
  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix[i].size(); j++) {
      file << matrix[i][j] << (j == matrix[i].size() - 1 ? "" : " ");
    }
    file << "\n";
  }
}

void measureAlgorithm(
    const std::string &alg_name,
    void (*mult_func)(const std::vector<std::vector<int>> &,
                      const std::vector<std::vector<int>> &,
                      std::vector<std::vector<int>> &),
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B,
    std::ofstream &csv_file, const std::string &n,
    const std::string &t, const std::string &d, const std::string &m) {

  int size = A.size();
  std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));

  long mem_before = getMemoryUsageKB();
  auto start_time = std::chrono::high_resolution_clock::now();

  mult_func(A, B, C);

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
  const bool TEST_NAIVE = true;
  const bool TEST_STRASSEN = true;
  const bool SAVE_OUTPUT = true;

  std::vector<std::string> N_vals = {"16", "64", "256", "1024"};
  std::vector<std::string> T_vals = {"dispersa", "diagonal", "densa"};
  std::vector<std::string> D_vals = {"D0", "D10"};
  std::vector<std::string> M_vals = {"a", "b", "c"};

  std::ofstream csv_file("data/measurements/matrix_results.csv", std::ios::app);

  csv_file.seekp(0, std::ios::end);
  if (csv_file.tellp() == 0) {
    csv_file << "Algorithm,N,Type,Domain,Sample,Time_us,Memory_KB\n";
  }

  std::cout << "--- Iniciando Procesamiento Modulado de Matrices ---\n";

  for (const auto &n : N_vals) {
    for (const auto &t : T_vals) {
      for (const auto &d : D_vals) {
        for (const auto &m : M_vals) {

          std::string base_name = n + "_" + t + "_" + d + "_" + m;
          std::string in_filepath_A = "data/matrix_input/" + base_name + "_1.txt";
          std::string in_filepath_B = "data/matrix_input/" + base_name + "_2.txt";

          std::string out_filepath = "data/matrix_output/" + base_name + "_out.txt";

          std::vector<std::vector<int>> A = loadMatrixFromFile(in_filepath_A);
          std::vector<std::vector<int>> B = loadMatrixFromFile(in_filepath_B);

          if (A.empty() || B.empty()) {
            std::cout << "[FALTA ARCHIVO] Saltando: " << base_name << "\n";
            continue;
          }

          std::cout << "Procesando " << base_name << "...\n";

          if (TEST_NAIVE) {
            if (std::stoi(n) <= 1024) {
              measureAlgorithm("Naive", runNaive, A, B, csv_file, n, t, d, m);
            } else {
              std::cout << "  -> Naive saltado por límite de seguridad (N=" << n << ")\n";
            }
          }

          if (TEST_STRASSEN) {
            measureAlgorithm("Strassen", runStrassen, A, B, csv_file, n, t, d, m);
          }

          if (SAVE_OUTPUT) {
            std::vector<std::vector<int>> C(A.size(), std::vector<int>(A.size(), 0));
            runNaive(A, B, C);
            saveMatrixToFile(C, out_filepath);
          }
        }
      }
    }
  }

  csv_file.close();
  std::cout << "¡Iteración completada! Datos añadidos a matrix_results.csv\n";
  return 0;
}