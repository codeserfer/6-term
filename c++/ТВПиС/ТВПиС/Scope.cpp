#include "stdafx.h"
#include "Scope.h"


Scope::Scope(Scope* parent)
{
	_table = new VariableTable();
	_parent = parent;
}


Scope::~Scope()
{
	if (_table) delete _table;

	_table = nullptr;
	_parent = nullptr;
}

Scope* Scope::GetParent()
{
	return _parent;
}

void Scope::Push(TValueKeeper* keeper)
{
	_table->Push(keeper);
}

TValueKeeper* Scope::Find(char* name)
{
	Scope* tempscope = this;
	while (tempscope != nullptr)
	{
		TValueKeeper* keeper = tempscope->_table->Find(name);

		if (keeper)
			return keeper;

		tempscope = tempscope->_parent;
	}

	return nullptr;
}

TValueKeeper* Scope::FindInCurrentScope(char*name)
{
	return _table->Find(name);
}