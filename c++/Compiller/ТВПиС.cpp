// ÒÂÏèÑ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Heap.h"
//#include "List.h"
#include "Stack.h"
#include "Deque.h"
#include "Queue.h"
#include "Segment.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(double));
	*int53 = 53;
	int* int24 = (int*)Heap::Instance().GetMemory(sizeof(double));
	*int24 = 24;
	int* int26 = (int*)Heap::Instance().GetMemory(sizeof(double));
	*int26 = 26;
	int* int134 = (int*)Heap::Instance().GetMemory(sizeof(double));
	*int134 = 134;

	Queue* l = new Queue(5);
	l->Add(int53);
	l->Add(int24);
	l->Add(int26);
	l->Add(int26);
	l->Add(int53);
	l->Add(int24);
	l->Add(int134);
	l->Add(int24);
	l->Add(int134);
	//l->Swap(0, 7);
	//l->Sort(0, 8);
	//l->Sort2(9);
	l->Sort();
	

	int* t = (int*)Heap::Instance().GetMemory(sizeof(int));
	for (int i = 0; i < 9; i++)
	{
		l->TakeFirst(t);
		std::cout << *t << endl;
	}

	

	return 0;
}