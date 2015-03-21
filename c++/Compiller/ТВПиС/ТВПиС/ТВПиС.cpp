// ÒÂÏèÑ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Heap.h"
#include "List.h"
#include "Segment.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	/*int* int1 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int1 = 55;
	cout << *int1;

	char* str1 = (char*)Heap::Instance().GetMemory(sizeof(char[7]));
	str1 = "Hello!\0";
	cout << str1;

	double* double1 = (double*)Heap::Instance().GetMemory(sizeof(double));
	*double1 = 76.47;
	cout << *double1;
	cout << endl << endl;*/

	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;
	int* int24 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int24 = 24;
	int* int26 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int26 = 26;
	int* int134 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int134 = 134;

	List* l = new List(sizeof(int));
	l->Add(int26);
	l->Add(int24);
	l->Add(int53);
	l->Add(int134);


	/*int* temp = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp);
	cout << *temp << endl;
	l->TakeFirst(temp);
	cout << *temp << endl;
	l->TakeFirst(temp);
	cout << *temp << endl;
	l->TakeFirst(temp);
	cout << *temp << endl;*/

	int* temp1 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp1);
	cout << *temp1 << endl;
	int* temp2 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp2);
	cout << *temp2 << endl;
	int* temp3 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp3);
	cout << *temp3 << endl;
	int* temp4 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp4);
	cout << *temp4 << endl;


	return 0;
}