#pragma once
#include "List.h"
#include "Defaults.h"

class TypeList
{
	List* types;

	TypeList(void);

	void AddPrimitiveTypes();

public:
	static TypeList& Instance();
	~TypeList(void);

	void AddUserType(char* name);
	bool IsType(char* name);
	int GetTypeIndex(char* name);
	char* GetTypeName(int index);
};

