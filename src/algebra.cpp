// #include "algebra.h"
// #include <iostream>
// #include <iomanip>
// #include <random>
// #include <algorithm>

// namespace algebra {
//     template<typename T>
//     MATRIX<T> create_matrix(std::size_t rows, std::size_t columns, std::optional<MatrixType> type,
//                             std::optional<T> lowerBound, std::optional<T> upperBound) {
//         MATRIX<T> matrix(rows, std::vector<T>(columns));
//         switch (type.value()) {
//             case MatrixType::Zeros:
//                 for (auto& row : matrix) std::fill(row.begin(), row.end(), static_cast<T>(0));
//                 break;
//             case MatrixType::Ones:
//                 for (auto& row : matrix) std::fill(row.begin(), row.end(), static_cast<T>(1));
//                 break;
//             case MatrixType::Identity:
//                 if (rows != columns) throw std::logic_error("Identity matrix must be square.");
//                 for (std::size_t i = 0; i < rows; i++) matrix[i][i] = static_cast<T>(1);
//                 break;
//             case MatrixType::Random:
//                 if (!lowerBound.has_value() || !upperBound.has_value() || lowerBound >= upperBound) {
//                     throw std::logic_error("For random matrix, specify valid lowerBound < upperBound.");
//                 }
//                 std::random_device rd;
//                 std::mt19937 gen(rd());
//                 std::uniform_real_distribution<T> dis(*lowerBound, *upperBound);
//                 for (auto& row : matrix) for (auto& elem : row) elem = dis(gen);
//                 break;
//         }
//         return matrix;
//     }
// }