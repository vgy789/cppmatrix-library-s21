#include "s21_matrix_oop.hpp"

#include <cstddef>

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
 * @brief Заполняет все элементы матрицы нулями
 *
 * @pre Объект S21Matrix должен быть правильно создан с корректными значениями
 * rows_ и cols_
 */
void S21Matrix::InitializeMatrix() {
  if (matrix_ == nullptr) {
    return;
  }

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = 0;
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
  InitializeMatrix();
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
      matrix_[i][j] = other.matrix_[i][j];
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
        matrix_[i][j] = other.matrix_[i][j];
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
