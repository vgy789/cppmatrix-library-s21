#include "s21_matrix_oop.hpp"

void S21Matrix::AllocateMatrix() {
  matirx_ = new double*[rows_];
  for (size_t i = 0; i < rows_; ++i) {
    matirx_[i] = nullptr;
  }

  try
  {
    for (int i = 0; i < rows_; ++i) {
      matirx_[i] = new double[cols_];
    }
  }
  catch(const std::bad_alloc& e)
  {
    DeallocateMatrix();
    throw;
  }
}

void S21Matrix::InitializeMatrix() {
  if (matirx_ == nullptr) return;

  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      matirx_[i][j] = 0;
    }
  }
}

void S21Matrix::DeallocateMatrix() {
  if (matirx_ == nullptr) return;

  for (size_t i = 0; i < rows_; ++i) {
    if (matirx_[i] != nullptr)
    {
      delete[] matirx_[i];
    }
    
  }
  delete[] matirx_;
  matirx_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols): rows_(rows), cols_(cols), matirx_(nullptr) {
  if (rows_ <= 0 || cols_ <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive.");
  }
  AllocateMatrix();
  InitializeMatrix();
}

S21Matrix::S21Matrix(const S21Matrix& other): rows_(0), cols_(0), matirx_(nullptr) {
  if (other.rows_ > 0 && other.cols_ > 0) {
    rows_ = other.rows_;
    cols_ = other.cols_;

    for (size_t i = 0; i < other.rows_; ++i)
    {
      for (size_t j = 0; j < other.cols_; ++j) {
        matirx_[i][j] = other.matirx_[i][j];        
      }
    }
  }
}

S21Matrix::~S21Matrix() {
  DeallocateMatrix();
  rows_ = 0;
  cols_ = 0;
}

double& S21Matrix::operator()(int row, int col) {
  if (matirx_ == nullptr || row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Matrix index out of range or matrix not allocated.");
  }
  return matirx_[row][col];
}

const double& S21Matrix::operator()(int row, int col) const {
  if (matirx_ == nullptr || row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Matrix index out of range or matrix not allocated.");
  }
  return matirx_[row][col];
}
