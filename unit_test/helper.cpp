#include "tests.hpp"

void random_matrix(S21Matrix& matrix) {
  const int M = 1 + random() % 10;
  const int N = 1 + random() % 10;
  matrix = S21Matrix(M, N);
  for (int i = 0; i < matrix.Rows(); ++i) {
    for (int j = 0; j < matrix.Cols(); ++j) {
      matrix(i, j) = random();
    }
  }
}

void check_sizes(int i, int j) {
  if (i <= 0 || j <= 0) {
    ASSERT_THROW(S21Matrix(i, j), std::invalid_argument);
  } else {
    S21Matrix matrix(i, j);
    EXPECT_EQ(matrix.Rows(), i);
    EXPECT_EQ(matrix.Cols(), j);
  }
}

void check_zero_values(int i, int j) {
  if (i <= 0 || j <= 0) {
    ASSERT_THROW(S21Matrix(i, j), std::invalid_argument);
  } else {
    S21Matrix matrix(i, j);
    for (int k = 0; k < i; ++k) {
      for (int n = 0; n < j; ++n) {
        EXPECT_EQ(matrix(k, n), 0);
      }
    }
  }
}
