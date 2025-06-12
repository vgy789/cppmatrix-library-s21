#ifndef UNIT_TEST_TESTS_HPP
#define UNIT_TEST_TESTS_HPP

#include <gtest/gtest.h>

#include "../s21_matrix_oop.hpp"

void random_matrix(S21Matrix& matrix);
void check_sizes(int i, int j);
void check_zero_values(int i, int j);

#endif  // UNIT_TEST_TESTS_HPP
