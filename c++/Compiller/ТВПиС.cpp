#include "stdafx.h"
#include "Heap.h"
#include "Stack.h"
#include "Deque.h"
#include "Queue.h"
#include "Segment.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	double d53 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d53 = 53;
	double d24 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d24 = 24;
	double d26 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d26 = 26;
	double d134 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d134 = 134;

	Stack* l = new Stack(5);
	l->Push(d53);
	l->Push(d24);
	l->Push(d26);
	l->Push(d26);
	l->Push(d53);
	l->Push(d24);
	l->Push(d134);
	l->Push(d24);
	l->Push(d134);
	l->Sort(false);
	
	for (int i = 0; i < 9; i++)
	{
		cout << l->Pop() << endl;
	}

	

	return 0;
}