#include "algebra.h"

#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>

using namespace algebra;

// "============================================="
// "              create_matrix Tests            "
// "============================================="

// Test creation of zero matrix
TEST(aut_ap_2024_spring_hw1, CreateZeroMatrix) {
	auto matrix = create_matrix<int>(3, 3, MatrixType::Zeros);
	for (const auto &row : matrix) {
		for (int elem : row) {
			EXPECT_EQ(elem, 0) << "All elements in a zero matrix should be 0.";
		}
	}
}

// Test creation of ones matrix
TEST(aut_ap_2024_spring_hw1, CreateOnesMatrix) {
	auto matrix = create_matrix<double>(2, 2, MatrixType::Ones);
	for (const auto &row : matrix) {
		for (double elem : row) {
			EXPECT_EQ(elem, 1.0)
				<< "All elements in a ones matrix should be 1.";
		}
	}
}

// Test creation of identity matrix
TEST(aut_ap_2024_spring_hw1, CreateIdentityMatrix) {
	auto matrix = create_matrix<float>(4, 4, MatrixType::Identity);
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (i == j) {
				EXPECT_EQ(matrix[i][j], 1.0f)
					<< "Diagonal elements of an identity matrix should be 1.";
			} else {
				EXPECT_EQ(matrix[i][j], 0.0f) << "Non-diagonal elements of an "
												 "identity matrix should be 0.";
			}
		}
	}
}

// Test identity matrix requirement for square dimensions
TEST(aut_ap_2024_spring_hw1, IdentityMatrixSquareRequirement) {
	EXPECT_ANY_THROW(create_matrix<int>(3, 2, MatrixType::Identity));
}

// Test creation of random matrix within bounds
TEST(aut_ap_2024_spring_hw1, CreateRandomMatrixWithinBounds) {
	auto matrix = create_matrix<double>(2, 2, MatrixType::Random, 0.0, 1.0);
	for (const auto &row : matrix) {
		for (double elem : row) {
			EXPECT_GE(elem, 0.0) << "Random matrix elements should be >= 0.0.";
			EXPECT_LE(elem, 1.0) << "Random matrix elements should be <= 1.0.";
		}
	}
}

// Test lower bound less than upper bound for random matrix
TEST(aut_ap_2024_spring_hw1, RandomMatrixBounds) {
	EXPECT_ANY_THROW(create_matrix<int>(2, 2, MatrixType::Random, 5, 4));
}

// Test that an exception is thrown if only one bound is specified for random
// matrix
TEST(aut_ap_2024_spring_hw1, RandomMatrixSingleBoundSpecified) {
	EXPECT_ANY_THROW(
		create_matrix<double>(2, 2, MatrixType::Random, std::nullopt, 10.0));
	EXPECT_ANY_THROW(
		create_matrix<double>(2, 2, MatrixType::Random, 0.0, std::nullopt));
}

// Test matrix creation with invalid dimensions
TEST(aut_ap_2024_spring_hw1, InvalidDimensions) {
	EXPECT_ANY_THROW(create_matrix<int>(0, 5, MatrixType::Zeros));
	EXPECT_ANY_THROW(create_matrix<int>(5, 0, MatrixType::Zeros));
}

// Test for handling of the largest possible elements in random matrix
TEST(aut_ap_2024_spring_hw1, RandomMatrixExtremeValues) {
	EXPECT_NO_THROW(create_matrix<int>(2, 2, MatrixType::Random,
									   std::numeric_limits<int>::min(),
									   std::numeric_limits<int>::max()));
}

// "============================================="
// "                 sum_sub Tests               "
// "============================================="

// Test summation of two matrices with the same dimensions
TEST(aut_ap_2024_spring_hw1, SumOfMatrices) {
	MATRIX<int> matrixA = {{1, 2}, {3, 4}};
	MATRIX<int> matrixB = {{5, 6}, {7, 8}};
	MATRIX<int> expected = {{6, 8}, {10, 12}};

	auto result = sum_sub(matrixA, matrixB);
	EXPECT_EQ(result, expected) << "Summation of matrices failed.";
}

// Test subtraction of two matrices with the same dimensions
TEST(aut_ap_2024_spring_hw1, SubtractionOfMatrices) {
	MATRIX<int> matrixA = {{10, 20}, {30, 40}};
	MATRIX<int> matrixB = {{1, 2}, {3, 4}};
	MATRIX<int> expected = {{9, 18}, {27, 36}};

	auto result = sum_sub(matrixA, matrixB, "sub");
	EXPECT_EQ(result, expected) << "Subtraction of matrices failed.";
}

// Test for dimension mismatch
TEST(aut_ap_2024_spring_hw1, DimensionMismatch) {
	MATRIX<int> matrixA = {{1, 2, 3}, {4, 5, 6}};
	MATRIX<int> matrixB = {{1, 2}, {3, 4}, {5, 6}};

	EXPECT_ANY_THROW(sum_sub(matrixA, matrixB))
		<< "Operation with dimension mismatch should throw.";
	EXPECT_ANY_THROW(sum_sub(matrixA, matrixB, "sub"))
		<< "Operation with dimension mismatch should throw.";
}

// Test operation parameter with invalid value defaults to sum
TEST(aut_ap_2024_spring_hw1, InvalidOperationParameter) {
	MATRIX<int> matrixA = {{1, 2}, {3, 4}};
	MATRIX<int> matrixB = {{5, 6}, {7, 8}};
	MATRIX<int> expectedSum = {{6, 8}, {10, 12}};

	// Assuming any invalid operation defaults to sum
	auto result = sum_sub(matrixA, matrixB, "invalid_operation");
	EXPECT_EQ(result, expectedSum)
		<< "Invalid operation parameter should default to summation.";
}

// Test empty matrices
TEST(aut_ap_2024_spring_hw1, EmptyMatrices) {
	MATRIX<int> matrixA = {};
	MATRIX<int> matrixB = {};
	MATRIX<int> expected = {};

	auto resultSum = sum_sub(matrixA, matrixB);
	EXPECT_EQ(resultSum, expected) << "Summation of empty matrices failed.";

	auto resultSub = sum_sub(matrixA, matrixB, "sub");
	EXPECT_EQ(resultSub, expected) << "Subtraction of empty matrices failed.";
}

// Test single element matrices
TEST(aut_ap_2024_spring_hw1, SingleElementMatrices) {
	MATRIX<int> matrixA = {{5}};
	MATRIX<int> matrixB = {{3}};
	MATRIX<int> expectedSum = {{8}};
	MATRIX<int> expectedSub = {{2}};

	auto resultSum = sum_sub(matrixA, matrixB);
	EXPECT_EQ(resultSum, expectedSum)
		<< "Summation of single element matrices failed.";

	auto resultSub = sum_sub(matrixA, matrixB, "sub");
	EXPECT_EQ(resultSub, expectedSub)
		<< "Subtraction of single element matrices failed.";
}

// "============================================="
// "                multiply Tests               "
// "============================================="

// Test scalar multiplication
TEST(aut_ap_2024_spring_hw1, ScalarMultiplication) {
	MATRIX<int> mat = {{1, 2}, {3, 4}};
	int scalar = 3;
	MATRIX<int> expected = {{3, 6}, {9, 12}};

	auto result = multiply(mat, scalar);
	EXPECT_EQ(result, expected) << "Scalar multiplication failed.";
}

// Test matrix multiplication with valid dimensions
TEST(aut_ap_2024_spring_hw1, ValidMatrixMultiplication) {
	MATRIX<int> matrixA = {{1, 2, 3}, {4, 5, 6}};
	MATRIX<int> matrixB = {{7, 8}, {9, 10}, {11, 12}};
	MATRIX<int> expected = {{58, 64}, {139, 154}};

	auto result = multiply(matrixA, matrixB);
	EXPECT_EQ(result, expected) << "Matrix multiplication failed.";
}

// Test matrix multiplication with dimension mismatch
TEST(aut_ap_2024_spring_hw1, DimensionMismatchMatrixMultiplication) {
	MATRIX<int> matrixA = {{1, 2}, {3, 4}};
	MATRIX<int> matrixB = {{5, 6}, {7, 8}, {9, 10}};

	EXPECT_ANY_THROW(multiply(matrixA, matrixB))
		<< "Matrix multiplication should throw on dimension mismatch.";
}

// Test scalar multiplication with zero scalar
TEST(aut_ap_2024_spring_hw1, ZeroScalarMultiplication) {
	MATRIX<int> mat = {{1, 2}, {3, 4}};
	int scalar = 0;
	MATRIX<int> expected = {{0, 0}, {0, 0}};

	auto result = multiply(mat, scalar);
	EXPECT_EQ(result, expected) << "Scalar multiplication with zero failed.";
}

// Test matrix multiplication with identity matrix
TEST(aut_ap_2024_spring_hw1, IdentityMatrixMultiplication) {
	MATRIX<int> matrixA = {{1, 2}, {3, 4}};
	MATRIX<int> identity = {{1, 0}, {0, 1}};
	MATRIX<int> expected = matrixA; // Multiplying by identity matrix should
									// give the original matrix

	auto result = multiply(matrixA, identity);
	EXPECT_EQ(result, expected)
		<< "Matrix multiplication with identity matrix failed.";
}

// Test scalar multiplication with negative scalar
TEST(aut_ap_2024_spring_hw1, NegativeScalarMultiplication) {
	MATRIX<int> mat = {{1, 2}, {3, 4}};
	int scalar = -3;
	MATRIX<int> expected = {{-3, -6}, {-9, -12}};

	auto result = multiply(mat, scalar);
	EXPECT_EQ(result, expected)
		<< "Scalar multiplication with negative scalar failed.";
}

// Test empty matrix with scalar multiplication
TEST(aut_ap_2024_spring_hw1, ScalarMultiplicationEmptyMatrix) {
	MATRIX<int> mat = {};
	int scalar = 5;
	MATRIX<int> expected = {};

	auto result = multiply(mat, scalar);
	EXPECT_EQ(result, expected)
		<< "Scalar multiplication with an empty matrix failed.";
}

// Test empty matrices with matrix multiplication
TEST(aut_ap_2024_spring_hw1, MatrixMultiplicationEmptyMatrices) {
	MATRIX<int> matrixA = {};
	MATRIX<int> matrixB = {};
	MATRIX<int> expected = {};

	EXPECT_ANY_THROW(multiply(matrixA, matrixB))
		<< "Matrix multiplication with empty matrices should throw.";
}

// "============================================="
// "             hadamard_product Tests          "
// "============================================="

// Test Hadamard Product of two matrices with the same dimensions
TEST(aut_ap_2024_spring_hw1, ValidHadamardProduct) {
	MATRIX<int> matA = {{1, 2, 3}, {4, 5, 6}};
	MATRIX<int> matB = {{7, 8, 9}, {10, 11, 12}};
	MATRIX<int> expected = {{7, 16, 27}, {40, 55, 72}};

	auto result = hadamard_product(matA, matB);
	EXPECT_EQ(result, expected) << "Hadamard product computation failed.";
}

// Test Hadamard Product with dimension mismatch
TEST(aut_ap_2024_spring_hw1, DimensionMismatch) {
	MATRIX<int> matA = {{1, 2, 3}};
	MATRIX<int> matB = {{4, 5}, {6, 7}};

	EXPECT_ANY_THROW(hadamard_product(matA, matB))
		<< "Hadamard product should throw on dimension mismatch.";
}

// Test Hadamard Product with empty matrices
TEST(aut_ap_2024_spring_hw1, EmptyMatrices) {
	MATRIX<int> matA = {};
	MATRIX<int> matB = {};

	auto result = hadamard_product(matA, matB);
	MATRIX<int> expected = {};
	EXPECT_EQ(result, expected)
		<< "Hadamard product of empty matrices should return an empty matrix.";
}

// Test Hadamard Product for matrices with a single element
TEST(aut_ap_2024_spring_hw1, SingleElementMatrices) {
	MATRIX<int> matA = {{2}};
	MATRIX<int> matB = {{3}};
	MATRIX<int> expected = {{6}};

	auto result = hadamard_product(matA, matB);
	EXPECT_EQ(result, expected)
		<< "Hadamard product of single element matrices failed.";
}

// Test Hadamard Product for negative values
TEST(aut_ap_2024_spring_hw1, NegativeValues) {
	MATRIX<int> matA = {{-1, -2}, {-3, -4}};
	MATRIX<int> matB = {{5, 6}, {7, 8}};
	MATRIX<int> expected = {{-5, -12}, {-21, -32}};

	auto result = hadamard_product(matA, matB);
	EXPECT_EQ(result, expected)
		<< "Hadamard product with negative values failed.";
}

// Test Hadamard Product with zero values in matrices
TEST(aut_ap_2024_spring_hw1, ZeroValues) {
	MATRIX<int> matA = {{0, 2}, {3, 0}};
	MATRIX<int> matB = {{4, 0}, {0, 5}};
	MATRIX<int> expected = {{0, 0}, {0, 0}};

	auto result = hadamard_product(matA, matB);
	EXPECT_EQ(result, expected) << "Hadamard product with zero values failed.";
}

// "============================================="
// "                transpose Tests              "
// "============================================="

// Test transposing a square matrix
TEST(aut_ap_2024_spring_hw1, TransposeSquareMatrix) {
	MATRIX<int> mat = {{1, 2}, {3, 4}};
	MATRIX<int> expected = {{1, 3}, {2, 4}};

	auto result = transpose(mat);
	EXPECT_EQ(result, expected) << "Transpose of a square matrix failed.";
}

// Test transposing a non-square matrix
TEST(aut_ap_2024_spring_hw1, TransposeNonSquareMatrix) {
	MATRIX<int> mat = {{1, 2, 3}, {4, 5, 6}};
	MATRIX<int> expected = {{1, 4}, {2, 5}, {3, 6}};

	auto result = transpose(mat);
	EXPECT_EQ(result, expected) << "Transpose of a non-square matrix failed.";
}

// Test transposing an empty matrix
TEST(aut_ap_2024_spring_hw1, TransposeEmptyMatrix) {
	MATRIX<int> mat = {};
	MATRIX<int> expected = {};

	auto result = transpose(mat);
	EXPECT_EQ(result, expected)
		<< "Transpose of an empty matrix should return an empty matrix.";
}

// Test transposing a matrix with one row
TEST(aut_ap_2024_spring_hw1, TransposeSingleRow) {
	MATRIX<int> mat = {{1, 2, 3}};
	MATRIX<int> expected = {{1}, {2}, {3}};

	auto result = transpose(mat);
	EXPECT_EQ(result, expected) << "Transpose of a single row matrix failed.";
}

// Test transposing a matrix with one column
TEST(aut_ap_2024_spring_hw1, TransposeSingleColumn) {
	MATRIX<int> mat = {{1}, {2}, {3}};
	MATRIX<int> expected = {{1, 2, 3}};

	auto result = transpose(mat);
	EXPECT_EQ(result, expected)
		<< "Transpose of a single column matrix failed.";
}

// "============================================="
// "                  trace Tests                "
// "============================================="

// Test calculating the trace of a square matrix
TEST(aut_ap_2024_spring_hw1, ValidSquareMatrix) {
	MATRIX<int> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	int expectedTrace = 15;

	auto result = trace(mat);
	EXPECT_EQ(result, expectedTrace)
		<< "Trace calculation for a square matrix failed.";
}

// Test calculating the trace of a non-square matrix throws error
TEST(aut_ap_2024_spring_hw1, NonSquareMatrix) {
	MATRIX<int> mat = {{1, 2, 3}, {4, 5, 6}};

	EXPECT_ANY_THROW(trace(mat))
		<< "Trace calculation should throw an error for non-square matrices.";
}

// Test calculating the trace of an empty matrix
TEST(aut_ap_2024_spring_hw1, EmptyMatrix) {
	MATRIX<int> mat = {};

	EXPECT_ANY_THROW(trace(mat))
		<< "Trace calculation should throw an error for an empty matrix.";
}

// Test calculating the trace of a single element matrix
TEST(aut_ap_2024_spring_hw1, SingleElementMatrix) {
	MATRIX<int> mat = {{42}};
	int expectedTrace = 42;

	auto result = trace(mat);
	EXPECT_EQ(result, expectedTrace)
		<< "Trace calculation for a single element matrix failed.";
}

// "============================================="
// "               determinant Tests             "
// "============================================="

// Test calculating the determinant of a 2x2 square matrix
TEST(aut_ap_2024_spring_hw1, Determinant2x2Matrix) {
	MATRIX<double> mat = {{4, 3}, {6, 3}};
	double expectedDet = -6.0;

	auto result = determinant(mat);
	EXPECT_NEAR(result, expectedDet, 1e-5)
		<< "Determinant calculation for a 2x2 matrix failed.";
}

// Test calculating the determinant of a larger square matrix
TEST(aut_ap_2024_spring_hw1, DeterminantLargerMatrix) {
	// Example for a 3x3 matrix
	MATRIX<double> mat = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
	// The expected determinant can be calculated manually or using a reliable
	// tool
	double expectedDet = -1.0;

	auto result = determinant(mat);
	EXPECT_NEAR(result, expectedDet, 1e-6)
		<< "Determinant calculation for a larger matrix failed.";
}

// Test calculating the determinant of a non-square matrix throws error
TEST(aut_ap_2024_spring_hw1, NonSquareMatrix) {
	MATRIX<double> mat = {{1, 2, 3}, {4, 5, 6}};

	EXPECT_ANY_THROW(determinant(mat))
		<< "Determinant calculation should throw an error for non-square "
		   "matrices.";
}

// Test calculating the determinant of an empty matrix
TEST(aut_ap_2024_spring_hw1, EmptyMatrix) {
	MATRIX<double> mat = {};

	EXPECT_ANY_THROW(determinant(mat))
		<< "Determinant calculation should throw an error for an empty matrix.";
}

// Test calculating the determinant of a single element matrix
TEST(aut_ap_2024_spring_hw1, SingleElementMatrix) {
	MATRIX<double> mat = {{42}};
	double expectedDet = 42;

	auto result = determinant(mat);
	EXPECT_NEAR(result, expectedDet, 1e-6)
		<< "Determinant calculation for a single element matrix failed.";
}

// "============================================="
// "                 Inverse Tests               "
// "============================================="

// Test calculating the inverse of a 2x2 square matrix
TEST(aut_ap_2024_spring_hw1, Inverse2x2Matrix) {
	MATRIX<double> mat = {{4, 7}, {2, 6}};
	MATRIX<double> expectedInv = {{0.6, -0.7}, {-0.2, 0.4}};

	auto result = inverse(mat);
	for (size_t i = 0; i < result.size(); ++i) {
		for (size_t j = 0; j < result[i].size(); ++j) {
			EXPECT_NEAR(result[i][j], expectedInv[i][j], 1e-5)
				<< "Inverse calculation for a 2x2 matrix failed at element ["
				<< i << "][" << j << "].";
		}
	}
}

// Test error handling for a non-square matrix
TEST(aut_ap_2024_spring_hw1, NonSquareMatrix) {
	MATRIX<double> mat = {{1, 2, 3}, {4, 5, 6}};

	EXPECT_ANY_THROW(inverse(mat))
		<< "Inverse calculation should throw an error for non-square matrices.";
}

// Test error handling for a matrix with zero determinant
TEST(aut_ap_2024_spring_hw1, ZeroDeterminant) {
	MATRIX<double> mat = {{1, 2}, {2, 4}}; // This matrix has a determinant of 0

	EXPECT_ANY_THROW(inverse(mat))
		<< "Inverse calculation should throw an error for matrices with zero "
		   "determinant.";
}

// Test calculating the inverse of an empty matrix
TEST(aut_ap_2024_spring_hw1, EmptyMatrix) {
	MATRIX<double> mat = {};

	EXPECT_ANY_THROW(inverse(mat))
		<< "Inverse calculation should throw an error for an empty matrix.";
}
