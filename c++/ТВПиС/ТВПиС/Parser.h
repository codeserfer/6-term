#pragma once
#include "FileReader.h"
#include "FileWritter.h"
#include "List.h"

class Parser
{
private:
	FileReader* fileReader;
	FileWritter* fileWritter;

	List* parseCoditions;
	List* startConditions;
	int startConditionI;

	struct Condition
	{
		char* before;
		char* after;
		char* word;

		Condition(char* before, char* after, char* word)
		{
			this->before = new char[strlen(before) + 1];
			this->after = new char[strlen(after) + 1];
			this->word = new char[strlen(word) + 1];

			Copy(this->before, before, strlen(before));
			Copy(this->after, after, strlen(after));
			Copy(this->word, word, strlen(word));
		}
	};

	void Add(char*, char*, char*);

	List* Split(char*);
	char* ThreeConcat(char*, char*, char*);

public:

	Parser(char*);
	~Parser();
	void StringParse(char*);
	char* Find(char*, char*);
	char* GetStartContition();
	void ResetstartConditionI();
};