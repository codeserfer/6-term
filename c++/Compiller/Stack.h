#pragma once
#include "List.h"

class Stack : public List
{
public:
	Stack(int elementCount = LISTSIZE) : List(sizeof(double), elementCount) { };
	~Stack();

	void Push (double);
	double Pop ();
	void Sort(bool dir = true, int method = 0);
	void SortMethod1(int, int, bool dir);
	void SortMethod2(bool dir);
};

