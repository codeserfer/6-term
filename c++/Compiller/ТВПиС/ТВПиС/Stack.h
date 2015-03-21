#pragma once
#include "List.h"
class Stack : public List
{
public:
	Stack();
	~Stack();

	void   Push (double);
	double Pop ();
};

