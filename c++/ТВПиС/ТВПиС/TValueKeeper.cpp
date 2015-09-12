#include "stdafx.h"
#include "TValueKeeper.h"


TValueKeeper::TValueKeeper(char* name, TValue* value, bool _const)
{
	this->_name = name;
	this->_value = value;
	this->_const = _const;
}

TValueKeeper::~TValueKeeper()
{
	if (_name) delete[] _name;
	if (_value) delete[] _value;

	_name = nullptr;
	_value = nullptr;
	_const = false;
}

char* TValueKeeper::GetName()
{
	return _name;
}

TValue* TValueKeeper::GetValue()
{
	return _value;
}

void TValueKeeper::SetValue(TValue* value)
{
	if (_const == true)
		throw Exceptions::ConstInitializationError;

	this->_value = value;
}

bool TValueKeeper::IsConst()
{
	return _const;
}

bool TValueKeeper::IsReference()
{
	return _value->IsReference();
}