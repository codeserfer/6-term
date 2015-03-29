#pragma once
#include "List.h"
#include "Defaults.h"

class Deque : public List
{
public:
	Deque(int elementCount = LISTSIZE) : List(sizeof(double), elementCount) { };
	~Deque();

	void Put (double);
	double First ();
	double Last ();

	void Sort(bool dir = true, int method = 0);
	void SortMethod1(int, int, bool dir);
	void SortMethod2(bool dir);
};

