#include "stdafx.h"
#include "Automat.h"
#include "Parser.h"
#include "Exceptions.h"
#include <iostream>

Automat::Token Automat::StrToToken(char* s)
{
	if (!strcmp(s, "SLZSymbolComment"))
		return Token::SingleLineComment;

	if (!strcmp(s, "MLZSymbolComment"))
		return Token::MultilineComment;

	if (!strcmp(s, "zeroSymbolString"))
		return Token::String;

	if (!strcmp(s, "isreservedWord"))
		return Token::ReservedWord;

	if (!strcmp(s, "zeroSymbolDigit"))
		return Token::Digit;

	if (!strcmp(s, "zeroSymbolChar"))
		return Token::Char;

	if (!strcmp(s, "isbracket"))
		return Token::Bracket;

	if (!strcmp(s, "isStar"))
		return Token::Star;

	if (!strcmp(s, "isCompOper"))
		return Token::CompOper;

	if (!strcmp(s, "isLogicOper"))
		return Token::LogicOper;

	if (!strcmp(s, "isBiteOp"))
		return Token::BiteOp;

	if (!strcmp(s, "isAssignment"))
		return Token::Assignment;

	if (!strcmp(s, "isSeparator"))
		return Token::Separator;

	if (!strcmp(s, "isUserType"))
		return Token::UserType;

	if (!strcmp(s, "isSpace"))
		return Token::Space;

	if (!strcmp(s, "isNewLine"))
		return Token::NewLine;

	if (!strcmp(s, "isReservedType"))
		return Token::ReservedType;

	if (!strcmp(s, "isCondition"))
		return Token::Condition;

	if (!strcmp(s, "isSysFunc"))
		return Token::SysFunction;

	if (!strcmp(s, "isArOp"))
		return Token::ApOp;

	if (!strcmp(s, "isIncOrDec"))
		return Token::IncOrDec;

}

bool Automat::IsLetter(char c)
{
	if ((c >= (char)'a') && (c <= (char)'z')) return true;
	if ((c >= (char)'A') && (c <= (char)'Z')) return true;
	
	return false;
}

Automat::Automat(char* fileName)
{
	parser = new Parser(fileName);
}

Automat::~Automat()
{
	if (parser) delete parser;
}

int Automat::WordsAutomat(char* s, char* startCondition, char* endCondition)
{
	char* currentCondition = startCondition;
	int i = 0;
	for (; i < strlen(s) && IsLetter(s[i]); i++);
	char* temp = new char[i];
	Copy(temp, s, i);
	currentCondition = parser->Find(currentCondition, temp);

	if (!strcmp(currentCondition, endCondition))
	{
		return i;
	}

	return 0;
}

char* Automat::oneCharString(char c)
{
	char* temp = new char[2];
	temp[0] = c;
	temp[1] = '\0';
	return temp;
}

Automat::ParseResult* Automat::StringParse(char* s)
{
	parser->ResetstartConditionI();
	char* startCondition = parser->GetStartContition();

	//Перебираем все автоматы
	while (startCondition)
	{
		//Отдельный автомат для зарезервированных слов
		if (!strcmp(startCondition, "isReservedWord"))
		{
			int lenght = WordsAutomat(s, startCondition, "reservedWord");
			if (lenght == 0)
			{
				startCondition = parser->GetStartContition();
				continue;
			}

			char* word = new char[lenght];
			Copy(word, s, lenght);
			return new ParseResult(Token::ReservedWord, word);

		}

		//Отдельный автомат для системных функций
		if (!strcmp(startCondition, "isSysFunc"))
		{
			int lenght = WordsAutomat(s, startCondition, "sysFunc");
			if (lenght == 0)
			{
				startCondition = parser->GetStartContition();
				continue;
			}

			char* word = new char[lenght];
			Copy(word, s, lenght);
			return new ParseResult(Token::SysFunction, word);
		}

		//Отдельный автомат для стандартных типов данных
		if (!strcmp(startCondition, "isReservedType"))
		{
			int lenght = WordsAutomat(s, startCondition, "reservedType");
			if (lenght == 0)
			{
				startCondition = parser->GetStartContition();
				continue;
			}

			char* word = new char[lenght];
			Copy(word, s, lenght);
			return new ParseResult(Token::ReservedType, word);

		}

		//Отдельный автомат для условных конструкций
		if (!strcmp(startCondition, "isCondition"))
		{
			int lenght = WordsAutomat(s, startCondition, "condition");
			if (lenght == 0)
			{
				startCondition = parser->GetStartContition();
				continue;
			}

			char* word = new char[lenght];
			Copy(word, s, lenght);
			return new ParseResult(Token::Condition, word);

		}

		char* currentCondition = startCondition;
		int fragmentLenght = 0;

		//Для посимвольных автоматов
		for (int i = 0; i < strlen(s); i++)
		{
			char* temp = oneCharString(s[i]);

			currentCondition = parser->Find(currentCondition, temp);


			if (strcmp(currentCondition, "error")) fragmentLenght++;

			if (!strcmp(currentCondition, "error0"))
			{
				break;
			}

			if (!strcmp(currentCondition, "error") || i == strlen(s) - 1)
			{
				if (fragmentLenght != 0)
				{
					char* word = new char[fragmentLenght];
					Copy(word, s, fragmentLenght);
					return new ParseResult(StrToToken(startCondition), word);
				}

				else
				{
					break;
				}
			}


			if (!strcmp(currentCondition, "charError"))
			{
				throw Exceptions::CharError;
			}

			if (!strcmp(currentCondition, "stringerror"))
			{
				throw Exceptions::StringError;
			}
		}

		startCondition = parser->GetStartContition();
	}
}

List* Automat::Work(char* s)
{

	ParseResult* result;

	List* tokens = new List(sizeof(ParseResult));

	while (strlen(s))
	{
		result = StringParse(s);

		tokens->Add(result);

		char* temp = new char[strlen(s) - strlen(result->string) + 1];
		Copy(temp, s + strlen(result->string), strlen(s) - strlen(result->string));
		delete[] s;
		s = temp;
	}

	return tokens;
}