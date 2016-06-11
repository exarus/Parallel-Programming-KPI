/*+===================================================================
* Паралельне прогорамування - 2. Програмування для паралельних сиcтем
* Лабораторна робота 3. OpenMP. Бар’єри, критичні секції
*
* A = (B * MO) * MT + C * (ME * MU)
*
* Гунавардана Руслан ІП-31
* 8 червня 2016 року н.э.
===================================================================+*/

// prevents compilation without /openmp
#ifndef _OPENMP
#error /openmp compiler parameter requiered
#endif

#include "stdafx.h"
#include "data.h"

using namespace parallel_programming_lab4;

// B * MO
void CalculateV(size_t from, size_t to, const RowVector& local_B);
// V * MT + C * (ME * MU)
void CalculateA(size_t from, size_t to, const RowVector &local_C, const RowVector &local_V, const Matrix &local_ME);

const size_t kDefaultN = 4;
const size_t P = 4;
size_t N;
size_t H[P + 1];

RowVector A, B, C, V;
Matrix ME, MO, MT, MU;

int _tmain(int argc, TCHAR* argv[])
{
  try
  {
    N = argc > 1 ? std::stoi(argv[1]) : kDefaultN;
  }
  catch (std::invalid_argument)
  {
    _tprintf(_T("Matrix size argument should be an integer. Passed instead: %s\n"), argv[1]);
    return -1;
  }

  A = RowVector::Ones(N);
  B = RowVector::Ones(N);
  C = RowVector::Ones(N);
  V = RowVector(N);
  ME = Matrix::Ones(N);
  MO = Matrix::Ones(N);
  MT = Matrix::Ones(N);
  MU = Matrix::Ones(N);

  for (size_t i = 0; i < P + 1; i++)
  {
    H[i] = static_cast<int>(N * static_cast<double>(i) / P);
  }
  
  #pragma omp parallel num_threads(P)
  {
    int i = omp_get_thread_num();
    RowVector local_B, local_V, local_C;
    Matrix local_ME;

    #pragma omp critical
    { local_B = B; }
    CalculateV(H[i], H[i + 1], local_B);

    #pragma omp barrier

    #pragma omp critical
    {
      local_V = V;
      local_C = C;
      local_ME = ME;
    }
    CalculateA(H[i], H[i + 1], local_C, local_V, local_ME);
  }

  A.Print();
  return 0;
}

void CalculateV(size_t from, size_t to, const RowVector& local_B)
{
  for (size_t i = from; i < to; i++)
  {
    V[i] = 0;
    for (size_t j = 0; j < N; j++)
    {
      V[i] += B[j] * MO[j][i];
    }
  }
}

void CalculateA(size_t from, size_t to, const RowVector& local_C, const RowVector& local_V, const Matrix& local_ME)
{
  for (size_t i = from; i < to; i++)
  {
    A[i] = 0;
    for (size_t j = 0; j < N; j++)
    {
      double cell = 0;
      for (size_t k = 0; k < N; k++)
      {
        cell += local_ME[j][k] * MU[k][i];
      }
      A[i] += local_V[j] * MT[j][i] + local_C[j] * cell;
    }
  }
}
