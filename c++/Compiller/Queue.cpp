#include "stdafx.h"
#include "Queue.h"

void Queue::Put(double x) 
{
	Add(&x); 
}

double Queue::Get() 
{
	double res;
	TakeFirst (&res);
	return res; 
}
