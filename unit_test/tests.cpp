#include "tests.hpp"

#include <_stdlib.h>

#include <cstdint>

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
  }
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
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
