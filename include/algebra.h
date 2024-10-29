#ifndef AUT_AP_2024_Spring_HW1
#define AUT_AP_2024_Spring_HW1

#include <vector>
#include <optional>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>

using Matrix = std::vector<std::vector<double>>;
using std::vector;

namespace algebra {

    // Matrix data structure
    template<typename T>
    using MATRIX = std::vector<std::vector<T>>;

    // Matrix initialization types
    enum class MatrixType { Zeros, Ones, Identity, Random };

    // Function template for matrix initialization
    template<typename T>
    MATRIX<T> create_matrix(std::size_t rows, std::size_t columns, std::optional<MatrixType> type = MatrixType::Zeros,
                            std::optional<T> lowerBound = std::nullopt, std::optional<T> upperBound = std::nullopt) {
        // 参数检查
        if (type.value_or(MatrixType::Zeros) == MatrixType::Random) {
            if (!lowerBound.has_value() || !upperBound.has_value()) {
                throw std::logic_error("For random matrix, specify valid lowerBound and upperBound.");
            }
            if (lowerBound.value() >= upperBound.value()) {
                throw std::logic_error("lowerBound should be less than upperBound.");
            }
        }

        if (rows <= 0 || columns <= 0) {
            throw std::logic_error("Matrix dimensions must be greater than zero.");
        }

        // 初始化矩阵
        MATRIX<T> matrix(rows, std::vector<T>(columns));

        // 根据矩阵类型填充数据
        switch (type.value_or(MatrixType::Zeros)) {
            case MatrixType::Zeros:
                for (auto& row : matrix) std::fill(row.begin(), row.end(), static_cast<T>(0));
                break;

            case MatrixType::Ones:
                for (auto& row : matrix) std::fill(row.begin(), row.end(), static_cast<T>(1));
                break;

            case MatrixType::Identity:
                if (rows != columns) {
                    throw std::logic_error("Identity matrix must be square.");
                }
                for (std::size_t i = 0; i < rows; i++) matrix[i][i] = static_cast<T>(1);
                break;

            case MatrixType::Random: {
                // 使用 std::conditional 选择合适的分布类型
                std::mt19937 gen(std::random_device{}());
                using Distribution = typename std::conditional<
                    std::is_floating_point<T>::value,
                    std::uniform_real_distribution<T>,
                    std::uniform_int_distribution<T>>::type;

                Distribution dist(*lowerBound, *upperBound);
                for (auto& row : matrix)
                    for (auto& elem : row)
                        elem = dist(gen);
                break;
            }
        }

        return matrix;
    }

    // Display function
    // template<typename T>
    // void display(const MATRIX<T>& matrix) {
    //     for (const auto& row : matrix) {
    //         for (const auto& elem : row) {
    //             std::cout << std::format("{:>7.3f} ", elem); // 右对齐，保留3位小数，总宽度为7
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    // Matrix addition and subtraction
    template<typename T>
    MATRIX<T> sum_sub(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB, std::optional<std::string> operation = "sum") {
        if (matrixA.size() != matrixB.size() || matrixA[0].size() != matrixB[0].size()) {
            throw std::invalid_argument("Matrix dimensions must match.");
        }
        MATRIX<T> result(matrixA.size(), std::vector<T>(matrixA[0].size()));
        for (std::size_t i = 0; i < matrixA.size(); ++i) {
            for (std::size_t j = 0; j < matrixA[0].size(); ++j) {
                result[i][j] = (operation.value() == "sub") ? (matrixA[i][j] - matrixB[i][j]) : (matrixA[i][j] + matrixB[i][j]);
            }
        }
        return result;
    }

    // Scalar multiplication
    template<typename T>
    MATRIX<T> multiply(const MATRIX<T>& matrix, const T scalar) {
        MATRIX<T> result = matrix;
        for (auto& row : result)
            for (auto& elem : row)
                elem *= scalar;
        return result;
    }

    // Matrix multiplication
    template<typename T>
    MATRIX<T> multiply(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB) {
        if (matrixA[0].size() != matrixB.size()) {
            throw std::invalid_argument("Matrix dimensions are incompatible for multiplication.");
        }
        MATRIX<T> result(matrixA.size(), std::vector<T>(matrixB[0].size(), 0));
        for (std::size_t i = 0; i < matrixA.size(); ++i) {
            for (std::size_t j = 0; j < matrixB[0].size(); ++j) {
                for (std::size_t k = 0; k < matrixA[0].size(); ++k) {
                    result[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
        return result;
    }

    // Hadamard product
    template<typename T>
    MATRIX<T> hadamard_product(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB) {
        if (matrixA.size() != matrixB.size() || matrixA[0].size() != matrixB[0].size()) {
            throw std::invalid_argument("Matrix dimensions must match.");
        }
        MATRIX<T> result(matrixA.size(), std::vector<T>(matrixA[0].size()));
        for (std::size_t i = 0; i < matrixA.size(); ++i) {
            for (std::size_t j = 0; j < matrixA[0].size(); ++j) {
                result[i][j] = matrixA[i][j] * matrixB[i][j];
            }
        }
        return result;
    }

    // Transpose
    template<typename T>
    MATRIX<T> transpose(const MATRIX<T>& matrix) {
        MATRIX<T> result(matrix[0].size(), std::vector<T>(matrix.size()));
        for (std::size_t i = 0; i < matrix.size(); ++i) {
            for (std::size_t j = 0; j < matrix[0].size(); ++j) {
                result[j][i] = matrix[i][j];
            }
        }
        return result;
    }

    // Trace of a matrix
    template<typename T>
    T trace(const MATRIX<T>& matrix) {
        if (matrix.size() != matrix[0].size()) {
            throw std::invalid_argument("Matrix must be square to calculate trace.");
        }
        T sum = 0;
        for (std::size_t i = 0; i < matrix.size(); ++i) {
            sum += matrix[i][i];
        }
        return sum;
    }

    // Determinant (for 2x2 matrices as a simple example)
    template<typename T>
    double determinant(const MATRIX<T>& matrix) {
        if (matrix.size() != matrix[0].size()) {
            throw std::invalid_argument("Matrix must be square to calculate determinant.");
        }
        // Example implementation for 2x2 matrix
        if (matrix.size() == 2) {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }
        throw std::invalid_argument("Determinant calculation for larger matrices not implemented.");
    }

    // Inverse (for 2x2 matrices as a simple example)
    template<typename T>
    MATRIX<double> inverse(const MATRIX<T>& matrix) {
        if (matrix.size() != 2 || matrix[0].size() != 2) {
            throw std::invalid_argument("Inverse calculation currently only implemented for 2x2 matrices.");
        }
        double det = determinant(matrix);
        if (det == 0) {
            throw std::invalid_argument("Matrix is singular and cannot be inverted.");
        }
        return {{matrix[1][1] / det, -matrix[0][1] / det},
                {-matrix[1][0] / det, matrix[0][0] / det}};
    }

} // namespace algebra

#endif //AUT_AP_2024_Spring_HW1
