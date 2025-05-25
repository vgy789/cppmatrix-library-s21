#include <gtest/gtest.h>

#include <stdexcept>

#include "../s21_matrix_oop.hpp"

namespace {
TEST(S21MatrixTest, DefaultConstructor) {
  S21Matrix m;
  EXPECT_EQ(m.Rows(), 0);
  EXPECT_EQ(m.Cols(), 0);
}

TEST(S21MatrixTest, ParameterizedConstructor) {
  const int N = 100;
  const int M = 100;
  for (int i = -3; i < N; ++i) {
    for (int j = -3; j < M; ++j) {
      if (i <= 0 || j <= 0) {
        ASSERT_THROW(S21Matrix(i, j), std::invalid_argument);
      } else {
        S21Matrix m(i, j);
        EXPECT_EQ(m.Rows(), i);
        EXPECT_EQ(m.Cols(), j);
      }
    }
  }
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
