#ifndef MATRIX_ALGORITHMS_HPP
#define MATRIX_ALGORITHMS_HPP

#include <vector>

void runNaive(const std::vector<std::vector<int>> &A,
              const std::vector<std::vector<int>> &B,
              std::vector<std::vector<int>> &C);

void runStrassen(const std::vector<std::vector<int>> &A,
                 const std::vector<std::vector<int>> &B,
                 std::vector<std::vector<int>> &C);

#endif