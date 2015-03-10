#include "stdafx.h"
#include "Segment.h"
#include "Heap.h"
#include <iostream>
#include <exception>

using namespace std;

Heap::Heap(int segmentSize)
{
	this->segmentSize = segmentSize;
	this->current = MakeSegment();
}

Heap::~Heap(void)
{
	Segment* i = current;
	while (i)
	{
		//i->ClearSegment();//деструктор delete
		delete i;
		i = i->prev;
	}
}

void* Heap::GetMemory(int size)
{
	if (size > SEGMENTSIZE || size < 0)
	{
		throw new bad_alloc();
	}

	Segment* i = current;
	while (i)
	{
		void* newBlock = i->PushBlock(size);
		if (newBlock != nullptr) return newBlock;
		i = i->prev;
	}

	i = MakeSegment();
	//if (i->data!=nullptr)//@Что
	//{
		i->PushBlock(size);
		return i;
	//}
	
	//return nullptr;
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
	//i->ClearSegment();//~
	delete i;
	if (prev) prev->prev = i->prev;
}

Segment* Heap::MakeSegment()
{
	Segment* temp = new Segment(current, segmentSize);
	current = temp;
	return current;
}

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
