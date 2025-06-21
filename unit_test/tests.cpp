#include "tests.hpp"

#include <cmath>
#include <cstdint>
#include <stdexcept>

#include "gtest/gtest.h"

enum { matrix_in_array = 15 };

namespace {
TEST(S21MatrixTest, Constructor_Default) {
  S21Matrix m;
  EXPECT_EQ(m.Rows(), 0);
  EXPECT_EQ(m.Cols(), 0);
}

TEST(S21MatrixTest, Constructor_Parameterized1) {
  const int N = 55;
  const int M = 57;
  for (int i = -3; i < N; ++i) {
    for (int j = -3; j < M; ++j) {
      check_sizes(i, j);
      check_zero_values(i, j);
    }
  }
}

TEST(S21MatrixTest, Constructor_Parameterized2) {
  ASSERT_NO_THROW(S21Matrix dst(0, 0));
  ASSERT_NO_THROW(S21Matrix dst(0, 1));
  ASSERT_NO_THROW(S21Matrix dst(1, 0));
  ASSERT_THROW(S21Matrix dst(-1, 0), std::invalid_argument);
  ASSERT_THROW(S21Matrix dst(0, -1), std::invalid_argument);
}

TEST(S21MatrixTest, Getters) {
  int sizes[matrix_in_array];
  for (int i = 0; i < matrix_in_array; ++i) {
    sizes[i] = 1 + (int)random() % 10;
  }

  const int len = sizeof(sizes) / sizeof(int);
  for (int i = 0, j = len - 1; i < len; ++i, --j) {
    S21Matrix matrix(sizes[i], sizes[j]);
    EXPECT_EQ(matrix.Rows(), sizes[i]);
    EXPECT_EQ(matrix.Cols(), sizes[j]);
    for (int i = 0; i < matrix.Rows(); ++i) {
      for (int j = 0; j < matrix.Cols(); ++j) {
        EXPECT_EQ(matrix(i, j), 0);
      }
    }
    ASSERT_THROW(matrix(INT32_MAX, INT32_MAX), std::out_of_range);
  }
}

TEST(S21MatrixTest, EqMatrix) {
  S21Matrix array[matrix_in_array];

  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(array[i]);
  }
  for (int i = 0; i < matrix_in_array; ++i) {
    EXPECT_EQ(array[i].EqMatrix(array[i]), true);
    for (int j = 0; j < matrix_in_array; ++j) {
      if (i != j) {
        EXPECT_EQ(array[i].EqMatrix(array[j]), false);
      }
    }
  }
}

TEST(S21MatrixTest, EqOverloadMatrix) {
  S21Matrix array[matrix_in_array];

  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(array[i]);
  }
  for (int i = 0, j = matrix_in_array - 1; i < matrix_in_array; ++i, --j) {
    EXPECT_EQ(array[i] == array[i], true);
    for (int j = 0; j < matrix_in_array; ++j) {
      if (i != j) {
        EXPECT_EQ(array[i] == array[j], false);
      }
    }
  }
}

TEST(S21MatrixTest, Constructor_Copy) {
  S21Matrix src[matrix_in_array];

  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(src[i]);
  }

  for (int i = 0; i < matrix_in_array; ++i) {
    S21Matrix dst = src[i];
    EXPECT_EQ(dst == src[i], true);
    // если указатели matrix_ разные, то функции не будут равны
    src[i](0, 0) += 1;
    EXPECT_EQ(dst == src[i], false);
  }
}

TEST(S21MatrixTest, Copy) {
  S21Matrix src[matrix_in_array];

  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(src[i]);
  }

  for (int i = 0; i < matrix_in_array; ++i) {
    S21Matrix dst(0, 0);
    dst = src[i];
    EXPECT_EQ(dst == src[i], true);
    // если указатели matrix_ разные, то функции не будут равны
    src[i](0, 0) += 1;
    EXPECT_EQ(dst == src[i], false);
  }
}

TEST(S21MatrixTest, Constructor_Move) {
  S21Matrix src[matrix_in_array];

  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(src[i]);
  }

  for (int i = 0; i < matrix_in_array; ++i) {
    S21Matrix dst = std::move(src[i]);
    EXPECT_EQ(dst == src[i], false);
    ASSERT_THROW(src[i](0, 0) += 1, std::out_of_range);
    EXPECT_EQ(dst == src[i], false);
  }
}

TEST(S21MatrixTest, Setter) {
  S21Matrix arr[matrix_in_array];
  for (int i = 0; i < matrix_in_array; ++i) {
    random_matrix(arr[i]);
  }

  for (int n = 0; n < matrix_in_array; ++n) {
    for (int i = 0; i < arr[n].Rows(); ++i) {
      for (int j = 0; j < arr[n].Cols(); ++j) {
        double current = arr[n](i, j);
        double next = (double)(10000000 + random() % 10);
        arr[n](i, j) = next;
        EXPECT_NE(current, arr[n](i, j));
      }
    }
  }
}

TEST(S21MatrixTest, Constructor_Array) {
  double array[] = {2.6, -4.114, 0, 5.255};
  S21Matrix matrix(2, 2, array);
  EXPECT_EQ(matrix(0, 0), 2.6);
  EXPECT_EQ(matrix(0, 1), -4.114);
  EXPECT_EQ(matrix(1, 0), 0);
  EXPECT_EQ(matrix(1, 1), 5.255);
}

template <typename InplaceOp, typename BinaryOp, typename MethodOp>
void TestBinaryOperation(const S21Matrix& A, const S21Matrix& B,
                         const S21Matrix& expected, InplaceOp inplace_op_action,
                         BinaryOp binary_op_action, MethodOp method_action) {
  S21Matrix copy_A_1 = A;
  S21Matrix copy_A_2 = A;

  // operator+=, -=
  ASSERT_NO_THROW(inplace_op_action(copy_A_1, B));
  EXPECT_EQ(copy_A_1, expected);
  // operator+, -
  S21Matrix result;
  ASSERT_NO_THROW(result = binary_op_action(A, B));
  EXPECT_EQ(result, expected);
  // SumMatrix, SubMatrix, MulMatrix
  ASSERT_NO_THROW(method_action(copy_A_2, B));
  EXPECT_EQ(copy_A_2, expected);
}

template <typename InplaceOp, typename BinaryOp, typename MethodOp>
void TestBinaryOperation(const S21Matrix& A, const double& B,
                         const S21Matrix& expected, InplaceOp inplace_op_action,
                         BinaryOp binary_op_action, MethodOp method_action) {
  S21Matrix copy_A_1 = A;
  S21Matrix copy_A_2 = A;

  // operator*=
  ASSERT_NO_THROW(inplace_op_action(copy_A_1, B));
  EXPECT_EQ(copy_A_1, expected);
  // operator*=
  S21Matrix result;
  ASSERT_NO_THROW(result = binary_op_action(A, B));
  EXPECT_EQ(result, expected);
  // MulNumber
  ASSERT_NO_THROW(method_action(copy_A_2, B));
  EXPECT_EQ(copy_A_2, expected);
}

template <typename InplaceOp, typename BinaryOp, typename MethodOp>
void TestBinaryOperationFailure(const S21Matrix& A, const S21Matrix& B,
                                InplaceOp inplace_op_action,
                                BinaryOp binary_op_action,
                                MethodOp method_action) {
  S21Matrix copy_A_1 = A;
  S21Matrix copy_A_2 = A;

  ASSERT_THROW(inplace_op_action(copy_A_1, B), std::invalid_argument);
  ASSERT_THROW(binary_op_action(A, B), std::invalid_argument);
  ASSERT_THROW(method_action(copy_A_2, B), std::invalid_argument);
}

TEST(S21MatrixTest, Sum1) {
  double dataA[] = {2, 2, 1, 1};
  double dataB[] = {2, 2, 1, 1};
  double dataExpected[] = {4, 4, 2, 2};
  const S21Matrix A(2, 2, dataA);
  const S21Matrix B(2, 2, dataB);
  const S21Matrix expected(2, 2, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a += b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a + b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SumMatrix(b); });
}
TEST(S21MatrixTest, Sum2) {
  double dataA[] = {5.1f};
  double dataB[] = {2.2f};
  double dataExpected[] = {7.3f};
  const S21Matrix A(1, 1, dataA);
  const S21Matrix B(1, 1, dataB);
  const S21Matrix expected(1, 1, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a += b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a + b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SumMatrix(b); });
}
TEST(S21MatrixTest, Sum3) {
  double dataA[] = {5.1f};
  double dataB[] = {2.2f, 9};
  const S21Matrix A(1, 1, dataA);
  const S21Matrix B(1, 2, dataB);
  TestBinaryOperationFailure(
      A, B, [](S21Matrix& a, const S21Matrix& b) { return a += b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a + b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SumMatrix(b); });
}
TEST(S21MatrixTest, Sum4) {
  const S21Matrix A(1, 1);
  const S21Matrix B(2, 2);
  TestBinaryOperationFailure(
      A, B, [](S21Matrix& a, const S21Matrix& b) { return a += b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a + b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SumMatrix(b); });
}

TEST(S21MatrixTest, Sub1) {
  double dataA[] = {4, 4, 2, 2};
  double dataB[] = {2, 2, 1, 1};
  double dataExpected[] = {2, 2, 1, 1};
  const S21Matrix A(2, 2, dataA);
  const S21Matrix B(2, 2, dataB);
  const S21Matrix expected(2, 2, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a -= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a - b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SubMatrix(b); });
}
TEST(S21MatrixTest, Sub2) {
  double dataA[] = {7.3};
  double dataB[] = {2.2};
  double dataExpected[] = {5.1};
  const S21Matrix A(1, 1, dataA);
  const S21Matrix B(1, 1, dataB);
  const S21Matrix expected(1, 1, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a -= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a - b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SubMatrix(b); });
}
TEST(S21MatrixTest, Sub3) {
  double dataA[] = {5.1f};
  double dataB[] = {2.2f, 9};
  const S21Matrix A(1, 1, dataA);
  const S21Matrix B(1, 2, dataB);
  TestBinaryOperationFailure(
      A, B, [](S21Matrix& a, const S21Matrix& b) { return a -= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a - b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SubMatrix(b); });
}
TEST(S21MatrixTest, Sub4) {
  const S21Matrix A(1, 1);
  const S21Matrix B(2, 2);
  TestBinaryOperationFailure(
      A, B, [](S21Matrix& a, const S21Matrix& b) { return a -= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a - b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.SubMatrix(b); });
}

TEST(S21MatrixTest, MulNumber1) {
  double dataA[] = {4, 4, 2, 2};
  const S21Matrix A(2, 2, dataA);
  const int B = 1;
  double dataExpected[] = {4, 4, 2, 2};
  const S21Matrix expected(2, 2, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const double& b) { return a *= b; },
      [](const S21Matrix& a, const double& b) { return a * b; },
      [](S21Matrix& a, const double& b) { return a.MulNumber(b); });
}
TEST(S21MatrixTest, MulNumber2) {
  double dataA[] = {1.1f, -2.12454636f, 0.0, 100.0};
  const S21Matrix A(2, 2, dataA);
  const int B = -3;
  double dataExpected[] = {-3.3f, 6.37363908f, 0.0, -300.0};
  const S21Matrix expected(2, 2, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const double& b) { return a *= b; },
      [](const S21Matrix& a, const double& b) { return a * b; },
      [](S21Matrix& a, const double& b) { return a.MulNumber(b); });
}

TEST(S21MatrixTest, MulMatrix1) {
  double dataA[] = {1, 4, 2, 5, 3, 6};
  const S21Matrix A(3, 2, dataA);
  double dataB[] = {1, -1, 1, 2, 3, 4};
  const S21Matrix B(2, 3, dataB);
  double dataExpected[] = {9, 11, 17, 12, 13, 22, 15, 15, 27};
  const S21Matrix expected(3, 3, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a *= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a * b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.MulMatrix(b); });
}
TEST(S21MatrixTest, MulMatrix2) {
  double dataA[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  const S21Matrix A(3, 3, dataA);
  double dataB[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  const S21Matrix B(3, 3, dataB);
  double dataExpected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  const S21Matrix expected(3, 3, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a *= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a * b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.MulMatrix(b); });
}
TEST(S21MatrixTest, MulMatrix3) {
  const S21Matrix A(1, 1);
  const S21Matrix B(3, 4);
  TestBinaryOperationFailure(
      A, B, [](S21Matrix& a, const S21Matrix& b) { return a *= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a * b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.MulMatrix(b); });
}
TEST(S21MatrixTest, MulMatrix4) {
  double dataA[] = {2.2, 2.0, 3.0, 0.0, 4.0, 1.0};
  const S21Matrix A(2, 3, dataA);
  double dataB[] = {0.0, 3.0, 4.0, 5.0, 2.0, 5.0,
                    6.0, 7.0, 8.0, 7.0, 5.0, 43.0};
  const S21Matrix B(3, 4, dataB);
  double dataExpected[] = {28.0, 37.6, 35.8, 154.0, 16.0, 27.0, 29.0, 71.0};
  const S21Matrix expected(2, 4, dataExpected);
  TestBinaryOperation(
      A, B, expected, [](S21Matrix& a, const S21Matrix& b) { return a *= b; },
      [](const S21Matrix& a, const S21Matrix& b) { return a * b; },
      [](S21Matrix& a, const S21Matrix& b) { return a.MulMatrix(b); });
}

template <class ResultType, typename MethodOp>
void TestMethodOperation(const S21Matrix& A, MethodOp method_action,
                         const ResultType& expected) {
  ResultType result;
  ASSERT_NO_THROW(result = method_action(A));
  EXPECT_EQ(result, expected);
}

template <class ErrType, typename MethodOp>
void TestMethodOperationFailure(const S21Matrix& A, MethodOp method_action) {
  ASSERT_THROW(method_action(A), ErrType);
}

TEST(S21MatrixTest, Determinant1) {
  double dataA[] = {-48};
  S21Matrix A(1, 1, dataA);
  TestMethodOperation<double>(
      A, [](const S21Matrix& a) { return a.Determinant(); }, -48);
}
TEST(S21MatrixTest, Determinant2) {
  double dataA[] = {1, 2, 3, 4};
  S21Matrix A(2, 2, dataA);
  TestMethodOperation<double>(
      A, [](const S21Matrix& a) { return a.Determinant(); }, -2);
}
TEST(S21MatrixTest, Determinant3) {
  double dataA[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  S21Matrix A(3, 3, dataA);
  TestMethodOperation<double>(
      A, [](const S21Matrix& a) { return a.Determinant(); }, 0);
}
TEST(S21MatrixTest, Determinant4) {
  double dataA[] = {10, 2, 3, 4, 5, 6, 7, 8, 9};
  S21Matrix A(3, 3, dataA);
  TestMethodOperation<double>(
      A, [](const S21Matrix& a) { return a.Determinant(); }, -27);
}
TEST(S21MatrixTest, Determinant5) {
  S21Matrix A(1, 2);
  TestMethodOperationFailure<std::invalid_argument>(
      A, [](const S21Matrix& a) { return a.Determinant(); });
}
TEST(S21MatrixTest, Determinant6) {
  S21Matrix A(1, 2);
  TestMethodOperationFailure<std::invalid_argument>(
      A, [](const S21Matrix& a) { return a.Determinant(); });
}
TEST(S21MatrixTest, Determinant7) {
  double dataA[] = {2, 4, 1, 1, 0, 2, 1, 0, 2, 1, 1, 3, 4, 0, 2, 3};
  S21Matrix A(4, 4, dataA);
  TestMethodOperation<double>(
      A, [](const S21Matrix& a) { return a.Determinant(); }, -26);
}
TEST(S21MatrixTest, Determinant8) {
  double dataA[] = {4.1, 1.2, 1.3, 2.4,  1.5, 1.1, 2.2, -1.3, 1.4,
                    1.5, 3.1, 1.2, 1.3,  1.4, 1.5, 2.1, 1.2,  1.3,
                    4.4, 1.5, 2.1, -1.2, 1.3, 1.4, 5.5};
  S21Matrix A(5, 5, dataA);
  double result = 0.0;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_LT(fabs(108.16 - result), S21Matrix::kEpsilon);
}

TEST(S21MatrixTest, Transpose1) {
  double dataA[] = {0, 1, 2, 3};
  S21Matrix A(2, 2, dataA);
  double dataExpected[] = {0, 2, 1, 3};
  S21Matrix expected(2, 2, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.Transpose(); }, expected);
}
TEST(S21MatrixTest, Transpose2) {
  double dataA[] = {0};
  S21Matrix A(1, 1, dataA);
  double dataExpected[] = {0};
  S21Matrix expected(1, 1, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.Transpose(); }, expected);
}
TEST(S21MatrixTest, Transpose3) {
  double dataA[] = {0, 0, 1, 1};
  S21Matrix A(2, 2, dataA);
  double dataExpected[] = {0, 1, 0, 1};
  S21Matrix expected(2, 2, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.Transpose(); }, expected);
}
TEST(S21MatrixTest, Transpose4) {
  double dataA[] = {0, 0, 1, 1, 1, 2};
  S21Matrix A(2, 3, dataA);
  double dataExpected[] = {0, 1, 0, 1, 1, 2};
  S21Matrix expected(3, 2, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.Transpose(); }, expected);
}
TEST(S21MatrixTest, Transpose5) {
  double dataA[] = {1, 4, 2, 5, 3, 6};
  S21Matrix A(3, 2, dataA);
  double dataExpected[] = {1, 2, 3, 4, 5, 6};
  S21Matrix expected(2, 3, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.Transpose(); }, expected);
}

TEST(S21MatrixTest, Complemet1) {
  double dataA[] = {1, 2, 3, 0, 4, 2, 5, 2, 1};
  S21Matrix A(3, 3, dataA);
  double dataExpected[] = {0, 10, -20, 4, -14, 8, -8, -2, 4};
  S21Matrix expected(3, 3, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.CalcComplements(); }, expected);
}
TEST(S21MatrixTest, Complemet2) {
  double dataA[] = {1, 2, 3, 4};
  S21Matrix A(2, 2, dataA);
  double dataExpected[] = {4, -3, -2, 1};
  S21Matrix expected(2, 2, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.CalcComplements(); }, expected);
}
TEST(S21MatrixTest, Complemet3) {
  S21Matrix A(2, 3);
  S21Matrix result;
  TestMethodOperationFailure<std::invalid_argument>(
      A, [](const S21Matrix& a) { return a.CalcComplements(); });
}

TEST(S21MatrixTest, Inverse1) {
  double dataA[] = {2, 5, 7, 6, 3, 4, 5, -2, -3};
  S21Matrix A(3, 3, dataA);
  double dataExpected[] = {1, -1, 1, -38, 41, -34, 27, -29, 24};
  S21Matrix expected(3, 3, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.InverseMatrix(); }, expected);
}
TEST(S21MatrixTest, Inverse2) {
  double dataA[] = {2.1, 5.2, 6.1, 3.2};
  S21Matrix A(2, 2, dataA);
  double dataExpected[] = {-0.128, 0.208, 0.244, -0.084};
  S21Matrix expected(2, 2, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.InverseMatrix(); }, expected);
}
TEST(S21MatrixTest, Inverse3) {
  double dataA[] = {2, 1, 1, 1, 0, 2, 1, 0, 2, 1, 1, 3, 4, 0, 2, 1};
  S21Matrix A(4, 4, dataA);
  double dataExpected[] = {1.375, -0.5, -0.375, -0.25, 1.25, 0, -0.25, -0.5,
                           -2.5,  1,    0.5,    1,     -0.5, 0, 0.5,   0};
  S21Matrix expected(4, 4, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.InverseMatrix(); }, expected);
}
TEST(S21MatrixTest, Inverse4) {
  double dataA[] = {2, 1, 3, 4, 3, 1, 2, 3, 1};
  S21Matrix A(3, 3, dataA);
  double dataExpected[] = {0,     0.5,   -0.5,  -0.125, -0.25,
                           0.625, 0.375, -0.25, 0.125};
  S21Matrix expected(3, 3, dataExpected);
  TestMethodOperation<S21Matrix>(
      A, [](const S21Matrix& a) { return a.InverseMatrix(); }, expected);
}
TEST(S21MatrixTest, Inverse5) {
  S21Matrix A(1, 2);
  S21Matrix result;
  TestMethodOperationFailure<std::invalid_argument>(
      A, [](const S21Matrix& a) { return a.InverseMatrix(); });
}
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
