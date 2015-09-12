#pragma once
#include "VariableTable.h"

class Scope
{
	VariableTable* _table;
	Scope*	_parent;

public:
	Scope(Scope* parent = nullptr);
	~Scope();

	void Push(TValueKeeper* keeper);
	TValueKeeper* Find(char* name);
	TValueKeeper* FindInCurrentScope(char*name);
	Scope* GetParent();

	friend class TList;
};

