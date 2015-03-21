#include "stdafx.h"
#include "Stack.h"

void Stack::Push(double x) 
{
	Add(&x); 
}

double Stack::Pop() {
	double res;
	TakeLast(&res); 
	return res; 
}
