#include "stdafx.h"
#include "TValue.h"
#include "iostream"

using namespace std;

TValue::TValue(double value, int type, int size)
{
	if (TypeList::Instance().GetTypeName(type))
	{
		this->type = type;

		if (size > 0)
		{
			this->pointer = new double[size];

			for (int i = 0; i < size; i++)
				this->pointer[i] = value;

			referenceType = true;
		}
		else
		{
			pointer = new double[1];
			*(this->pointer) = value;

			referenceType = false;
		}
	}
	else
	{
		throw Exceptions::UnknownType;
	}
}
TValue::TValue(double* pointer, int type)
{
	if (TypeList::Instance().GetTypeName(type))
	{
		this->pointer = pointer;
		this->type = type;
		this->referenceType = false;
	}
	else
	{
		throw Exceptions::UnknownType;
	}
}
TValue::TValue()
{
	this->pointer = nullptr;
	if (this->type = TypeList::Instance().GetTypeIndex("void") < 0)
	{
		throw Exceptions::UnknownType;
	}
}

TValue::~TValue()
{
	if (pointer && referenceType)
		delete pointer;

	pointer = nullptr;
}

int TValue::GetType()
{
	return type;
}

double TValue::GetValue(int index)
{
	if (referenceType)
	{
		return pointer[index];
	}

	return *pointer;
}
void TValue::SetValue(double v, int index)
{
	if (referenceType)
	{
		pointer[index] = v;
	}

	*pointer = v;
}

double* TValue::GetPointer()
{
	return pointer;
}
void TValue::SetPointer(double* pointer)
{
	this->pointer = pointer;
}

TValue* TValue::GetElementTValue(int index)
{
	if (referenceType)
		return new TValue(pointer + index, type);

	throw Exceptions::InvalidOperation;
}


bool TValue::IsReference()
{
	return referenceType;
}

void TValue::Print()
{
	if (referenceType)
	{
		std::cout << "Массив "<< pointer << " типа " << TypeList::Instance().GetTypeName(type) << std::endl;
	}
	else
	{
		std::cout << "Значение " << *pointer << " типа " << TypeList::Instance().GetTypeName(type) << std::endl;
	}
}