/*+===================================================================
* Паралельне прогорамування - 1. Основи паралельного програмування
* Лабораторна робота N 6.
* ПОТОКИ В БИБЛИОТЕКЕ OpenMP
* F1 1.16: d = ((A + B)* (C *(MA*MD)))
* F2 2.23: f = MAX(MK * ML - MO)
* F3 3.28: v = MAX(MR*S) + MIN((MT*MW + MV))
* Гунавардана Руслан ІП-31
* 22 листопада 2015 року н.э.
===================================================================+*/

// parallel-programming-lab7.cpp : Defines the entry point for the console application.
// compile with: /openmp 
//

// prevents compilation without /openmp
#ifndef _OPENMP
#error /openmp compiler parameter requiered
#endif

#include "stdafx.h"
#include "data.h"

using namespace parallel_programming_lab7;

using std::array;

constexpr uint_fast32_t kDefaultMatrixSize = 4;
constexpr uint_fast32_t kMinSleepTime = 700;
constexpr uint_fast32_t kTasksCount = 3;
static_assert(kTasksCount <= std::numeric_limits<int>::max(), "kTasksCount should be < 2^31");

int _tmain(int argc, TCHAR* argv[])
{
  uint_fast32_t size;
  try
  {
    size = argc > 1 ? std::stoi(argv[1]) : kDefaultMatrixSize;
  }
  catch (std::invalid_argument)
  {
    _tprintf(_T("Matrix size argument should be an integer. Passed instead: %s\n"), argv[1]);
    return -1;
  }
  const auto A  = RowVector::Ones(size), B  = RowVector::Ones(size), C  = RowVector::Ones(size);
  const auto S  = ColVector::Ones(size);
  const auto MA = Matrix::Ones(size), MD = Matrix::Ones(size), MK = Matrix::Ones(size), 
             ML = Matrix::Ones(size), MO = Matrix::Ones(size), MT = Matrix::Ones(size),
             MR = Matrix::Ones(size), MW = Matrix::Ones(size), MV = Matrix::Ones(size);
  const array<std::function<double()>, kTasksCount> tasks
    {
      [&]{ return (A + B) * (C * (MA * MD)); },
      [&]{ return (MK * ML - MO).Max(); },
      [&]
      {
        double d1, d2;
        #pragma omp parallel sections
        {
          #pragma omp section
          d1 = (MR * S).Max();
          #pragma omp section
          d2 = (MT * MW + MV).Min();
        }
        return d1 + d2;
      },
    };

  _tprintf(omp_in_parallel() ? _T("In parallel region.\n") : _T("Not in parallel region.\n"));
  omp_set_nested(true);

  #pragma omp parallel for num_threads(static_cast<int>(tasks.size()))
  for (int i = 0; static_cast<size_t>(i) < tasks.size(); i++)
  {
    #pragma omp critical
    _tprintf(_T("Task %i started in Thread %i.\n"), i + 1, omp_get_thread_num());
    
    auto result = tasks[i]();

    #pragma omp critical
    _tprintf(_T("Task %i finished. Result: %.1f\n"), i + 1, result);
  }
  _tprintf(_T("Threads joined.\n"));
  return 0;
}

