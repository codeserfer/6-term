#include "stdafx.h"
#include "Deque.h"

void Deque::Put(double x) 
{
	Add(&x); 
}

double Deque::First() 
{ 
	double res;
	TakeFirst(&res); 
	return res; 
}

double Deque::Last()  
{
	double res;
	TakeLast(&res);
	return res; 
}
