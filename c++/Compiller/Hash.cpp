#include "stdafx.h"
#include "Hash.h"


Hash::Hash(int n1, int n2, int n3, int n4, int n5)
{
	this->n1 = n1;
	this->n2 = n2;
	this->n3 = n3;
	this->n4 = n4;
	this->n5 = n5;
}
List* Hash::FindList(char* KeyWord)
{
	
}


Hash::~Hash()
{
}

int Hash::CombineKeys(int k1, int k2, int k3, int k4, int k5)
{
	return 	k5 +
		k4*n4 +
		k3*n3*n4 +
		k2*n2*n3*n4 +
		k1*n1*n2*n3*n4;
}

int Hash::Key1(char* keyWord) {	return 0; }
int Hash::Key2(char* keyWord){	return 0; }
int Hash::Key3(char* keyWord) {	return 0; }
int Hash::Key4(char* keyWord) {	return 0; }
int Hash::Key5(char* keyWord) 
{
	return 0; 
}

