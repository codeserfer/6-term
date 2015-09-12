#include "stdafx.h"
#include "TypeList.h"
#include <cstring>
#include "defaults.h"
#include "Exceptions.h"

TypeList::TypeList(void)
{
	types = new List(MAX_TYPE_NAME_LENGHT);
	AddPrimitiveTypes();
}


TypeList::~TypeList(void)
{
	delete types;
}

TypeList& TypeList::Instance()
{
	static TypeList _instance;
	return _instance;
}

void TypeList::AddPrimitiveTypes()
{
	types->Add((void*)"int");
	types->Add((void*)"float");
	types->Add((void*)"double");
	//types->Add((void*)"void");
	//types->Add((void*)"wchar_t");
	types->Add((void*)"bool");
	types->Add((void*)"char");
}

void TypeList::AddUserType(char* name)
{
	if (name != nullptr && strlen(name) > MAX_TYPE_NAME_LENGHT && !IsType(name)) //@
		throw Exceptions::TypeNameLengthError;

	types->Add(name);
}

bool TypeList::IsType(char* name)
{
	//for (int i = 0; i < types->Count(); i++)
	//	if (strcmp(name, (char*)types->GetAccessor(i)) == 0)
	//		return true;

	//return false;

	return GetTypeIndex(name) < 0 ? false : true;
}

int TypeList::GetTypeIndex(char* name)
{
	for (int i = 0; i < types->Count(); i++)
		if (strcmp(name, (char*)types->GetAccessor(i)) == 0)
			return i;

	return -1;
}

char* TypeList::GetTypeName(int index)
{
	if (index < 0 || index >= types->Count())
		return nullptr;

	char* name = (char*)types->GetAccessor(index), * copy = new char[strlen(name) + 1];
	Copy(copy, name, strlen(name));

	return copy;
}

