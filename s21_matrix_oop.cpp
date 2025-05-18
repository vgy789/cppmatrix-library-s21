#include "s21_matrix_oop.hpp"

void S21Matrix::AllocateMatrix() {
  matrix_ = new double *[rows_];
  for (size_t i = 0; i < rows_; ++i) {
    matrix_[i] = nullptr;
  }

  try {
    for (size_t i = 0; i < rows_; ++i) {
      matrix_[i] = new double[cols_];
    }
  } catch (const std::bad_alloc &e) {
    DeallocateMatrix();
    throw;
  }
}

void S21Matrix::InitializeMatrix() {
  if (matrix_ == nullptr) {
    return;
  }

  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      matrix_[i][j] = 0;
    }
  }
}

void S21Matrix::DeallocateMatrix() {
  if (matrix_ == nullptr) {
    return;
  }

  for (size_t i = 0; i < rows_; ++i) {
    if (matrix_[i] != nullptr) {
      delete[] matrix_[i];
    }
  }
  delete[] matrix_;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols)
    : rows_(rows), cols_(cols), matrix_(nullptr) {
  if (rows_ <= 0 || cols_ <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive.");
  }
  AllocateMatrix();
  InitializeMatrix();
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(0), cols_(0), matrix_(nullptr) {
  if (other.rows_ > 0 && other.cols_ > 0) {
    rows_ = other.rows_;
    cols_ = other.cols_;

    for (size_t i = 0; i < other.rows_; ++i) {
      for (size_t j = 0; j < other.cols_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
}

S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() {
  DeallocateMatrix();
  rows_ = 0;
  cols_ = 0;
}

double &S21Matrix::operator()(int row, int col) {
  if (matrix_ == nullptr || row < 0 || (unsigned)row >= rows_ || col < 0 ||
  (unsigned)col >= cols_) {
    throw std::out_of_range(
        "Matrix index out of range or matrix not allocated.");
  }
  return matrix_[row][col];
}

const double &S21Matrix::operator()(int row, int col) const {
  if (matrix_ == nullptr || row < 0 || (unsigned)row >= rows_ || col < 0 ||
  (unsigned)col >= cols_) {
    throw std::out_of_range(
        "Matrix index out of range or matrix not allocated.");
  }
  return matrix_[row][col];
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this == &other) {
    return *this;
  }

  S21Matrix temp(other);

  DeallocateMatrix();
  matrix_ = temp.matrix_;
  rows_ = temp.rows_;
  cols_ = temp.cols_;

  temp.matrix_ = nullptr;

  return *this;
}

S21Matrix &S21Matrix::operator=(S21Matrix &&other) {
  DeallocateMatrix();

  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;

  other.matrix_ = nullptr;
  return *this;
}
