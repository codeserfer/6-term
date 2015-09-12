#include "stdafx.h"
#include "UnaryOperationList.h"
#include <cstring>
#include "defaults.h"
#include "Exceptions.h"

UnaryOperationList::UnaryOperationList(void)
{
	operations = new List(MAX_OPERATION_NAME_LENGHT + 1);
	LoadOperations();
}


UnaryOperationList::~UnaryOperationList(void)
{
	delete operations;
}

UnaryOperationList& UnaryOperationList::Instance()
{
	static UnaryOperationList _instance;
	return _instance;
}

void UnaryOperationList::LoadOperations()
{
	operations->Add((void*)"+");		// Унарный плюс 
	operations->Add((void*)"-");		// Унарный минус

	operations->Add((void*)"!");		// Логическое НЕ и побитовое отрицание

	operations->Add((void*)"о++");		// Постфиксный инкремент
	operations->Add((void*)"о--");		// Постфиксный декремент
	operations->Add((void*)"++о");		// Префиксный инкремент
	operations->Add((void*)"--о");		// Префиксный декремент

	operations->Add((void*)"input");	// Ввод
	operations->Add((void*)"output");	// Вывод

	operations->Add((void*)"sin");		// Синус
	operations->Add((void*)"cos");		// Косинус
	operations->Add((void*)"rand");		// Косинус
}

bool UnaryOperationList::IsOperation(char* name)
{
	return GetOperationIndex(name) < 0 ? false : true;
}

int UnaryOperationList::GetOperationIndex(char* name)
{
	for (int i = 0; i < operations->Count(); i++)
		if (strcmp(name, (char*)operations->GetAccessor(i)) == 0)
			return i;

	return -1;
}

char* UnaryOperationList::GetOperationName(int index)
{
	if (index < 0 || index >= operations->Count())
		return nullptr;

	char* name = (char*)operations->GetAccessor(index), *copy = new char[strlen(name) + 1];
	Copy(copy, name, strlen(name));

	return copy;
}
