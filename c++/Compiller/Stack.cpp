#include "stdafx.h"
#include "Stack.h"


Stack::Stack() : List(sizeof(double)) { };
Stack::~Stack() { List::~List(); }
void Stack::Push(double x) 
double Stack::Pop() {
	double res;
	TakeLast(&res); 
	return res; 
}