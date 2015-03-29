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

void Queue::Sort(bool dir, int method)
{
	switch (method)
	{
	case 0:
		SortMethod1(0, Count()-1, dir);
	case 1:
		SortMethod2(dir);
	default:
		break;
	}

}

void Queue::SortMethod1(int first, int last, bool dir)
{
	int i = first;
	int j = last;
	int x = *(int*)G((first + last) / 2);

	do {
		while (*(int*)G(i) < x) i++;
		while (*(int*)G(j) > x) j--;

		if (i <= j) {
			if (dir) 
			{
				if (*(double*)G(i) > *(double*)G(j)) Swap(i, j);
			}
			else
			{
				if (*(double*)G(i) < *(double*)G(j)) Swap(i, j);
			}
			
			i++;
			j--;
		}
	} while (i <= j);

	if (i < last)
		SortMethod1 (i, last, dir);
	if (first < j)
		SortMethod1 (first, j, dir);
}

void Queue::SortMethod2(bool dir)
{
	int n = Count();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (dir)
			{
				if (*(double*)G(j) > *(double*)G(j + 1)) Swap(j, j + 1);
			}
			else
			{
				if (*(double*)G(j) < *(double*)G(j + 1)) Swap(j, j + 1);
			}
		}
	}
}