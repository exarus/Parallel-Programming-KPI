/*+===================================================================
* Паралельне прогорамування - 2. Програмування для паралельних систем
* Лабораторна робота 2. Win32. Семафори, мютекси, події, критичні секції
*
* A = sort(Z) * e - C * (MO * MK)
*
* Гунавардана Руслан ІП-31
* 27 травня 2015 року н.э.
===================================================================+*/

#include "stdafx.h"
#include "data.h"

using namespace parallel_programming_lab3;

using std::array;
using std::vector;
using std::merge;
using std::wcerr;
using std::wcout;
using std::endl;
using std::sort;

constexpr size_t kDefaultMatrixSize = 4;
constexpr uint_fast32_t kTasksCount = 4;

struct TaskParameters
{
  size_t size;
  array<int, kTasksCount + 1> H;
  double *e;
  RowVector *A, *C, *Z;
  Matrix *MK, *MO;
};

void Task1(TaskParameters* p);
void Task2(TaskParameters* p);
void Task3(TaskParameters* p);
void Task4(TaskParameters* p);

void InitThreading();
void PerformCalculation(const int num, const TaskParameters* p, const double e, const RowVector &C, const Matrix &MO);
void WaitAndCloseThreads(const vector<HANDLE>& thread_handles);

// events
const HANDLE e_T1_input_ready = CreateEvent(nullptr, TRUE, FALSE, nullptr);
const HANDLE e_T2_input_ready = CreateEvent(nullptr, TRUE, FALSE, nullptr);
const HANDLE e_T4_input_ready = CreateEvent(nullptr, TRUE, FALSE, nullptr);
const HANDLE e_T2_sorted = CreateEvent(nullptr, FALSE, FALSE, nullptr);
const HANDLE e_T4_sorted = CreateEvent(nullptr, FALSE, FALSE, nullptr);
const HANDLE e_T34_sorted = CreateEvent(nullptr, FALSE, FALSE, nullptr);
const HANDLE e_all_sorted = CreateEvent(nullptr, TRUE, FALSE, nullptr);
const HANDLE e_T2_finish = CreateEvent(nullptr, FALSE, FALSE, nullptr);
const HANDLE e_T3_finish = CreateEvent(nullptr, FALSE, FALSE, nullptr);
const HANDLE e_T4_finish = CreateEvent(nullptr, FALSE, FALSE, nullptr);

// safe copy
const HANDLE m_e_copy = CreateMutex(nullptr, FALSE, nullptr);
const HANDLE s_C_copy = CreateSemaphore(nullptr, 1, 1, nullptr);
CRITICAL_SECTION cs_MO_copy;

int _tmain(int argc, TCHAR* argv[])
{
  size_t size;
  try
  {
    size = argc > 1 ? std::stoi(argv[1]) : kDefaultMatrixSize;
  }
  catch (std::invalid_argument)
  {
    _tprintf(_T("Matrix size argument should be an integer. Passed instead: %s\n"), argv[1]);
    return -1;
  }

  double e = 0;
  RowVector A(size), Z(size), C(size);
  Matrix MO(size, size), MK(size, size);

  InitThreading();
  vector<HANDLE> threads(kTasksCount);
  const array<void(*)(TaskParameters*), kTasksCount> tasks { &Task1, &Task2, &Task3, &Task4 };

  array<int, kTasksCount + 1> H;
  for (size_t i = 0; i < H.size(); i++)
  {
    H[i] = static_cast<int>(size * static_cast<double>(i) / kTasksCount);
  }
  TaskParameters params{ size, H, &e, &A, &C, &Z, &MK, &MO };

  for (size_t i = 0; i < kTasksCount; i++)
  {
    auto thread_function = reinterpret_cast<LPTHREAD_START_ROUTINE>(tasks[i]);
    threads[i] = CreateThread(nullptr, 0, thread_function, &params, 0, nullptr);
    if (threads[i] == nullptr)
    {
      wcerr << _T("Creating Thread ") << i + 1 << _T(" failed.") << endl;
      ExitProcess(2);
    }
  }

  WaitAndCloseThreads(threads);
  return 0;
}

void Task1(TaskParameters* p)
{
  auto e = p->e;
  auto A = p->A, C = p->C, Z = p->Z;
  auto MO = p->MO, MK = p->MK;
  Z->Fill(1);
  SetEvent(e_T1_input_ready);

  sort(Z->begin(), Z->begin() + p->H[1]);
  WaitForSingleObject(e_T2_sorted, INFINITE);
  RowVector B(p->H[2]);
  merge(Z->begin(), Z->begin() + p->H[1], Z->begin() + p->H[1], Z->begin() + p->H[2], B.begin());
  for (size_t i = 0; i < B.size(); i++)
  {
    (*p->Z)[i] = B[i];
  }
  WaitForSingleObject(e_T34_sorted, INFINITE);
  RowVector B2(p->size);
  merge(p->Z->begin(), p->Z->begin() + p->H[2], p->Z->begin() + p->H[2], p->Z->begin() + p->H[4], B2.begin());
  for (size_t i = 0; i < B.size(); i++)
  {
    (*p->Z)[i] = B2[i];
  }
  SetEvent(e_all_sorted);

  WaitForSingleObject(e_T2_input_ready, INFINITE);
  WaitForSingleObject(m_e_copy, INFINITE);
  auto e1 = *e;
  ReleaseMutex(m_e_copy);

  WaitForSingleObject(e_T4_input_ready, INFINITE);
  WaitForSingleObject(s_C_copy, INFINITE);
  auto C1 = *C;
  ReleaseSemaphore(s_C_copy, 1, nullptr);
  EnterCriticalSection(&cs_MO_copy);
  auto MO1 = *MO;
  LeaveCriticalSection(&cs_MO_copy);

  PerformCalculation(0, p, e1, C1, MO1);
  HANDLE e_finish[] = { e_T2_finish, e_T3_finish, e_T4_finish };
  WaitForMultipleObjects(3, e_finish, TRUE, INFINITE);
  A->Print();
}

void Task2(TaskParameters* p)
{
  *p->e = 1;
  p->MK->Fill(1);
  SetEvent(e_T2_input_ready);

  WaitForSingleObject(e_T1_input_ready, INFINITE);
  sort(p->Z->begin() + p->H[1], p->Z->begin() + p->H[2]);
  SetEvent(e_T2_sorted);

  WaitForSingleObject(m_e_copy, INFINITE);
  auto e2 = *p->e;
  ReleaseMutex(m_e_copy);

  WaitForSingleObject(e_T4_input_ready, INFINITE);
  WaitForSingleObject(s_C_copy, INFINITE);
  auto C2 = *p->C;
  ReleaseSemaphore(s_C_copy, 1, nullptr);
  EnterCriticalSection(&cs_MO_copy);
  auto MO2 = *p->MO;
  LeaveCriticalSection(&cs_MO_copy);
  WaitForSingleObject(e_all_sorted, INFINITE);

  PerformCalculation(1, p, e2, C2, MO2);
  SetEvent(e_T2_finish);
}

void Task3(TaskParameters* p)
{
  WaitForSingleObject(e_T1_input_ready, INFINITE);
  sort(p->Z->begin() + p->H[2], p->Z->begin() + p->H[3]);
  WaitForSingleObject(e_T4_sorted, INFINITE);
  RowVector B(p->H[4] - p->H[2]);
  merge(p->Z->begin() + p->H[2], p->Z->begin() + p->H[3], p->Z->begin() + p->H[3], p->Z->begin() + p->H[4], B.begin());
  for (size_t i = 0; i < B.size(); i++)
  {
    (*p->Z)[i + p->H[2]] = B[i];
  }
  SetEvent(e_T34_sorted);

  WaitForSingleObject(e_T2_input_ready, INFINITE);
  WaitForSingleObject(m_e_copy, INFINITE);
  auto e3 = *p->e;
  ReleaseMutex(m_e_copy);

  WaitForSingleObject(e_T4_input_ready, INFINITE);
  WaitForSingleObject(s_C_copy, INFINITE);
  auto C3 = *p->C;
  ReleaseSemaphore(s_C_copy, 1, nullptr);
  EnterCriticalSection(&cs_MO_copy);
  auto MO3 = *p->MO;
  LeaveCriticalSection(&cs_MO_copy);
  WaitForSingleObject(e_all_sorted, INFINITE);

  PerformCalculation(2, p, e3, C3, MO3);
  SetEvent(e_T3_finish);
}

void Task4(TaskParameters* p)
{
  p->C->Fill(1);
  p->MO->Fill(1);
  SetEvent(e_T4_input_ready);

  WaitForSingleObject(e_T1_input_ready, INFINITE);
  sort(p->Z->begin() + p->H[3], p->Z->begin() + p->H[4]);
  SetEvent(e_T4_sorted);

  WaitForSingleObject(e_T2_input_ready, INFINITE);
  WaitForSingleObject(m_e_copy, INFINITE);
  auto e4 = *p->e;
  ReleaseMutex(m_e_copy);

  WaitForSingleObject(s_C_copy, INFINITE);
  auto C4 = *p->C;
  ReleaseSemaphore(s_C_copy, 1, nullptr);
  EnterCriticalSection(&cs_MO_copy);
  auto MO4 = *p->MO;
  LeaveCriticalSection(&cs_MO_copy);
  WaitForSingleObject(e_all_sorted, INFINITE);

  PerformCalculation(3, p, e4, C4, MO4);
  SetEvent(e_T4_finish);
}

void InitThreading()
{
  if (m_e_copy == nullptr || s_C_copy == nullptr)
  {
    wcerr << _T("Synchronization primitives initialization failed.") << endl;
    ExitProcess(2);
  }
  InitializeCriticalSectionAndSpinCount(&cs_MO_copy, 0);
}

void WaitAndCloseThreads(const vector<HANDLE>& thread_handles)
{
  WaitForMultipleObjects(thread_handles.size(), thread_handles.data(), TRUE, INFINITE);
  _tprintf(_T("Threads joined.\n"));
  for (size_t i = 0; i < thread_handles.size(); i++)
  {
    CloseHandle(thread_handles[i]);
  }
  CloseHandle(e_T1_input_ready);
  CloseHandle(e_T2_input_ready);
  CloseHandle(e_T4_input_ready);
  CloseHandle(e_T2_sorted);
  CloseHandle(e_T4_sorted);
  CloseHandle(e_T34_sorted);
  CloseHandle(e_all_sorted);
  CloseHandle(e_T2_finish);
  CloseHandle(e_T3_finish);
  CloseHandle(e_T4_finish);
  CloseHandle(m_e_copy);
  CloseHandle(s_C_copy);
  DeleteCriticalSection(&cs_MO_copy);
  _tprintf(_T("Handles closed.\n"));
}

void PerformCalculation(const int num, const TaskParameters* p, const double e, const RowVector &C, const Matrix &MO)
{
  auto A = p->A;
  auto Z = *p->Z;
  auto MK = *p->MK;

  for (size_t i = p->H[num]; i < p->H[num + 1]; i++)
  {
    (*A)[i] = Z[i] * e;
    for (size_t j = 0; j < p->size; j++)
    {
      double cell_ji = 0;
      for (size_t k = 0; k < p->size; k++)
      {
        cell_ji = cell_ji + MO[j][k] * MK[k][i];
      }
      (*A)[i] += C[j] * cell_ji;
    }
  }
}
