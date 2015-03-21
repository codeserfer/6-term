#pragma once
#include "List.h"
class Queue : public List
{
public:
	Queue() : List(sizeof(double)) { };
	~Queue() { List::~List(); }

	void   Put (double);
	double Get ();
};

