#pragma once
#include "List.h"

#define NUMBER_OF_KEYS 5 

class Hash
{
public:
	Hash(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
	~Hash();

	List* FindList(char*);

protected:
	virtual unsigned int Key1(char*);
	virtual unsigned int Key2(char*);
	virtual unsigned int Key3(char*);
	virtual unsigned int Key4(char*);
	virtual unsigned int Key5(char*);

	unsigned int CombineKeys(unsigned int key1 = 0, unsigned int key2 = 0, unsigned int key3 = 0, unsigned int key4 = 0, unsigned int key5 = 0);

protected:
	List** table;
	int n1, n2, n3, n4, n5;
	int capacity;
};



//struct Article
//{
//	char* word;
//	char* description;
//};
//
//
//
//class DictionList : public List
//
//{
//
//	DictionList() : List(sizeof(Article)) {};
//
//	~DictionList(); // purge all Article data
//
//
//
//	void Put(Article* article); // allocate memory for word & description
//
//	Article* Find(char* word);
//
//	void Del(Article* article) { Del(article->word); }
//
//	void Del(char* word);
//
//};
//
//
//
//class Diction : public Hash
//
//{
//
//public:
//	Diction() : Hash(33, 33, 0, 0, 0) {};
//	~Diction();
//
//
//	int Key1(char* key_word) {
//		int f = key_word[0] - 'A';
//
//		if (f<33 && f>0) return f;
//
//		else             return 0;
//	}
//
//	int Key2(char* key_word) { return key_word[1] % 33; }
//
//
//
//	Article* Find(char* word);
//
//	Article* AutoCreate(char* word);
//
//private:
//
//
//
//};
