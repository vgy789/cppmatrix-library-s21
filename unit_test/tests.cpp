#include "tests.hpp"

#include <cstdint>
#include <stdexcept>

#include "../s21_matrix_oop.hpp"
#include "gtest/gtest.h"

enum { matrix_in_array = 15 };

int g_new_fail_on_count = -1;
int g_new_call_count = 0;
void* operator new(size_t size) {
  ++g_new_call_count;
  if (g_new_call_count == g_new_fail_on_count) {
    throw std::bad_alloc();
  }
  return malloc(size);
}
void* operator new[](size_t size) {
  ++g_new_call_count;
  if (g_new_call_count == g_new_fail_on_count) {
    throw std::bad_alloc();
  }
  return malloc(size);
}
void operator delete(void* memory) noexcept { free(memory); }
void operator delete[](void* memory) noexcept { free(memory); }

namespace {
TEST(S21MatrixTest, ExceptionSafetyGuarantee) {
  g_new_call_count = 0;
  g_new_fail_on_count = 1;
  ASSERT_THROW(S21Matrix m(INT32_MAX, INT32_MAX), std::bad_alloc);
  g_new_fail_on_count = -1;
}

TEST(S21MatrixTest, Constructor_Default) {
  S21Matrix m;
  EXPECT_EQ(m.Rows(), 0);
  EXPECT_EQ(m.Cols(), 0);
}

TEST(S21MatrixTest, Constructor_Parameterized) {
  const int N = 55;
  const int M = 57;
  for (int i = -3; i < N; ++i) {
    for (int j = -3; j < M; ++j) {
      check_sizes(i, j);
      check_zero_values(i, j);
    }
  }
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

void sum_tc(const S21Matrix A, const S21Matrix B, const int is_success,
            const S21Matrix expected = S21Matrix()) {
  S21Matrix copy_for_plus_equal = A;
  S21Matrix copy_for_sum_matrix = A;
  S21Matrix result_plus;
  if (is_success) {
    // operator+=
    ASSERT_NO_THROW(copy_for_plus_equal += B);
    EXPECT_TRUE(copy_for_plus_equal == expected);
    // operator+
    ASSERT_NO_THROW(result_plus = A + B);
    EXPECT_TRUE(result_plus == expected);
    // SumMatrix
    ASSERT_NO_THROW(copy_for_sum_matrix.SumMatrix(B));
    EXPECT_TRUE(copy_for_sum_matrix == expected);
  } else {
    // operator+=
    ASSERT_THROW(copy_for_plus_equal += B, std::invalid_argument);
    // operator+
    ASSERT_THROW(A + B, std::invalid_argument);
    // SumMatrix
    ASSERT_THROW(copy_for_sum_matrix.SumMatrix(B), std::invalid_argument);
  }
}

TEST(S21MatrixTest, Sum1) {
  const S21Matrix A(2, 2, (double[]){2, 2, 1, 1});
  const S21Matrix B(2, 2, (double[]){2, 2, 1, 1});
  const S21Matrix expected(2, 2, (double[]){4, 4, 2, 2});
  sum_tc(A, B, true, expected);
}
TEST(S21MatrixTest, Sum2) {
  const S21Matrix A(1, 1, (double[]){5.1f});
  const S21Matrix B(1, 1, (double[]){2.2f});
  const S21Matrix expected(1, 1, (double[]){7.3f});
  sum_tc(A, B, true, expected);
}
TEST(S21MatrixTest, Sum3) {
  const S21Matrix A(1, 1, (double[]){5.1f});
  const S21Matrix B(1, 2, (double[]){2.2f, 9});
  sum_tc(A, B, false);
}
TEST(S21MatrixTest, Sum4) {
  const S21Matrix A(1, 1);
  const S21Matrix B(2, 2);
  sum_tc(A, B, false);
}

void sub_tc(const S21Matrix A, const S21Matrix B, const int is_success,
            const S21Matrix expected = S21Matrix()) {
  S21Matrix copy_for_minus_equal = A;
  S21Matrix copy_for_sub_matrix = A;
  S21Matrix result_plus;
  if (is_success) {
    // operator-=
    ASSERT_NO_THROW(copy_for_minus_equal -= B);
    EXPECT_TRUE(copy_for_minus_equal == expected);
    // operator-
    ASSERT_NO_THROW(result_plus = A - B);
    EXPECT_TRUE(result_plus == expected);
    // SubMatrix
    ASSERT_NO_THROW(copy_for_sub_matrix.SubMatrix(B));
    EXPECT_TRUE(copy_for_sub_matrix == expected);
  } else {
    // operator-=
    ASSERT_THROW(copy_for_minus_equal -= B, std::invalid_argument);
    // operator-
    ASSERT_THROW(A - B, std::invalid_argument);
    // SubMatrix
    ASSERT_THROW(copy_for_sub_matrix.SubMatrix(B), std::invalid_argument);
  }
}

TEST(S21MatrixTest, Sub1) {
  const S21Matrix A(2, 2, (double[]){4, 4, 2, 2});
  const S21Matrix B(2, 2, (double[]){2, 2, 1, 1});
  const S21Matrix expected(2, 2, (double[]){2, 2, 1, 1});
  sub_tc(A, B, true, expected);
}
TEST(S21MatrixTest, Sub2) {
  const S21Matrix A(1, 1, (double[]){7.3});
  const S21Matrix B(1, 1, (double[]){2.2});
  const S21Matrix expected(1, 1, (double[]){5.1});
  sub_tc(A, B, true, expected);
}
TEST(S21MatrixTest, Sub3) {
  const S21Matrix A(1, 1, (double[]){5.1f});
  const S21Matrix B(1, 2, (double[]){2.2f, 9});
  sub_tc(A, B, false);
}
TEST(S21MatrixTest, Sub4) {
  const S21Matrix A(1, 1);
  const S21Matrix B(2, 2);
  sub_tc(A, B, false);
}

void mul_number_tc(const S21Matrix A, const int B, const int is_success,
                   const S21Matrix expected = S21Matrix()) {
  S21Matrix copy_for_mul_equal = A;
  S21Matrix copy_for_mul_matrix = A;
  S21Matrix result_mulply;
  if (is_success) {
    // operator*=
    ASSERT_NO_THROW(copy_for_mul_equal *= B);
    EXPECT_TRUE(copy_for_mul_equal == expected);
    // operator*
    ASSERT_NO_THROW(result_mulply = A * B);
    EXPECT_TRUE(result_mulply == expected);
    // MulNumber
    ASSERT_NO_THROW(copy_for_mul_matrix.MulNumber(B));
    EXPECT_TRUE(copy_for_mul_matrix == expected);
  } else {
    // operator*=
    ASSERT_THROW(copy_for_mul_equal *= B, std::invalid_argument);
    // operator*
    ASSERT_THROW(A * B, std::invalid_argument);
    // MulNumber
    ASSERT_THROW(copy_for_mul_matrix.MulNumber(B), std::invalid_argument);
  }
}

TEST(S21MatrixTest, MulNumber1) {
  const S21Matrix A(2, 2, (double[]){4, 4, 2, 2});
  const int B = 1;
  const S21Matrix expected(2, 2, (double[]){4, 4, 2, 2});
  mul_number_tc(A, B, true, expected);
}
TEST(S21MatrixTest, MulNumber2) {
  const S21Matrix A(2, 2, (double[]){1.1f, -2.12454636f, 0.0, 100.0});
  const int B = -3;
  const S21Matrix expected(2, 2, (double[]){-3.3f, 6.37363908f, 0.0, -300.0});
  mul_number_tc(A, B, true, expected);
}

void mul_matrix_tc(const S21Matrix A, const S21Matrix B, const int is_success,
                   const S21Matrix expected = S21Matrix()) {
  S21Matrix copy_for_mul_equal = A;
  S21Matrix copy_for_mul_matrix = A;
  S21Matrix result_mulply;
  if (is_success) {
    // operator*=
    ASSERT_NO_THROW(copy_for_mul_equal *= B);
    EXPECT_TRUE(copy_for_mul_equal == expected);
    // operator*
    ASSERT_NO_THROW(result_mulply = A * B);
    EXPECT_TRUE(result_mulply == expected);
    // MulMatrix
    ASSERT_NO_THROW(copy_for_mul_matrix.MulMatrix(B));
    EXPECT_TRUE(copy_for_mul_matrix == expected);
  } else {
    // operator*=
    ASSERT_THROW(copy_for_mul_equal *= B, std::invalid_argument);
    // operator*
    ASSERT_THROW(A * B, std::invalid_argument);
    // MulMatrix
    ASSERT_THROW(copy_for_mul_matrix.MulMatrix(B), std::invalid_argument);
  }
}

TEST(S21MatrixTest, MulMatrix1) {
  const S21Matrix A(3, 2, (double[]){1, 4, 2, 5, 3, 6});
  const S21Matrix B(2, 3, (double[]){1, -1, 1, 2, 3, 4});
  const S21Matrix expected(3, 3, (double[]){9, 11, 17, 12, 13, 22, 15, 15, 27});
  mul_matrix_tc(A, B, true, expected);
}
TEST(S21MatrixTest, MulMatrix2) {
  const S21Matrix A(3, 3, (double[]){1, 2, 3, 4, 5, 6, 7, 8, 9});
  const S21Matrix B(3, 3, (double[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
  const S21Matrix expected(3, 3, (double[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
  mul_matrix_tc(A, B, true, expected);
}
TEST(S21MatrixTest, MulMatrix3) {
  const S21Matrix A(1, 1);
  const S21Matrix B(3, 4);
  mul_matrix_tc(A, B, false);
}
TEST(S21MatrixTest, MulMatrix4) {
  const S21Matrix A(2, 3, (double[]){2.2, 2.0, 3.0, 0.0, 4.0, 1.0});
  const S21Matrix B(
      3, 4,
      (double[]){0.0, 3.0, 4.0, 5.0, 2.0, 5.0, 6.0, 7.0, 8.0, 7.0, 5.0, 43.0});
  const S21Matrix expected(
      2, 4, (double[]){28.0, 37.6, 35.8, 154.0, 16.0, 27.0, 29.0, 71.0});
  mul_matrix_tc(A, B, true, expected);
}

TEST(S21MatrixTest, Determinant1) {
  S21Matrix A(1, 1, (double[]){-48});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_EQ(result, -48);
}
TEST(S21MatrixTest, Determinant2) {
  S21Matrix A(2, 2, (double[]){1, 2, 3, 4});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_EQ(result, -2);
}
TEST(S21MatrixTest, Determinant3) {
  S21Matrix A(3, 3, (double[]){1, 2, 3, 4, 5, 6, 7, 8, 9});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_EQ(result, 0);
}
TEST(S21MatrixTest, Determinant4) {
  S21Matrix A(3, 3, (double[]){10, 2, 3, 4, 5, 6, 7, 8, 9});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_EQ(result, -27);
}
TEST(S21MatrixTest, Determinant5) {
  S21Matrix A(1, 2);
  double result;
  ASSERT_THROW(result = A.Determinant(), std::invalid_argument);
}

TEST(S21MatrixTest, Determinant6) {
  S21Matrix A(1, 2);
  double result;
  ASSERT_THROW(result = A.Determinant(), std::invalid_argument);
}

TEST(S21MatrixTest, Determinant7) {
  S21Matrix A(4, 4, (double[]){2, 4, 1, 1, 0, 2, 1, 0, 2, 1, 1, 3, 4, 0, 2, 3});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_EQ(result, -26);
}

TEST(S21MatrixTest, Determinant8) {
  S21Matrix A(5, 5, (double[]){4.1, 1.2, 1.3, 2.4,  1.5, 1.1, 2.2, -1.3, 1.4,
                               1.5, 3.1, 1.2, 1.3,  1.4, 1.5, 2.1, 1.2,  1.3,
                               4.4, 1.5, 2.1, -1.2, 1.3, 1.4, 5.5});
  double result;
  ASSERT_NO_THROW(result = A.Determinant());
  EXPECT_LT(fabs(108.16 - result), S21Matrix::kEpsilon);
}

TEST(S21MatrixTest, Transpose1) {
  S21Matrix A(2, 2, (double[]){0, 1, 2, 3});
  S21Matrix expected(2, 2, (double[]){0, 2, 1, 3});
  S21Matrix result;
  ASSERT_NO_THROW(result = A.Transpose());
  EXPECT_EQ(result, expected);
}
TEST(S21MatrixTest, Transpose2) {
  S21Matrix A(1, 1, (double[]){0});
  S21Matrix expected(1, 1, (double[]){0});
  S21Matrix result;
  ASSERT_NO_THROW(result = A.Transpose());
  EXPECT_EQ(result, expected);
}
TEST(S21MatrixTest, Transpose3) {
  S21Matrix A(2, 2, (double[]){0, 0, 1, 1});
  S21Matrix expected(2, 2, (double[]){0, 1, 0, 1});
  S21Matrix result;
  ASSERT_NO_THROW(result = A.Transpose());
  EXPECT_EQ(result, expected);
}
TEST(S21MatrixTest, Transpose4) {
  S21Matrix A(2, 3, (double[]){0, 0, 1, 1, 1, 2});
  S21Matrix expected(3, 2, (double[]){0, 1, 0, 1, 1, 2});
  S21Matrix result;
  ASSERT_NO_THROW(result = A.Transpose());
  EXPECT_EQ(result, expected);
}
TEST(S21MatrixTest, Transpose5) {
  S21Matrix A(3, 2, (double[]){1, 4, 2, 5, 3, 6});
  S21Matrix expected(2, 3, (double[]){1, 2, 3, 4, 5, 6});
  S21Matrix result;
  ASSERT_NO_THROW(result = A.Transpose());
  EXPECT_EQ(result, expected);
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
