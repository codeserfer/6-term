#include "stdafx.h"
#include "Deque.h"


Deque::Deque() : List(sizeof(double)) { };
Deque::~Deque() { List::~List(); }
void Deque::Put(double x) {	Add(&x); }
double Deque::First() { 	double res;	TakeFirst(&res); 	return res; }
double Deque::Last()  
{
	double res;
	TakeLast(&res);
	return res; 
}
