﻿/*+===================================================================
 * Паралельне прогорамування - 1. Основи паралельного програмування
 * Лабораторна робота N 6.
 * ПОТОКИ В БИБЛИОТЕКЕ OpenMP
 * F1 1.16: d = ((A + B)* (C *(MA*MD)))
 * F2 2.23: f = MAX(MK * ML - MO)
 * F3 3.28: v = MAX(MR*S) + MIN((MT*MW + MV))
 * Гунавардана Руслан ІП-31
 * 28 травня 2016 року н.э.
 ===================================================================+*/

#include "stdafx.h"
#include "assert.h"
#include "data.h"

#undef max
#undef min

using std::wcout;
using std::endl;
using std::vector;
using std::fill;

constexpr int kMaxPrintable = 9;

parallel_programming_lab4::ColVector::ColVector(const size_type size) : vector(size)
{
  assert(1 <= size && static_cast<int>(size) <= std::numeric_limits<int>::max());
}

parallel_programming_lab4::ColVector::ColVector() : vector()
{
}

parallel_programming_lab4::ColVector::ColVector(const size_type size, double fill_with) : ColVector(size)
{
  fill(begin(), end(), fill_with);
}

parallel_programming_lab4::ColVector parallel_programming_lab4::ColVector::Zeros(const size_type size)
{
  return ColVector(size, 0.);
}


parallel_programming_lab4::ColVector parallel_programming_lab4::ColVector::Ones(const size_type size)
{
  return ColVector(size, 1.);
}


double parallel_programming_lab4::ColVector::operator*(const ColVector& other) const
{
  assert(this->size() == other.size());
  double result = 0;
  for (size_type i = 0; i < size(); i++)
    result += (*this)[i] * other[i];
  return result;
}


double parallel_programming_lab4::ColVector::Max() const noexcept
{
  auto max = std::numeric_limits<double>::min();
  for (size_type i = 0; i < size(); i++)
    if (max < (*this)[i])
      max = (*this)[i];
  return max;
}

void parallel_programming_lab4::ColVector::Fill(const double fill_with) noexcept
{
  fill(begin(), end(), fill_with);
}

void parallel_programming_lab4::ColVector::Print() const noexcept
{
  if (this->size() < kMaxPrintable)
  {
    for (auto e : *this)
    {
      wcout << e << ' ';
    }
    wcout << endl;
  }
}

parallel_programming_lab4::Matrix::Matrix() : vector()
{
  fill(begin(), end(), vector<double>());
}

parallel_programming_lab4::Matrix::Matrix(const size_type rows, const size_type cols) : vector(rows)
{
  assert(1 <= rows && static_cast<int>(rows) <= std::numeric_limits<int>::max()
    && 1 <= cols && static_cast<int>(cols) <= std::numeric_limits<int>::max());
  fill(begin(), end(), vector<double>(cols));
}


parallel_programming_lab4::Matrix::Matrix(const size_type rows, const size_type cols, double fill_with) : Matrix(rows, cols)
{
  Fill(fill_with);
}

parallel_programming_lab4::Matrix parallel_programming_lab4::Matrix::Ones(const size_type size)
{
  return Matrix(size, size, 1);
}


parallel_programming_lab4::Matrix parallel_programming_lab4::Matrix::operator+(const Matrix& other) const
{
  assert(this->size() == other.size() && (*this)[0].size() == other[0].size());
  Matrix result(size(), size());
  for (size_type i = 0; i < size(); i++)
    for (size_type j = 0; j < (*this)[i].size(); j++)
      result[i][j] = (*this)[i][j] + other[i][j];
  return result;
}


parallel_programming_lab4::Matrix parallel_programming_lab4::Matrix::operator-(const Matrix& other) const
{
  assert(this->size() == other.size() && (*this)[0].size() == other[0].size());
  Matrix result(size(), size());
  for (size_type i = 0; i < size(); i++)
    for (size_type j = 0; j < (*this)[i].size(); j++)
      result[i][j] = (*this)[i][j] - other[i][j];
  return result;
}


parallel_programming_lab4::Matrix parallel_programming_lab4::Matrix::operator*(const Matrix& other) const
{
  assert((*this)[0].size() == other.size());
  Matrix result(this->size(), other[0].size());
  for (int i = 0; static_cast<size_type>(i) < this->size(); i++)
  {
    for (int j = 0; static_cast<size_type>(j) < other[i].size(); j++)
    {
      result[i][j] = 0;
      for (size_type k = 0; k < other.size(); k++)
        result[i][j] += (*this)[i][k] * other[k][j];
    }
  }
  return result;
}


parallel_programming_lab4::ColVector parallel_programming_lab4::Matrix::operator*(const ColVector& vector) const
{
  assert((*this)[0].size() == vector.size());
  auto result = ColVector::Zeros(this->size());
  for (int i = 0; static_cast<size_type>(i) < this->size(); i++)
    for (size_type j = 0; j < vector.size(); j++)
      result[i] += vector[j] * (*this)[i][j];
  return result;
}


double parallel_programming_lab4::Matrix::Max() const noexcept
{
  auto max = std::numeric_limits<double>::min();
  for (size_type i = 0; i < size(); i++)
  {
    for (size_type j = 0; j < (*this)[0].size(); j++)
    {
      if (max < (*this)[i][j])
        max = (*this)[i][j];
    }
  }
  return max;
}


double parallel_programming_lab4::Matrix::Min() const noexcept
{
  auto min = std::numeric_limits<double>::max();
  for (size_type i = 0; i < size(); i++)
  {
    for (size_type j = 0; j < (*this)[i].size(); j++)
    {
      if (min >(*this)[i][j])
        min = (*this)[i][j];
    }
  }
  return min;
}

void parallel_programming_lab4::Matrix::Fill(const double fill_with) noexcept
{
  for (size_t i = 0; i < this->size(); i++)
  {
    fill((*this)[i].begin(), (*this)[i].end(), fill_with);
  }
}

void parallel_programming_lab4::Matrix::Print() const noexcept
{
  if (this->size() < kMaxPrintable)
  {
    for (auto r : *this)
    {
      if (r.size() < kMaxPrintable)
      {
        for (auto e : r)
        {
          wcout << e << ' ';
        }
        wcout << endl;
      }
    }
  }
}

parallel_programming_lab4::RowVector::RowVector() : vector()
{
}

parallel_programming_lab4::RowVector::RowVector(const size_type size) : vector(size)
{
  assert(1 <= size && static_cast<int>(size) <= std::numeric_limits<int>::max());
}

parallel_programming_lab4::RowVector::RowVector(const size_type size, double fill_with) : RowVector(size)
{
  fill(begin(), end(), fill_with);
}

parallel_programming_lab4::RowVector parallel_programming_lab4::RowVector::Ones(const size_type size)
{
  return RowVector(size, 1.);
}


parallel_programming_lab4::RowVector parallel_programming_lab4::RowVector::operator+(const RowVector& other) const
{
  assert(this->size() == other.size());
  RowVector result(size());
  for (size_type i = 0; i < size(); i++)
    result[i] = (*this)[i] + other[i];
  return result;
}


double parallel_programming_lab4::RowVector::operator*(const RowVector& other) const
{
  assert(this->size() == other.size());
  double result = 0;
  for (size_type i = 0; i < size(); i++)
    result += (*this)[i] * other[i];
  return result;
}


parallel_programming_lab4::RowVector parallel_programming_lab4::RowVector::operator*(const Matrix& matrix) const
{
  assert(this->size() == matrix.size());
  RowVector result(matrix[0].size());
  for (size_type i = 0; i < matrix[0].size(); i++)
  {
    result[i] = 0;
    for (size_type j = 0; j < this->size(); j++)
      result[i] += (*this)[j] * matrix[j][i];
  }

  return result;
}

void parallel_programming_lab4::RowVector::Fill(const double fill_with) noexcept
{
  fill(begin(), end(), fill_with);
}

void parallel_programming_lab4::RowVector::Print() const noexcept
{
  if (this->size() < kMaxPrintable)
  {
    for (auto e : *this)
    {
      wcout << e << ' ';
    }
    wcout << endl;
  }
}
