#include "FileReader.h"
#include "Automat.h"
#include "TNode.h"
#include "Hash.h"
#include "Exceptions.h"
#include "Scope.h"
#include "ScopeTree.h"
#include "VariableTable.h"
#include "TValueKeeper.h"
#include  "UnaryOperationList.h"
#include "BinaryOperationList.h"
#include <iostream>

#pragma once
class Scope;

enum TNodeType
{
	Const,
	Variable,
	Array
};

class ASTBuilder
{
private:

	struct NewToken
	{
		Automat::Token type;
		char* string;
		double value;
		int priority;
		int lineIndex;

		NewToken(Automat::Token type, char* string, double value, int priority, int lineIndex)
		{
			this->type = type;
			this->string = string;
			this->priority = priority;
			this->value = value;
			this->lineIndex = lineIndex;
		}
	};

	TList* astRoot;
	List* tokens;

	Scope* currentScope;
	TList* currentList;

	static bool IsFraction(char* s);
	static double StrToDouble(char* s);

	int ClosingBracketIndex(int openingBracketIndex);
	int FindOperationWithMinimalPriority(int start, int finish);
	void Prioritize(List*);
	int FindToken(int, int, char*);
	bool isOperator(Automat::Token);
	bool IsAnExpression(int start, int end);
	List* OldTokenToNewToken(List*);
	TNode* ParseLine(int, int);

	TNode* ParseSingleTokenLine(int index);
	void ParseConstInitialization(int& index);
	void ParseInitialization(int& index);
	TNode* ParseIfElse(int& index);
	TNode* ParseWhile(int& index);
	TNode* ParseVariableName(int& index, TNodeType& type);
	void ParseMultiLine(int, int);

public:
	ASTBuilder(char*);
	~ASTBuilder();

	void Build();
	void Run();
	void PrintList(List*);
};

