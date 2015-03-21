#pragma once
#include "List.h"

class Deque : public List
{
public:
	Deque() : List(sizeof(double)) { };
	~Deque() { List::~List(); }

	void   Put (double);
	double First ();
	double Last ();
};

