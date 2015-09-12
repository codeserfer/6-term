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

	void Sort(bool dir = true, SortingMethod method = SortingMethod::QuickSort);
	void QuickSort(int, int, bool dir);
	void BubbleSort(bool dir);
};

