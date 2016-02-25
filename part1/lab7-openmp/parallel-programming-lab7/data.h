#pragma once

namespace parallel_programming_lab7
{
  using std::vector;

  class ColVector : public vector<double>
  {
    explicit ColVector(const size_type size);
    explicit ColVector(const size_type size, double fill_with);
  public:
    static ColVector Zeros(const size_type size);
    static ColVector Ones(const size_type size);

    double operator*(const ColVector &other) const;
    double Max() const noexcept;
  };

  class Matrix : public vector<vector<double>>
  {
    explicit Matrix(const size_type rows, const size_type cols);
    explicit Matrix(const size_type rows, const size_type cols, double fill_with);
  public:
    static Matrix Ones(const size_type size);

    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    ColVector operator*(const ColVector &other) const;
    double Max() const noexcept;
    double Min() const noexcept;
  };

  class RowVector : public vector<double>
  {
    explicit RowVector(const size_type size);
    explicit RowVector(const size_type size, double fill_with);
  public:
    static RowVector Ones(const size_type size);

    RowVector operator+(const RowVector &other) const;
    double operator*(const RowVector &other) const;
    RowVector operator*(const Matrix &other) const;
  };
}

