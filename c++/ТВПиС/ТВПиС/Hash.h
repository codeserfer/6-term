#pragma once
#include "List.h"

#define NUMBER_OF_KEYS 5 

class Hash
{
public:
	Hash(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
	~Hash();

	List* FindList(char*);

protected:
	virtual unsigned int Key1(char*) { return 0; }
	virtual unsigned int Key2(char*) { return 0; }
	virtual unsigned int Key3(char*) { return 0; }
	virtual unsigned int Key4(char*) { return 0; }
	virtual unsigned int Key5(char*) { return 0; }

	unsigned int CombineKeys(unsigned int key1 = 0, unsigned int key2 = 0, unsigned int key3 = 0, unsigned int key4 = 0, unsigned int key5 = 0);

	List** table;
	int n1, n2, n3, n4, n5;
	int capacity;

private:

	int NumberSize(int number) { return (number /= 10) ? NumberSize(number) + 1 : 1; }
};
