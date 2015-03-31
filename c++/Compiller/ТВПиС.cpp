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

	double d53 = *(double*)Heap::Instance(4000).GetMemory(sizeof(double));
	d53 = 53;
	double d24 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d24 = 24;
	double d26 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d26 = 26;
	double d134 = *(double*)Heap::Instance().GetMemory(sizeof(double));
	d134 = 134;

	List* l = new List(sizeof(double), 5);
	l->Add(&d53);
	l->Add(&d26);
	l->Add(&d24);
	l->Add(&d26);
	l->Add(&d134);
	l->Add(&d53);
	l->Add(&d26);
	l->Add(&d134);
	l->Add(&d26);
	l->Add(&d53);
	l->Add(&d24);
	l->Add(&d26);
	l->Add(&d24);
	l->Add(&d53);
	l->Add(&d134);
	l->Add(&d134);
	l->Add(&d26);
	l->Add(&d53);
	l->Add(&d26);
	l->Add(&d134);
	l->Add(&d53);
	l->Add(&d134);

	cout << "Count before taking: " << l->Count() << endl;

	double t = *(double*)Heap::Instance().GetMemory(sizeof(double));
	l->TakeFirst(&t);
	cout << "First element: " << t << endl;

	l->TakeLast(&t);
	cout << "Last element: " << t << endl;

	l->Take(6, &t);
	cout << "Sixth element: " << t << endl;

	cout << "Count after taking: " << l->Count() << endl;


	Stack* s = new Stack(5);
	s->Push(d53);
	s->Push(d24);
	s->Push(d26);
	s->Push(d26);
	s->Push(d53);
	s->Push(d24);
	s->Push(d134);
	s->Push(d24);
	s->Push(d134);

	s->Sort(false, SortingMethod::BubbleSort);

	cout << "Sorted stack:" << endl;
	
	for (int i = 0; i < 9; i++)
	{
		cout << s->Pop() << endl;
	}
	

	return 0;
}