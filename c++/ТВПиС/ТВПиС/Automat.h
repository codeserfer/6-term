#pragma once
#include "Parser.h"

using namespace std;

class Automat
{
public:
	enum Token
	{
		String,					//0
		SingleLineComment,		//1
		MultilineComment,		//2
		Digit,					//3
		Char,					//4
		Bracket,				//5
		ReservedWord,			//6
		Star,					//7
		CompOper,				//8
		LogicOper,				//9
		BiteOp,					//10
		Assignment,				//11
		SysFunction,			//12
		Separator,				//13
		UserType,				//14
		Space,					//15
		NewLine,				//16
		ReservedType,			//17
		Condition,				//18
		ApOp,					//19
		IncOrDec				//20
	};


private:
	Parser* parser;


	

	Token StrToToken(char*);
	bool IsLetter(char);
	char* oneCharString(char);

public:

	

	struct ParseResult
	{
		Token token;
		char* string;


		ParseResult(Token token, char* string)
		{
			this->token = token;
			this->string = string;
		}
	};

	Automat(char*);
	~Automat();

	int WordsAutomat(char*, char*, char*);
	ParseResult* StringParse(char* s);

	List* Work(char*);
};