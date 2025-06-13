#ifndef SRC_S21_MATRIX_OOP_HPP
#define SRC_S21_MATRIX_OOP_HPP

#include <cassert>
#include <cstddef>

class S21Matrix {
  int rows_, cols_;
  double **matrix_;

 public:
  static const double kEpsilon;
  S21Matrix() : rows_(0), cols_(0), matrix_(nullptr){};
  S21Matrix(int rows, int cols);
  S21Matrix(int rows, int cols, const double array[]);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other);
  ~S21Matrix();

  inline int Rows() const { return rows_; }
  inline int Cols() const { return cols_; }
  inline double Epsilon() const { return kEpsilon; }
  inline int Length() const { return Rows() * Cols(); }
  inline bool IsSquare() const { return Rows() == Cols(); }
  void Print() const;
  double Determinant() const;

  S21Matrix Transpose() const;

  void SumMatrix(const S21Matrix &other);
  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix operator+(const S21Matrix &other) const;
  void SubMatrix(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix operator-(const S21Matrix &other) const;
  void MulNumber(const double number);
  S21Matrix &operator*=(const double number);
  S21Matrix operator*(const double number) const;
  void MulMatrix(const S21Matrix &other);
  S21Matrix &operator*=(const S21Matrix &other);
  S21Matrix operator*(const S21Matrix &other) const;
  bool EqMatrix(const S21Matrix &other) const;
  bool operator==(const S21Matrix &other) const;

  S21Matrix &operator=(const S21Matrix &other);
  S21Matrix &operator=(S21Matrix &&other);
  double &operator()(int row, int col);
  const double &operator()(int row, int col) const;

 private:
  void AllocateMatrix();
  void InitializeMatrix(const double *);
  void DeallocateMatrix();

  double DetRecursive() const;
  S21Matrix Submatrix(int row, int col) const;
};

#endif  // SRC_S21_MATRIX_OOP_HPP
