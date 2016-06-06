/*+===================================================================
* Паралельне прогорамування - 1. Основи паралельного програмування
* Лабораторна робота N 5.
* ПОТОКИ В БИБЛИОТЕКЕ WIN32 
* F1 1.16: d = ((A + B)* (C *(MA*MD)))
* F2 2.23: f = MAX(MK * ML - MO)
* F3 3.28: v = MAX(MR*S) + MIN((MT*MW + MV))
* Гунавардана Руслан ІП-31
* 15 листопада 2015 року н.э.
===================================================================+*/

// parallel-programming-lab5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using std::wcerr;
using std::endl;
using std::vector;

using arma::vec;
using arma::mat;
using arma::rowvec;
using arma::colvec;
using arma::ones;
using arma::dot;

template <class T>
struct NumberedFunction
{
	uint_fast32_t num;
	std::function<T> function;
};

DWORD WINAPI InvokeAndPrint(LPVOID args);
vector<HANDLE> StartThreads(vector<NumberedFunction<double()>> &tasks);
void WaitAndCloseThreads(const vector<HANDLE> &thread_handles);

const uint_fast32_t kDefaultMatrixSize = 4;
const uint_fast32_t kMinSleepTime = 700;

int main(int argc, TCHAR* argv[])
{
	size_t size;
	try
	{
		size = argc > 1 ? std::stoi(argv[1]) : kDefaultMatrixSize;
	}
	catch (std::invalid_argument)
	{
		wcerr << _T("Matrix size argument should be an integer. Passed instead: ") << argv[1] << endl;
		ExitProcess(1);
	}

	const rowvec A = ones<rowvec>(size), B = ones<rowvec>(size), C = ones<rowvec>(size);
	const colvec S = ones<colvec>(size);
	const mat MA = ones(size, size), MD = ones(size, size), MK = ones(size, size),
	          ML = ones(size, size), MO = ones(size, size), MT = ones(size, size),
	          MR = ones(size, size), MW = ones(size, size), MV = ones(size, size);
	vector<NumberedFunction<double()>> tasks{
		{1, [&]{ return dot(A + B, C * (MA * MD)); }},
		{2, [&]{ return (MK * ML - MO).max(); }},
		{3, [&]{ return (MR * S).max() + (MT * MW + MV).min(); }},
	};

	WaitAndCloseThreads(StartThreads(tasks));
	return 0;
}

DWORD WINAPI InvokeAndPrint(LPVOID args)
{
	auto func = reinterpret_cast<NumberedFunction<double()>*>(args);
	_tprintf(_T("Task %u started.\n"), func->num);
	Sleep(func->num * kMinSleepTime);
	_tprintf(_T("%.1f\nTask %u finished.\n"), func->function(), func->num);
	return 0;
}

vector<HANDLE> StartThreads(vector<NumberedFunction<double()>> &tasks)
{
	vector<HANDLE> thread_handles(tasks.size());
	DWORD thread_id;
	for (size_t i = 0; i < tasks.size(); i++)
	{
		// Create Thread and check for success.
		thread_handles[i] = CreateThread(nullptr, 0, InvokeAndPrint, &tasks[i], CREATE_SUSPENDED, &thread_id);
		if (thread_handles[i] == nullptr)
		{
			wcerr << _T("Creating Thread ") << i + 1 << _T(" failed.") << endl;
			ExitProcess(2);
		}
		_tprintf(_T("Created Thread %u with id %lu.\n"), i + 1, thread_id);

		if (!SetThreadPriority(thread_handles[i], THREAD_PRIORITY_ABOVE_NORMAL))
		{
			wcerr << _T("Setting priority for Thread ") << i + 1 << _T(" failed.") << endl;
			ExitProcess(3);
		}

		// Suspending and Resuming Thread
		DWORD suspended_count;
		for (size_t j = 0; j <= i; j++)
		{
			suspended_count = SuspendThread(thread_handles[i]);
			if (suspended_count == -1)
			{
				wcerr << _T("Suspending Thread ") << i + 1 << _T(" failed. ")
					_T("Suspended count: ") << suspended_count << endl;
				ExitProcess(4);
			}
		}
		do
		{
			suspended_count = ResumeThread(thread_handles[i]);
		} while (suspended_count != 1 && suspended_count != -1);
		if (suspended_count != 1)
		{
			wcerr << _T("Resuming Thread ") << i + 1 << _T(" failed. ")
				_T("Suspended count: ") << suspended_count << endl;
			ExitProcess(5);
		}
	}
	return thread_handles;
}

void WaitAndCloseThreads(const vector<HANDLE> &thread_handles)
{
	// Wait until all threads are terminated.
	WaitForMultipleObjects(thread_handles.size(), thread_handles.data(), TRUE, INFINITE);
	_tprintf(_T("Threads joined.\n"));

	// Close all thread handles and free memory allocations.
	for (size_t i = 0; i < thread_handles.size(); i++)
	{
		CloseHandle(thread_handles[i]);
	}
	_tprintf(_T("Handles closed.\n"));
}

