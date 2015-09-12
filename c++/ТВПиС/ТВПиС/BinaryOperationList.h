#pragma once
#include "List.h"
#include "Defaults.h"

class BinaryOperationList
{
	List* operations;

	BinaryOperationList(void);

	void LoadOperations();

public:
	static BinaryOperationList& Instance();
	~BinaryOperationList(void);

	bool IsOperation(char* name);
	int GetOperationIndex(char* name);
	char* GetOperationName(int index);
};

