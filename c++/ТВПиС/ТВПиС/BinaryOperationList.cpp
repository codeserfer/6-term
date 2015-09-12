#include "stdafx.h"
#include "BinaryOperationList.h"
#include <cstring>
#include "defaults.h"
#include "Exceptions.h"

BinaryOperationList::BinaryOperationList(void)
{
	operations = new List(MAX_OPERATION_NAME_LENGHT);
	LoadOperations();
}


BinaryOperationList::~BinaryOperationList(void)
{
	delete operations;
}

BinaryOperationList& BinaryOperationList::Instance()
{
	static BinaryOperationList _instance;
	return _instance;
}

void BinaryOperationList::LoadOperations()
{
	// Операции присваивания
	operations->Add((void*)"=");	// Присваивание

	// Арифметические операции
	operations->Add((void*)"+");	// Сложение
	operations->Add((void*)"-");	// Вычитание
	operations->Add((void*)"*");	// Умножение
	operations->Add((void*)"/");	// Деление
	operations->Add((void*)"%");	// Остаток от деления

	//Операции сравнения и логические операции
	operations->Add((void*)"&&");	// Логическое И
	operations->Add((void*)"||");	// Логическое ИЛИ

	operations->Add((void*)"==");	// Проверка на равенство
	operations->Add((void*)"!=");	// Проверка на неравенство
	operations->Add((void*)">");	// Больше
	operations->Add((void*)"<");	// Меньше
	operations->Add((void*)">=");	// Больше или равно
	operations->Add((void*)"<=");	// Меньше или равно
	
	//Побитовые операции
	operations->Add((void*)"&");	// Побитовое И
	operations->Add((void*)"|");	// Побитовое ИЛИ
	operations->Add((void*)"<<");	// Битовый сдвиг влево
	operations->Add((void*)">>");	// Битовый сдвиг право

	operations->Add((void*)"[]");	// Квадратные скобки

	// Минимум и максимум
	operations->Add((void*)"min");	// Минимум
	operations->Add((void*)"max");	// Максимум
}

bool BinaryOperationList::IsOperation(char* name)
{
	return GetOperationIndex(name) < 0 ? false : true;
}

int BinaryOperationList::GetOperationIndex(char* name)
{
	for (int i = 0; i < operations->Count(); i++)
		if (strcmp(name, (char*)operations->GetAccessor(i)) == 0)
			return i;

	return -1;
}

char* BinaryOperationList::GetOperationName(int index)
{
	if (index < 0 || index >= operations->Count())
		return nullptr;

	char* name = (char*)operations->GetAccessor(index), *copy = new char[strlen(name) + 1];
	Copy(copy, name, strlen(name));

	return copy;
}
