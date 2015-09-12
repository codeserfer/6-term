#include "stdafx.h"
#include "OperationList.h"
#include <cstring>
#include "defaults.h"
#include "Exceptions.h"

OperationList::OperationList(void)
{
	operations = new List(MAX_OPERATION_NAME_LENGHT);
	LoadOperations();
}


OperationList::~OperationList(void)
{
	delete operations;
}

OperationList& OperationList::Instance()
{
	static OperationList _instance;
	return _instance;
}

void OperationList::LoadOperations()
{
	operations->Add((void*)"=");
	operations->Add((void*)"+"); //
	operations->Add((void*)"-"); //
	operations->Add((void*)"*"); //
	operations->Add((void*)"/");
	operations->Add((void*)"%");
	operations->Add((void*)"î++"); //
	operations->Add((void*)"î--"); //
	operations->Add((void*)"++î"); //
	operations->Add((void*)"--î"); //

	operations->Add((void*)"==");
	operations->Add((void*)"!=");
	operations->Add((void*)">");
	operations->Add((void*)"<");
	operations->Add((void*)">=");
	operations->Add((void*)"<=");

	operations->Add((void*)"!");
	operations->Add((void*)"&&");
	operations->Add((void*)"||");

	operations->Add((void*)"~");
	operations->Add((void*)"&");
	operations->Add((void*)"|");
	operations->Add((void*)"^");
	operations->Add((void*)"<<");
	operations->Add((void*)">>");

	operations->Add((void*)"+=");
	operations->Add((void*)"-=");
	operations->Add((void*)"*=");
	operations->Add((void*)"/=");
	operations->Add((void*)"%=");
	operations->Add((void*)"&=");
	operations->Add((void*)"|=");
	operations->Add((void*)"^=");
	operations->Add((void*)"<<=");
	operations->Add((void*)">>=");

	operations->Add((void*)"[]");
	operations->Add((void*)"&");
	operations->Add((void*)"->");
	operations->Add((void*)".");
	operations->Add((void*)"->*");
	operations->Add((void*)".*");

	operations->Add((void*)"()");
	operations->Add((void*)",");
	operations->Add((void*)";"); //
	operations->Add((void*)"?:");
	operations->Add((void*)"::");
	operations->Add((void*)"sizeof");
	operations->Add((void*)"alignof");
	operations->Add((void*)"typeid");
	operations->Add((void*)"new");
	operations->Add((void*)"delete");

	operations->Add((void*)"input");
	operations->Add((void*)"output");

	operations->Add((void*)"sin");
	operations->Add((void*)"cos");
	operations->Add((void*)"min");
	operations->Add((void*)"max");
}

bool OperationList::IsOperation(char* name)
{
	return GetOperationIndex(name) < 0 ? false : true;
}

int OperationList::GetOperationIndex(char* name)
{
	for (int i = 0; i < operations->Count(); i++)
		if (strcmp(name, (char*)operations->GetAccessor(i)) == 0)
			return i;

	return -1;
}

char* OperationList::GetOperationName(int index)
{
	if (index < 0 || index >= operations->Count())
		return nullptr;

	char* name = (char*)operations->GetAccessor(index), *copy = new char[strlen(name) + 1];
	Copy(copy, name, strlen(name));

	return copy;
}
