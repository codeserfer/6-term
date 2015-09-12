#pragma once
#include "List.h"
#include "Hash.h"
#include <string.h>
#include <iostream>
using namespace std;

struct Article
{
	char* word;
	char* description;

	Article(char* word, char* description)
	{
		this->word = new char[strlen(word)];
		this->description = new char[strlen(description)];

		strcpy_s(this->word, strlen(word), word);
		strcpy_s(this->description, strlen(description), description);
	}

	Article(const Article& article)
	{
		word = new char[strlen(article.word)];
		description = new char[strlen(article.description)];

		strcpy_s(word, strlen(article.word), article.word);
		strcpy_s(description, strlen(article.description), article.description);
	}
};

class Diction : public Hash
{
public:
	Diction() : Hash(25, 9, 0, 0, 0) { }
	~Diction();

	unsigned int Key1(char*);
	unsigned int Key2(char*);

	Article* Find(char*);

	Article* Create(Article*);

	Article* Create(char*, char* description = nullptr);

	Article* Delete(char*);
};

