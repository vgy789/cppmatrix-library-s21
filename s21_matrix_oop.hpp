#ifndef SRC_S21_MATRIX_OOP_HPP
#define SRC_S21_MATRIX_OOP_HPP

#include <stdexcept>

class S21Matrix {
  int rows_, cols_;
  double **matrix_;

 public:
  S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) {};
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other);
  ~S21Matrix();

  int Rows() { return rows_; }
  int Cols() { return cols_; }

  S21Matrix &operator=(const S21Matrix &other);
  S21Matrix &operator=(S21Matrix &&other);
  double &operator()(int row, int col);
  const double &operator()(int row, int col) const;

 private:
  void AllocateMatrix();
  void InitializeMatrix();
  void DeallocateMatrix();
};

#endif  // SRC_S21_MATRIX_OOP_HPP
