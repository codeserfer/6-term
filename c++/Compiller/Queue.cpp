#include "stdafx.h"
#include "Queue.h"


Queue::Queue() : List(sizeof(double)) { };
Queue::~Queue() { List::~List(); }

void Queue::Put(double x) {	Add(&x); }
double Queue::Get() 
{
	double res;
	TakeFirst (&res);
	return res; 
}
