#include "stdafx.h"
#include "Deque.h"


Deque::Deque() : List(sizeof(double)) { };
Deque::~Deque() { List::~List(); }
void Deque::Put(double x) 
double Deque::First() 
double Deque::Last()  
{
	double res;
	TakeLast(&res);
	return res; 
}