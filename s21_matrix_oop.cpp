#include "s21_matrix_oop.hpp"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <stdexcept>

const double S21Matrix::kEpsilon = 1.0e-6;

/**
 * @brief Выделяет память для матрицы
 *
 * @throw std::bad_alloc если не удалось выделить память
 */
void S21Matrix::AllocateMatrix() {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = nullptr;
  }

  try {
    for (int i = 0; i < rows_; ++i) {
      matrix_[i] = new double[cols_];
    }
  } catch (const std::bad_alloc &e) {
    DeallocateMatrix();
    throw;
  }
}

/**
 * @brief Заполняет все элементы матрицы массивом значений или нулями, если
 * массив не передан
 *
 * @pre Объект S21Matrix должен быть правильно создан с корректными значениями
 * rows_ и cols_
 * @pre Массив значений должен содержать количество элементов соответсвующее
 * размерности S21Matrix
 * @details Если array == nullptr, все элементы матрицы инициализируются нулями.
 */
void S21Matrix::InitializeMatrix(const double *array) {
  if (matrix_ == nullptr) {
    return;
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (array) {
        (*this)(i, j) = array[i * cols_ + j];
      } else {
        (*this)(i, j) = 0.0;
      }
    }
  }
}

/**
 * @brief Освобождает память выделенную для матрицы
 */
void S21Matrix::DeallocateMatrix() {
  if (matrix_ == nullptr) {
    return;
  }

  for (int i = 0; i < rows_; ++i) {
    if (matrix_[i] != nullptr) {
      delete[] matrix_[i];
    }
  }
  delete[] matrix_;
  matrix_ = nullptr;
}

/**
 * @brief Конструктор для создания матрицы с заданными размерами
 * @param rows Количество строк в матрице
 * @param cols Количество столбцов в матрице
 * @throw std::invalid_argument Если количество строк или столбцов не является
 * положительным числом
 * @details Создает объект матрицы с указанным количеством строк и столбцов.
 *          Выделяет память для матрицы и инициализирует все элементы.
 *          Элементы матрицы инициализируются нулями.
 */
S21Matrix::S21Matrix(int rows, int cols)
    : rows_(0), cols_(0), matrix_(nullptr) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive.");
  }
  rows_ = rows;
  cols_ = cols;
  AllocateMatrix();
  InitializeMatrix(nullptr);
}

/**
 * @brief Конструктор для создания матрицы с заданными размерами и элементами
 * @param rows Количество строк в матрице
 * @param cols Количество столбцов в матрице
 * @param array Массив элементов матрицы
 * @throw std::invalid_argument Если количество строк или столбцов не является
 * положительным числом
 * @details Создаёт объект матрицы с указанным количеством строк и столбцов.
 *          Выделяет память для матрицы и инициализирует все элементы
 *          Элементы матрицы инициализируются массивом.
 */
S21Matrix::S21Matrix(int rows, int cols, const double *array)
    : S21Matrix(rows, cols) {
  InitializeMatrix(array);
}

/**
 * @brief Конструктор копирования для класса S21Matrix
 *
 * @param other ссылка на исходный объект S21Matrix для перемещения
 */
S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(0), cols_(0), matrix_(nullptr) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  if (other.matrix_ == nullptr) {
    matrix_ = nullptr;
    return;
  }

  AllocateMatrix();
  for (int i = 0; i < other.rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      (*this)(i, j) = other(i, j);
    }
  }
}

/**
 * @brief Конструктор перемещения для класса S21Matrix
 *
 * @param other R-value ссылка на исходный объект S21Matrix для перемещения
 */
S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

/**
 * @brief Деструктор объекта S21Matrix
 */
S21Matrix::~S21Matrix() { DeallocateMatrix(); }

/**
 * @brief Перегруженный оператор () для доступа к элементам матрицы
 *
 * @param row Индекс строки элемента для доступа
 * @param col Индекс столбца элемента для доступа
 * @return Константная ссылка на элемент в указанной позиции
 * @throw std::out_of_range если индексы выходят за пределы или матрица не
 выделена
 */
double &S21Matrix::operator()(int row, int col) {
  if (matrix_ == nullptr || row < 0 || row >= rows_ || col < 0 ||
      col >= cols_) {
    throw std::out_of_range(
        "Matrix index out of range or matrix not allocated.");
  }
  return matrix_[row][col];
}

/**
 * @brief Перегруженный константный оператор () для доступа к элементам
 матрицы
 *
 * @param row Индекс строки элемента для доступа
 * @param col Индекс столбца элемента для доступа
 * @return Константная ссылка на элемент в указанной позиции
 * @throw std::out_of_range если индексы выходят за пределы или матрица не
 выделена
 */
const double &S21Matrix::operator()(int row, int col) const {
  if (matrix_ == nullptr || row < 0 || row >= rows_ || col < 0 ||
      col >= cols_) {
    throw std::out_of_range(
        "Matrix index out of range or matrix not allocated.");
  }
  return matrix_[row][col];
}

/**
 * @brief Перегрузка оператора присваивания для класса S21Matrix
 *
 * @param other ссылка на исходный объект S21Matrix для перемещения
 * @return Ссылка на скопированный объект
 */
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this == &other) {
    return *this;
  }

  DeallocateMatrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = nullptr;

  if (rows_ > 0 && cols_ > 0) {
    AllocateMatrix();
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
  }
  return *this;
}

/**
 * @brief Оператор перемещающего присваивания для класса S21Matrix
 *
 * @param other R-value ссылка на исходный объект S21Matrix для перемещения
 * @return Ссылка на скопированный объект
 */
S21Matrix &S21Matrix::operator=(S21Matrix &&other) {
  DeallocateMatrix();

  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;

  other.matrix_ = nullptr;
  return *this;
}

void S21Matrix::Print() const {
  for (int i = 0; i < this->Rows(); ++i) {
    if (i != 0) {
      printf("\n");
    }
    for (int j = 0; j < this->Cols(); ++j) {
      if (j != 0) {
        printf(" ");
      }
      printf("%lf", (*this)(i, j));
    }
  }
}

S21Matrix S21Matrix::Submatrix(int row, int col) const {
  bool row_passed = false;
  bool column_passed = false;
  S21Matrix submatrix(this->Rows() - 1, this->Rows() - 1);

  for (int i = 0; i < this->Rows(); ++i) {
    if (i == row) {
      continue;
    }
    row_passed = (i > row);
    for (int j = 0; j < this->Cols(); ++j) {
      if (j == col) {
        continue;
      }
      column_passed = (j > col);
      submatrix(i - row_passed, j - column_passed) = (*this)(i, j);
    }
  }
  return submatrix;
}

double S21Matrix::DetRecursive() const {
  double det = 0.0;
  if (rows_ == 1) {
    return (*this)(0, 0);
  }
  if (rows_ == 2) {
    return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
  }
  // https://ru.onlinemschool.com/math/library/matrix/minors/#h2
  // разложение по первой строке
  int sign = +1;
  for (int k = 0; k < rows_; ++k) {
    S21Matrix submatrix = this->Submatrix(0, k);
    det += sign * (*this)(0, k) * submatrix.DetRecursive();
    sign = -sign;
  }

  return det;
}

double S21Matrix::Determinant() const {
  if (!IsSquare()) {
    throw std::invalid_argument(
        "Determinant is only defined for square matrices.");
  }

  return this->DetRecursive();
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix transpose(Cols(), Rows());
  for (int i = 0; i < Rows(); ++i) {
    for (int j = 0; j < Cols(); ++j) {
      transpose(j, i) = (*this)(i, j);
    }
  }
  return transpose;
}

bool S21Matrix::EqMatrix(const S21Matrix &other) const {

bool S21Matrix::operator==(const S21Matrix &other) const {
  if (this->Cols() != other.Cols() || this->Rows() != other.Rows()) {
    return false;
  }

  for (int i = 0; i < this->Rows(); ++i) {
    for (int j = 0; j < this->Cols(); ++j) {
      if (fabs((*this)(i, j) - other(i, j)) > kEpsilon) {
        return false;
      }
    }
  }
  return true;
}

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  return *this == other;
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  const bool is_equeal_size =
      (this->Rows() == other.Rows() && this->Cols() == other.Cols());
  if (!is_equeal_size) {
    throw std::invalid_argument(
        "Matrices must have the same dimensions for addition.");
  }
  for (int i = 0; i < this->Rows(); ++i) {
    for (int j = 0; j < this->Cols(); ++j) {
      (*this)(i, j) = (*this)(i, j) + other(i, j);
    }
  }
  return *this;
}

void S21Matrix::SumMatrix(const S21Matrix &other) { *this += other; }

S21Matrix S21Matrix::operator+(const S21Matrix &other) const {
  S21Matrix result(*this);
  result += other;
  return result;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  const bool is_equeal_size =
      (this->Rows() == other.Rows() && this->Cols() == other.Cols());
  if (!is_equeal_size) {
    throw std::invalid_argument(
        "Matrices must have the same dimensions for addition.");
  }
  for (int i = 0; i < this->Rows(); ++i) {
    for (int j = 0; j < this->Cols(); ++j) {
      (*this)(i, j) = (*this)(i, j) - other(i, j);
    }
  }
  return *this;
}

void S21Matrix::SubMatrix(const S21Matrix &other) { *this -= other; }

S21Matrix S21Matrix::operator-(const S21Matrix &other) const {
  S21Matrix result(*this);
  result -= other;
  return result;
}

S21Matrix &S21Matrix::operator*=(const double number) {
  for (int i = 0; i < this->Rows(); ++i) {
    for (int j = 0; j < this->Cols(); ++j) {
      (*this)(i, j) = (*this)(i, j) * number;
    }
  }
  return *this;
}

void S21Matrix::MulNumber(const double number) { *this *= number; }

S21Matrix S21Matrix::operator*(const double number) const {
  S21Matrix result(*this);
  result *= number;
  return result;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  if (this->Cols() != other.Rows()) {
    throw std::invalid_argument(
        "Matrices must have compatible dimensions for multiplication.");
  }

  S21Matrix result(this->Rows(), other.Cols());
  for (int i = 0; i < result.Rows(); ++i) {
    for (int j = 0; j < result.Cols(); ++j) {
      for (int k = 0; k < this->Cols(); ++k) {
        result(i, j) += (*this)(i, k) * other(k, j);
      }
    }
  }
  *this = std::move(result);
  return *this;
}

void S21Matrix::MulMatrix(const S21Matrix &other) { *this *= other; }

S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21Matrix result(*this);
  result *= other;
  return result;
}
