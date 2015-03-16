#include "stdafx.h"
#include "Stack.h"


Stack::Stack() : List(sizeof(double)) { };
Stack::~Stack() { List::~List(); }
void Stack::Push(double x) {	Add(&x); }
double Stack::Pop() {
	double res;
	TakeLast(&res); 
	return res; 
}
