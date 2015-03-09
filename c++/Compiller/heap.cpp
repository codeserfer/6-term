#include "stdafx.h"
#include "Heap.h"
#include <iostream>
#include <exception>

using namespace std;

//Разобраться с тупым segmentSize
Heap::Heap(int segmentSize)
{
	//this->segmentSize = segmentSize;
	this->current = MakeSegment();
}
Heap::~Heap(void)
{
	Segment* i = current;
	while (i)
	{
		i->ClearSegment();
		i = i->prev;
	}
}

void* Heap::GetMemory(int size)
{
	if (size > SEGMENTSIZE || size < 0)
	{
		throw new bad_alloc();
	}

	// Поиск во всех страницах участка требуемого размера
	Segment* i = current;
	while (i)
	{
		void* newBlock = i->PushBlock(size);
		if (newBlock != nullptr) return newBlock;
		i = i->prev;
	}

	// Если такого не нашлось, то выделяем новую страницу
	i = MakeSegment();
	if (i->data!=nullptr)
	{
		i->PushBlock(size);
		return i;
	}
	
	return nullptr;
}


void Heap::FreeMemory(Segment* segment)
{
	Segment* i = current;
	Segment* prev = nullptr;
	while (i != segment)
	{
		prev = i;
		i = i->prev;
	}
	i->ClearSegment();
	if (prev) prev->prev = i->prev;
}

//Выделение памяти под новую страницу
Heap::Segment* Heap::MakeSegment()
{
	Segment* temp = new Segment (current);
	current = temp;
	return current;

}

//Удаление всех страниц памяти
void Heap::DeleteSegments()
{
	Segment* prev;

	while (current != nullptr)
	{
		prev = current->prev;
		delete prev;
		current = prev;
	}
}
