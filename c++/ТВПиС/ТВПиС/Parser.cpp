#include "stdafx.h"
#include "Parser.h"
#include "List.h"
#include "FileReader.h"

#define var auto

Parser::Parser(char* fileName)
{
	startConditionI = 0;
	parseCoditions = new List(sizeof(Condition));
	startConditions = new List(20);

	fileWritter = nullptr;//new FileWritter(LOG);

	fileReader = new FileReader(fileName);
	char* s;
	while ((s = fileReader->ReadLine()) != NULL)
	{
		StringParse(s);
	}
	delete fileReader;
}

Parser::~Parser()
{
	if (parseCoditions) delete parseCoditions;
	if (startConditions) delete startConditions;
	if (fileWritter) delete fileWritter;
}

char* Parser::ThreeConcat(char* s1, char* s2, char* s3)
{
	char* temp = new char[strlen(s1) + strlen(s2) + strlen(s3) + 1];
	Copy(temp, s1, strlen(s1));
	Copy(temp + strlen(s1), "\t", 1);
	Copy(temp + strlen(s1) + 1, s2, strlen(s2));
	Copy(temp + strlen(s1) + 1 + strlen(s2), "\t", 1);
	Copy(temp + strlen(s1) + 1 + strlen(s2) + 1, s3, strlen(s3));

	return temp;
}


void Parser::Add(char* before, char* after, char* word)
{
	var temp = new Condition(before, after, word);
	parseCoditions->Add(temp);
}

char* Parser::GetStartContition()
{
	if (startConditionI<startConditions->Count())
		return ((char*)startConditions->GetAccessor(startConditionI++));
	
	return nullptr;
}

void Parser::ResetstartConditionI()
{
	startConditionI = 0;
}

List* Parser::Split(char* s)
{
	char* word = new char[20];
	List* wordList = new List(20); //Список разделенных слов

	//Оператор запятая
	if (strlen(s) == 1 && s[0] == ',')
	{
		word[0] = ',';
		word[1] = '\0';

		wordList->Add(word);
		return wordList;
	}

	//Если escape-последовательность
	if (strlen(s) == 2 && s[0] == '\\')
	{
		if (s[1] == 'n') word[0] = '\n';
		if (s[1] == 't') word[0] = '\t';
		if (s[1] == '0') word[0] = '\0';
		word[1] = '\0';

		wordList->Add(word);
		return wordList;
	}

	//Если несколько символов или слов, разделенных запятой
	int startPosition = 0;
	int i = 0;
	for (int j = 0; i < strlen(s); i++)
	{
		if (s[i] == ',')
		{
			Copy(word, s + startPosition, i - startPosition);
			startPosition = i + 1;
			wordList->Add(word);
		}
	}
	Copy(word, s + startPosition, i - startPosition);
	wordList->Add(word);
	
	return wordList;
}

void Parser::StringParse(char* s)
{
	if (strlen(s) == 0) return;
	if (s[0] == '#') return; //Комментарии
	if (strlen(s)>5)
	{
		char* start = new char[6];
		Copy(start, s, 5);
		
		if (!strcmp(start, "start"))
		{
			char* startCondition = new char[strlen(s) - 6];
			Copy(startCondition, s+6, strlen(s)-6);
			startConditions->Add(startCondition);
			return;
		}
	}

	//Поиск позиций начала подстрок
	int positions[2];
	for (int i = 0, j = 0; i < strlen(s) && j < 2; i++)
	{
		if (s[i] == '\t') positions[j++] = i + 1;
	}

	char* before = new char[positions[0]];
	char* words = new char[positions[1] - positions[0]];
	char* after = new char[strlen(s) - positions[1]];

	Copy(before, s, positions[0] - 1);
	Copy(words, s + positions[0], positions[1] - positions[0] - 1);
	Copy(after, s + positions[1], strlen(s) - positions[1]);

	List* wordList = Split(words);

	int wordCount = wordList->Count();
	for (int i = 0; i < wordCount; i++)
	{
		char* temp = (char*)wordList->Get(0);
		Add(before, after, temp);
	}

	delete wordList;
}

char* Parser::Find(char* condition, char* word)
{
	Condition* temp = nullptr;

	for (int i = 0; i < parseCoditions->Count(); i++)
	{
		temp = (Condition*)parseCoditions->GetAccessor(i);
		if (!strcmp(temp->before, condition) && (!strcmp(temp->word, word)))
		{
			char* writeString = ThreeConcat(temp->before, temp->word, temp->after);

			if (fileWritter) fileWritter->WriteLine(writeString);

			return temp->after;
		}
		if (!strcmp(temp->before, condition) && (!strcmp(temp->word, "...")))
		{
			char* writeString = ThreeConcat(temp->before, word, temp->after);

			if (fileWritter) fileWritter->WriteLine(writeString);

			return temp->after;
		}
	}
}