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

using std::cout;
using std::endl;

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

const uint_fast32_t kThreadsNum = 3;
const uint_fast32_t kDefaultMatrixSize = 4;
const uint_fast32_t kMinSleepTime = 300;

int main(int argc, char* argv[])
{
	auto size = kDefaultMatrixSize;
	if (argc > 1)
	{
		try
		{
			size = std::stoi(argv[1]);
		}
		catch (std::invalid_argument)
		{
			std::cerr << "Matrix size argument should be an integer. Passed instead: " << argv[1];
			ExitProcess(1);
		}
	}

	rowvec A = ones<rowvec>(size), B = ones<rowvec>(size), C = ones<rowvec>(size);
	colvec S = ones<colvec>(size);
	mat MA = ones(size, size), MD = ones(size, size), MK = ones(size, size),
		ML = ones(size, size), MO = ones(size, size), MT = ones(size, size),
		MR = ones(size, size), MW = ones(size, size), MV = ones(size, size);

	NumberedFunction<double()> tasks[]{
		{1, [&]{ return dot(A + B, C * (MA * MD));             }},
		{2, [&]{ return (MK * ML - MO).max();                  }},
		{3, [&]{ return (MR * S).max() + (MT * MW + MV).min(); }},
	};

	DWORD thread_ids[kThreadsNum];
	HANDLE thread_handles[kThreadsNum];
	for (size_t i = 0; i < kThreadsNum; i++)
	{
		thread_handles[i] = CreateThread(nullptr, 0, InvokeAndPrint, &tasks[i], 0, &thread_ids[i]);

		// Check the return value for success.
		if (thread_handles[i] == nullptr)
		{
			std::cerr << "Creating Thread " << i << " failed.";
			ExitProcess(2);
		}
	}

	// Wait until all threads have terminated.
	WaitForMultipleObjects(kThreadsNum, thread_handles, TRUE, INFINITE);
	cout << "Threads joined." << endl;

	// Close all thread handles and free memory allocations.
	for (size_t i = 0; i < kThreadsNum; i++)
	{
		CloseHandle(thread_handles[i]);
	}
	cout << "Handles closed." << endl;

	return 0;
}

DWORD WINAPI InvokeAndPrint(LPVOID args)
{
	auto func = reinterpret_cast<NumberedFunction<double()>*>(args);
	cout << "Task " + std::to_string(func->num) + " started." << endl;
	Sleep(func->num * kMinSleepTime);
	cout << func->function() << endl;
	cout << "Task " + std::to_string(func->num) + " finished." << endl;
	return 0;
}
