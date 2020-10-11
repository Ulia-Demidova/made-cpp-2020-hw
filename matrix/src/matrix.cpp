#include "matrix.h"
#include <cmath>
using namespace task;


Matrix::Matrix() : rows(1), cols(1) {
  ptr = new double *[1];
  ptr[0] = new double[1];
  ptr[0][0] = 1.;
}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
  ptr = new double *[rows];
  for (size_t i = 0; i < rows; ++i) {
    ptr[i] = new double[cols];
  }
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (i == j)
        ptr[i][j] = 1.;
      else
        ptr[i][j] = 0.;
    }
  }
}

Matrix::Matrix(const Matrix &copy) : rows(copy.rows), cols(copy.cols) {
  ptr = new double *[rows];
  for (size_t i = 0; i < rows; ++i) {
    ptr[i] = new double[cols];
  }
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      ptr[i][j] = copy.ptr[i][j];
    }
  }
}

Matrix &Matrix::operator=(const Matrix &a) {
  if (&a == this) {
    return *this;
  }
  for (size_t i = 0; i < rows; ++i) {
    delete[] ptr[i];
  }
  delete[] ptr;

  rows = a.rows;
  cols = a.cols;

  ptr = new double *[rows];
  for (size_t i = 0; i < rows; ++i) {
    ptr[i] = new double[cols];
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      ptr[i][j] = a.ptr[i][j];
    }
  }
  return *this;
}

Matrix::~Matrix() {
  for (size_t i = 0; i < rows; ++i) {
    delete[] ptr[i];
  }
  delete[] ptr;
}

double &Matrix::get(size_t row, size_t col) {
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    throw OutOfBoundsException();
  return ptr[row][col];
}

const double &Matrix::get(size_t row, size_t col) const {
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    throw OutOfBoundsException();
  return ptr[row][col];
}

void Matrix::set(size_t row, size_t col, const double &value) {
  if (row < 0 || row >= rows || col < 0 || col >= cols)
    throw OutOfBoundsException();
  ptr[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  Matrix new_matrix(new_rows, new_cols);
  for (size_t i = 0; i < new_rows; ++i) {
    for (size_t j = 0; j < new_cols; ++j) {
      if (i >= rows || j >= cols)
        new_matrix[i][j] = 0.;
      else
        new_matrix[i][j] = ptr[i][j];
    }
  }
  *this = new_matrix;
}

double *Matrix::operator[](size_t row) { return ptr[row]; }

const double *Matrix::operator[](size_t row) const { return ptr[row]; }

Matrix &Matrix::operator+=(const Matrix &a) {
  if (rows != a.rows || cols != a.cols)
    throw SizeMismatchException();
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      ptr[i][j] += a.ptr[i][j];
    }
  }
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &a) {
  if (rows != a.rows || cols != a.cols)
    throw SizeMismatchException();
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      ptr[i][j] -= a.ptr[i][j];
    }
  }
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &a) {
  if (cols != a.rows)
    throw SizeMismatchException();
  Matrix result(rows, a.cols);
  double sum;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < a.cols; ++j) {
      sum = 0.;
      for (size_t k = 0; k < cols; ++k) {
        sum += ptr[i][k] * a.ptr[k][j];
      }
      result.ptr[i][j] = sum;
    }
  }
  *this = result;
  return *this;
}

Matrix &Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      ptr[i][j] *= number;
    }
  }
  return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
  Matrix result = *this;
  return result += a;
}

Matrix Matrix::operator-(const Matrix &a) const {
  Matrix result = *this;
  return result -= a;
}

Matrix Matrix::operator*(const Matrix &a) const {
  Matrix result = *this;
  return result *= a;
}

Matrix Matrix::operator*(const double &a) const {
  Matrix result = *this;
  return result *= a;
}

Matrix Matrix::operator-() const {
  Matrix result = *this;
  return result *= -1;
}

Matrix Matrix::operator+() const { return *this; }

double Matrix::det() const {
  if (rows != cols)
    throw SizeMismatchException();
  if (rows == 1)
    return ptr[0][0];
  int sign = 1;
  double sum = 0;
  for (size_t j = 0; j < cols; ++j) {
    sum += ptr[0][j] * sign * this->minor(j).det();
    sign *= -1;
  }
  return sum;
}

Matrix Matrix::minor(size_t column) const { // to private section ???
  Matrix result(rows - 1, cols - 1);
  for (size_t i = 1; i < rows; ++i) {
    for (size_t j = 0; j < column; ++j) {
      result[i - 1][j] = ptr[i][j];
    }
    for (size_t j = column + 1; j < cols; ++j) {
      result[i - 1][j - 1] = ptr[i][j];
    }
  }
  return result;
}

void Matrix::transpose() {
  Matrix transposed = this->transposed();
  *this = transposed;
}

Matrix Matrix::transposed() const {
  Matrix result(cols, rows);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      result[j][i] = ptr[i][j];
    }
  }
  return result;
}

double Matrix::trace() const {
  if (rows != cols)
    throw SizeMismatchException();
  double sum = 0;
  for (size_t i = 0; i < rows; ++i)
    sum += ptr[i][i];
  return sum;
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> vec;
  for (size_t i = 0; i < cols; ++i) {
    vec.push_back(ptr[row][i]);
  }
  return vec;
}

std::vector<double> Matrix::getColumn(size_t column) {
  std::vector<double> vec;
  for (size_t i = 0; i < rows; ++i) {
    vec.push_back(ptr[i][column]);
  }
  return vec;
}

bool Matrix::operator==(const Matrix &a) const {
  if (rows != a.rows || cols != a.cols)
    return false;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (abs(ptr[i][j] - a[i][j]) > EPS)
        return false;
    }
  }
  return true;
}

bool Matrix::operator!=(const Matrix &a) const { return !(*this == a); }

Matrix task::operator*(const double &a, const Matrix &b) { return b * a; }

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
  for (size_t i = 0; i < matrix.rows; ++i) {
    for (size_t j = 0; j < matrix.cols; ++j) {
      output << matrix[i][j] << ' ';
    }
  }
  output << '\n';
  return output;
}
std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
  int rows, cols;
  input >> rows >> cols;
  matrix = Matrix(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      input >> matrix.ptr[i][j];
    }
  }
  return input;
}
