#pragma once
#include "TValue.h"

class TValueKeeper
{
	char* _name;
	TValue* _value;
	bool _const;

public:
	TValueKeeper(char* name, TValue* value, bool _const = false);

	~TValueKeeper();

	char* GetName();

	TValue* GetValue();
	void SetValue(TValue*);

	bool IsConst();
	bool IsReference();
};

