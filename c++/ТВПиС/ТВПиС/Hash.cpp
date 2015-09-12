#include "stdafx.h"
#include "Hash.h"
#include <exception>

using namespace std;

Hash::Hash(unsigned int n1, unsigned int n2, unsigned int n3, unsigned int n4, unsigned int n5) 
{
	table = new List*[capacity = CombineKeys(this->n1 = n1, this->n2 = n2, this->n3 = n3, this->n4 = n4, this->n5 = n5) + 1];

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
	return	n1 ? key1 % (n1 + 1) : 0 +
			n2 ? key2 % (n2 + 1) : 0 +
			n3 ? key3 % (n3 + 1) : 0 +
			n4 ? key4 % (n4 + 1) : 0 +
			n5 ? key5 % (n5 + 1) : 0;
}

