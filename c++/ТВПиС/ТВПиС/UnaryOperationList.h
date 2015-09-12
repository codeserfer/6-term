#pragma once
#include "List.h"
#include "Defaults.h"

class UnaryOperationList
{
	List* operations;

	UnaryOperationList(void);

	void LoadOperations();

public:
	static UnaryOperationList& Instance();
	~UnaryOperationList(void);

	bool IsOperation(char* name);
	int GetOperationIndex(char* name);
	char* GetOperationName(int index);
};

