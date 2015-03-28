#pragma once
#include "List.h"
#include "Defaults.h"

class Stack : public List
{
public:
	Stack(int elementCount = LISTSIZE) : List(sizeof(double), elementCount) { };
	~Stack();

	void   Push (double);
	double Pop ();
};

