#pragma once

#include "TValue.h"
#include "TypeList.h"
#include "OperationList.h"
#include "Exceptions.h"
#include "Scope.h";
#include <iostream>

// Базовый класс для всех узлов и листьев синтаксического дерева.
class TNode
{
public:
	virtual TValue* Execute() = 0;
};

// Константа.
class TConst : public TNode
{
public:
	TConst(TValue* c);
	~TConst();
	TValue* GetTValue();

	double GetValue();
	int GetType();
	TValue* Execute();

protected:
	TValue* _const;
};

// Переменная.
class TVariable : public TNode
{
public:
	TVariable(TValue* v);
	~TVariable();
	TValue* GetTValue();

	double GetValue();
	void SetValue(double);
	int GetType();
	TValue* Execute();

protected:
	TValue* _variable;
};

// Массив.
class TArray : public TNode
{
public:
	TArray(TValue* array, TNode* index);
	~TArray();

	TValue* Execute();

protected:
	TValue* _array;
	TNode* _index;
};

// Унарная операция.
class TUnaryOperation : public TNode
{
public:
	TUnaryOperation(int, TNode*);
	~TUnaryOperation();

	TValue* Execute();

protected:
	int      _type;		// # в таблице операций
	TNode*   _operand;
};

// Бинарная операция.
class TBinaryOperation : public TNode
{
public:
	TBinaryOperation(int, TNode*, TNode*);
	~TBinaryOperation();

	TValue* Execute();

protected:
	int     _type;
	TNode*   _left;
	TNode*   _right;
};

// Список независимых операций.
class TList : public TNode
{
	struct ListNode
	{
		TNode* _expression;
		ListNode* _next;
	};

public:
	TList(TList* parent = nullptr);
	~TList();

	TValue* Execute();
	void  Push(TNode*);

	Scope* GetScope();

protected:
	ListNode* first;
	Scope* scope;
};

// Конструкция IF-ELSE.
class TIf : public TNode
{
public:
	TIf(TNode*, TList*, TList*);
	~TIf();

	TValue* Execute();

protected:
	TNode* _condition;
	TList* _then;
	TList* _else;
};

// Конструкция WHILE.
class TWhile : public TNode
{
public:
	TWhile(TNode*, TList*);
	~TWhile();

	TValue* Execute();

protected:
	TNode* _condition;
	TList* _body;
};