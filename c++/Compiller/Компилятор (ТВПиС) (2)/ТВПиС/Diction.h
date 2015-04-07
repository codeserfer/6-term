#pragma once
#include "List.h"
#include "Hash.h"
#include <string.h>

using namespace std;

struct Article
{
	char* word;
	char* description;

	Article(const Article& article)
	{
		word = new char[strlen(article.word)]; // null? +
		description = new char[strlen(article.description)];

		strcpy(word, article.word);
		strcpy(description, article.description);
	}
};

class DictionList : public List
{
public:
	DictionList() : List(sizeof(Article)){ }
	~DictionList();

	void Create(Article* article)
	{
		Delete(article->word);
		Add(article);
	}

	Article* Find(char* word)
	{
		Article* article = nullptr;
		for (int i = 0; i < Count() && (article = ((Article*)(this->GetAccessor(i))))->word != word; i++);
		return article;
	}
	
	int FindPosition(char* word)
	{
		for (int i = 0; i < Count(); i++)
		{
			if (((Article*)(this->GetAccessor(i)))->word == word)
			{
				return i;
			}
		}		
	}

	Article* Delete(Article* article) { return Delete(article->word); }
	Article* Delete(char* word)
	{
		for (int i = 0; i < Count(); i++)
		{
			if (((Article*)(this->GetAccessor(i)))->word == word)
			{
				return (Article*)Get(i);
			}
		}
	}
};

class Diction : public Hash
{
public:
	Diction() : Hash(25, 9, 0, 0, 0) { }
	~Diction();

	unsigned int Key1(char* keyWord)
	{
		if (keyWord)
		{
			int hash = keyWord[0] - 'A';
			return (hash >= 0 && hash < 26) ? hash : 0;
		}

		return 0;
	}
	unsigned int Key2(char* keyWord)
	{
		if (keyWord)
		{
			int hash = 0;
			for (int i = 0; keyWord[i]; i++)
				hash++;

			return hash % 10;
		}

		return 0;
	}

	Article* Find(char* word)
	{
		DictionList* list = nullptr;

		if (list = (DictionList*)FindList(word))
		{
			return list->Find(word);
		}

		return nullptr;
	}

	Article* Create(Article* arcticle)
	{
		DictionList* list = nullptr;

		if (!(list = table[CombineKeys(Key1(arcticle->word), Key2(arcticle->word))]))
		{
			list = new DictionList();
		}

		list->Create(arcticle); // чтобы Create возвращал ссылку на созданный объект

		return nullptr; //@
	}

	Article* AutoCreate(char* word)
	{
		return Create(new Article(word, nullptr)); // x_x
	}

	DictionList** table;
};

