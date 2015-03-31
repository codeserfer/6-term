#pragma once
#include "List.h"
class Hash{
public:
	Hash(int, int, int, int, int);
	~Hash();
	List* FindList(char*);
protected:
	virtual int Key1(char*);
	virtual int Key2(char*);
	virtual int Key3(char*);
	virtual int Key4(char*);
	virtual int Key5(char*);
	int CombineKeys(int, int, int, int, int);
private:
	List** table;
	int n1, n2, n3, n4, n5;
};

struct Article
{
	char* word;
	char* description;
};

class DictionList : public List
{
	DictionList() : List(sizeof(Article)) {};
	~DictionList(); // purge all Article data

	void Put(Article* article); // allocate memory for word & description
	Article* Find(char* word);
	void Del(Article* article) { Del(article->word); }
	void Del(char* word);
};

class Diction : public Hash
{
public:
	Diction() : Hash(33, 33, 0, 0, 0) {};
	~Diction();
	int Key1(char* key_word) {		int f = key_word[0] - 'A';
		if (f<33 && f>0) return f;
		else             return 0;	}
	int Key2(char* key_word) { return key_word[1] % 33; }

	Article* Find(char* word);
	Article* AutoCreate(char* word);
private:

};
