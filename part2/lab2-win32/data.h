#pragma once

namespace parallel_programming_lab3
{
  class ColVector : public std::vector<double>
  {
    explicit ColVector(const size_type size);
    explicit ColVector(const size_type size, double fill_with);
  public:
    static ColVector Zeros(const size_type size);
    static ColVector Ones(const size_type size);

    double operator*(const ColVector &other) const;
    double Max() const noexcept;
    void Fill(const double fill_with) noexcept;
    void Print() const noexcept;
  };

  class Matrix : public std::vector<std::vector<double>>
  {
    explicit Matrix(const size_type rows, const size_type cols, double fill_with);
  public:
    explicit Matrix(const size_type rows, const size_type cols);
    static Matrix Ones(const size_type size);

    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    ColVector operator*(const ColVector &other) const;
    double Max() const noexcept;
    double Min() const noexcept;
    void Fill(const double fill_with) noexcept;
    void Print() const noexcept;
  };

  class RowVector : public std::vector<double>
  {
    explicit RowVector(const size_type size, double fill_with);
  public:
    explicit RowVector(const size_type size);
    static RowVector Ones(const size_type size);

    RowVector operator+(const RowVector &other) const;
    double operator*(const RowVector &other) const;
    RowVector operator*(const Matrix &other) const;
    void Fill(const double fill_with) noexcept;
    void Print() const noexcept;
  };
}

