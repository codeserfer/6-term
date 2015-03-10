// ÒÂÏèÑ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Heap.h"
#include "Segment.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Segment* a = new Segment(nullptr);
	char* b1 = (char*)a->PushBlock(10);
	char* b2 = (char*)a->PushBlock(20);
	char* b3 = (char*)a->PushBlock(30);
	char* b4 = (char*)a->PushBlock(40);
	char* b5 = (char*)a->PushBlock(65439);

	a->RemoveBlock(b3);
	a->RemoveBlock(b2);
	a->RemoveBlock(b4);
	
	char* b6 = (char*)a->PushBlock(60);

	a->RemoveBlock(b5);

	return 0;
}

