#include "stdafx.h"
#include "Heap.h"
#include <list>

int _tmain(int argc, _TCHAR* argv[])
{

	Heap* h = new Heap();
	std::list<void*> Mylist;

	//double = 8 байтов
	for (int i = 0; i < 8193; i++)
	{
		Mylist.push_back(h->GetMemory(sizeof(double)));
	}
	std::cout << h->GetCountOfSegments();

	return 0;
}

