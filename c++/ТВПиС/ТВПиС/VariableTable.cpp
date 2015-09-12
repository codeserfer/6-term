#include "stdafx.h"
#include "VariableTable.h"

VariableTable::~VariableTable()
{
	Hash::~Hash();
}

unsigned int VariableTable::Key1(char* keyWord)
{
	return keyWord ? (int)(*keyWord) % 10 : 0;
}

TValueKeeper* VariableTable::Find(char* name)
{
	List* list = nullptr;

	if (list = FindList(name))
	{
		TValueKeeper* keeper = nullptr;

		for (int i = 0; i < list->Count(); i++)
		{
			if (strcmp((keeper = ((TValueKeeper*)(list->GetAccessor(i))))->GetName(), name) == 0)
			{
				return keeper;
			}
		}
	}

	return nullptr;
}

TValueKeeper* VariableTable::Push(TValueKeeper* keeper)
{
	if (keeper)
	{
		//List* list = nullptr;

		if (!(table[CombineKeys(Key1(keeper->GetName()))]))
		{
			table[CombineKeys(Key1(keeper->GetName()))] = new List(sizeof(TValueKeeper));
		}


		TValueKeeper* temp = (TValueKeeper*)table[CombineKeys(Key1(keeper->GetName()))]->Add(keeper);
		return (temp);
	}

	return nullptr;
}

TValueKeeper* VariableTable::Push(char* name, double value, int type, int size)
{
	return Push(new TValueKeeper(name, new TValue(value, type, size)));
}