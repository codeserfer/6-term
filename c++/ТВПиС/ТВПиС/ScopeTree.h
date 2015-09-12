#pragma once
#include "Scope.h"

class ScopeTree
{
	Scope* _root;

public:
	ScopeTree();
	~ScopeTree();

	Scope* Create(Scope* parent);
};

