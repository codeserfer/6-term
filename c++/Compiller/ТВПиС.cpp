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

	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;
	int* int24 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int24 = 24;
	int* int26 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int26 = 26;
	int* int134 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int134 = 134;

	List* l = new List(sizeof (int), 10);
	l->Add(int53);
	l->Add(int24);
	l->Add(int26);
	l->Add(int26);
	l->Add(int53);
	l->Add(int24);
	l->Add(int134);
	l->Add(int24);
	l->Add(int134);

	int* t = (int*)l->G(2);
	cout << *t;
	t = (int*)l->G(2);
	cout << *t;

	t = l[0];
	

	return 0;
}