#include "stdafx.h"
#include "Hash.h"
#include <exception>

using namespace std;

Hash::Hash(unsigned int n1, unsigned int n2, unsigned int n3, unsigned int n4, unsigned int n5)
{
	table = new List*[capacity = CombineKeys(this->n1 = n1, this->n2 = n2, this->n3 = n3, this->n4 = n4, this->n5 = n5)];

	for (int i = 0; i < capacity; i++)
		table[i] = nullptr;
}
Hash::~Hash()
{
	if (table)
	{
		for (int i = 0; i < capacity; i++)
		{
			if (table[i])
			{
				delete table[i];
				table[i] = nullptr;
			}
		}
		delete[] table;
		table = nullptr;
	}
}

List* Hash::FindList(char* keyWord)
{
	if (keyWord)
		return table[CombineKeys(Key1(keyWord), Key2(keyWord), Key3(keyWord), Key4(keyWord), Key5(keyWord))];

	return nullptr;
}

unsigned int Hash::CombineKeys(unsigned int key1, unsigned int key2, unsigned int key3, unsigned int key4, unsigned int key5)
{
	// @ “ут пахнет жареными душами, мой маленький Ўмель. 

	int* n = new int[NUMBER_OF_KEYS];
	
	n[0] = n1 > 0 ? key1 : 1;
	n[1] = n2 > 0 ? key2 : 1;
	n[2] = n3 > 0 ? key3 : 1;
	n[3] = n4 > 0 ? key4 : 1;
	n[4] = n5 > 0 ? key5 : 1;

	return	(key5 % (n[4] + 1)) +
			(key4 % (n[3] + 1)) * n[3] +
			(key3 % (n[2] + 1)) * n[2] * n[3] +
			(key2 % (n[1] + 1)) * n[1] * n[2] * n[3] +
			(key1 % (n[0] + 1)) * n[0] * n[1] * n[2] * n[3];
}

unsigned int Hash::Key1(char* keyWord)
{
	return 0; 
}
unsigned int Hash::Key2(char* keyWord)
{
	return 0; 
}
unsigned int Hash::Key3(char* keyWord)
{
	return 0; 
}
unsigned int Hash::Key4(char* keyWord)
{
	return 0; 
}
unsigned int Hash::Key5(char* keyWord)
{
	return 0; 
}

