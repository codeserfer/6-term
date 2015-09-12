#include "stdafx.h"
#include "Diction.h"

Diction::~Diction()
{
	Hash::~Hash();
}


unsigned int Diction::Key1(char* keyWord)
{
	if (keyWord)
	{
		int hash = keyWord[0] - 'A';
			return (hash >= 0 && hash < 26) ? hash : 0;
	}

	return 0;
}

unsigned int Diction::Key2(char* keyWord)
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

Article* Diction::Find(char* word)
{
	List* list = nullptr;

	if (list = FindList(word))
	{
		Article* article = nullptr;

		for (int i = 0; i < list->Count(); i++)
		{
			// ctrcmp()
			if ((article = ((Article*)(list->GetAccessor(i))))->word == word)
			{
				return article;
			}
		}
	}

	return nullptr;
}

Article* Diction::Create(Article* article)
{
	if (article)
	{
		List* list = nullptr;

		if (!(list = table[CombineKeys(Key1(article->word), Key2(article->word))]))
		{
			list = new List(sizeof(Article));
		}

		return (Article*)(list->Add(new Article(article->word, article->description)));
	}

	return nullptr;
}

Article* Diction::Create(char* word, char* description)
{
	if (word)
	{
		return Create(new Article(word, description));
	}

	return nullptr;
}

Article* Diction::Delete(char* word)
{
	if (word)
	{
		List* list = FindList(word);

		if (list)
		{
			for (int i = 0; i < list->Count(); i++)
			{
				if (((Article*)(list->GetAccessor(i)))->word == word)
				{
					return (Article*)list->Get(i);
				}
			}
		}
	}
	return nullptr;
}

