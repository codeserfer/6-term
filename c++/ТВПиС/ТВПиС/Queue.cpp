#include "stdafx.h"
#include "Queue.h"

void Queue::Put(double x) 
{
	Add(&x); 
}

double Queue::Get()
{
	double res;
	TakeFirst (&res);
	return res; 
}

Queue::~Queue()
{
	List::~List(); 
}

void Queue::Sort(bool dir, SortingMethod method)
{
	switch (method)
	{
	case SortingMethod::QuickSort:
		QuickSort(0, Count() - 1, dir);
	case SortingMethod::BubbleSort:
		BubbleSort(dir);
	default:
		break;
	}

}

void Queue::QuickSort(int first, int last, bool dir)
{
	int i = first;
	int j = last;
	int x = *(int*)GetAccessor((first + last) / 2);

	do {
		while (*(int*)GetAccessor(i) < x) i++;
		while (*(int*)GetAccessor(j) > x) j--;

		if (i <= j) {
			if (dir)
			{
				if (*(double*)GetAccessor(i) > *(double*)GetAccessor(j)) Swap(i, j);
			}
			else
			{
				if (*(double*)GetAccessor(i) < *(double*)GetAccessor(j)) Swap(i, j);
			}

			i++;
			j--;
		}
	} while (i <= j);

	if (i < last)
		QuickSort(i, last, dir);
	if (first < j)
		QuickSort(first, j, dir);
}

void Queue::BubbleSort(bool dir)
{
	int n = Count();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (dir)
			{
				if (*(double*)GetAccessor(j) > *(double*)GetAccessor(j + 1)) Swap(j, j + 1);
			}
			else
			{
				if (*(double*)GetAccessor(j) < *(double*)GetAccessor(j + 1)) Swap(j, j + 1);
			}
		}
	}
}