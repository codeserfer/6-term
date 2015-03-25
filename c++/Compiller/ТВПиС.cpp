// “¬œË—.cpp : Defines the entry point for the console application.
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

	/*int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
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

	int* temp1 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeLast(temp1);
	cout << *temp1 << endl;
	int* temp2 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp2);
	cout << *temp2 << endl;
	int* temp3 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeLast(temp3);
	cout << *temp3 << endl;
	int* temp4 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp4);
	cout << *temp4 << endl;


	int* temp5 = (int*)Heap::Instance().GetMemory(sizeof(int));
	l->TakeFirst(temp5);
	cout << *temp5 << endl;*/

	/*List* l = new List(sizeof(int), 10);
	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;
	int* int24 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int24 = 24;
	int* int26 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int26 = 26;
	int* int134 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int134 = 134;

	l->Add(int24);
	l->Add(int26);
	l->Add(int53);
	l->Add(int134);
	l->Add(int134);
	l->Add(int53);
	l->Add(int24);
	l->Add(int134);
	l->Add(int26);
	l->Add(int134);
	l->Add(int24);
	l->Add(int24);
	l->Add(int134);

	int* t = (int*)Heap::Instance().GetMemory(sizeof(int));
	for (int i = 0; i < 13; i++)
	{
		l->TakeFirst(t);
		std::cout << *t << endl;
	}*/


	/*List* l = new List(sizeof(int));
	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;

	for (int i = 0; i < 500; i++)
	{
		l->Add(int53);
	}

	int* t = (int*)Heap::Instance().GetMemory(sizeof(int));

	for (int i = 0; i < 500; i++)
	{
		l->TakeFirst(t);
	}*/


	
	/*int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;
	int* int24 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int24 = 24;
	int* int26 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int26 = 26;
	int* int134 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int134 = 134;

	List* l = new List(sizeof(int), 10);
	l->Add(int24);
	l->Add(int26);
	l->Add(int53);
	l->Add(int134);
	l->Add(int134);
	l->Add(int53);
	l->Add(int24);
	l->Add(int134);
	l->Add(int26);
	l->Add(int134);
	l->Add(int24);
	l->Add(int24);
	l->Add(int134);
	l->Add(int26);


	int* t = (int*)Heap::Instance().GetMemory(sizeof(int));
	for (int i = 0; i < 14; i++)
	{
		l->TakeLast(t);
		std::cout << *t << endl;
	}*/

	List* l = new List(sizeof(int));
	int* int53 = (int*)Heap::Instance().GetMemory(sizeof(int));
	*int53 = 53;

	for (int i = 0; i < 500; i++)
	{
		l->Add(int53);
	}

	int* t = (int*)Heap::Instance().GetMemory(sizeof(int));

	for (int i = 0; i < 500; i++)
	{
		l->TakeLast(t);
	}





	return 0;
}