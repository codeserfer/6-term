#include "stdafx.h"
#include "Stack.h"

void Stack::Push(double x) 
{
	Add(&x); 
}

double Stack::Pop() {
	double res;
	TakeLast(&res); 
	return res; 
}

Stack::~Stack()
{
	List::~List();
}

void Stack::Sort(bool dir, int method)
{
	switch (method)
	{
	case 0:
		SortMethod1(0, Count() - 1, dir);
	case 1:
		SortMethod2(dir);
	default:
		break;
	}

}

void Stack::SortMethod1(int first, int last, bool dir)
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
		SortMethod1(i, last, dir);
	if (first < j)
		SortMethod1(first, j, dir);
}

void Stack::SortMethod2(bool dir)
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