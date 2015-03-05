#include "stdafx.h"
#include "Heap.h"
#include <iostream>
#include <exception>

using namespace std;

Heap::Heap(int segmentSize)
{
	//this->segmentSize = segmentSize;
	this->current = 0;
}
Heap::~Heap(void)
{
}

void* Heap::GetMemory(int size)
{
	if (size > SEGMENTSIZE || size < 0)
	{
		throw new bad_alloc();
	}

	// Поиск в текущей и во всех страницах участка требуемого размера
	// Если такого не нашлось, то выделяем новую страницу
			return nullptr;

}

void Heap::FreeMemory(void* segment)
{

}

//Выделение памяти под новую страницу
int Heap::MakeSegment()
{
	//Segment* temp = new Segment;

	//temp->prev = current;

	////
	//temp->data = malloc(sizeof(segmentSize));
	//temp->descriptor[0].size = segmentSize;
	//temp->descriptor[0].offset = 0x0;
	//temp->descriptor[0].used = false;
	//temp->DescriptorCount = 0;

	//current = temp;

			return 0;

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
