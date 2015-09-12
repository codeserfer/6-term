#pragma once
#include "List.h"
#include "Defaults.h"

class OperationList
{
	List* operations;

	OperationList(void);

	void LoadOperations();

public:
	static OperationList& Instance();
	~OperationList(void);

	bool IsOperation(char* name);
	int GetOperationIndex(char* name);
	char* GetOperationName(int index);
};

