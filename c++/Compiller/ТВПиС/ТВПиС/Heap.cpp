#include "stdafx.h"
#include "Segment.h"
#include "Heap.h"
#include <iostream>
#include <exception>
#include <fstream> //ÓÁÐÀÒÜ

using namespace std;

Heap& Heap::Instance()
{
	static Heap _instance;
	return _instance;
}

Heap::Heap(int segmentSize)
{
	this->segmentSize = segmentSize;
	this->current = MakeSegment();
}

Heap::~Heap(void)
{
	DeleteSegments();
	current = nullptr;
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

		////
		ofstream fout("output.txt", ios_base::app);
		fout << newBlock << " " << size << " "  << (void*)((char*)newBlock + size) << endl;
		fout.close();

		////


		if (newBlock != nullptr) return newBlock;
		i = i->prev;
	}

	i = MakeSegment();
	//if (i->data!=nullptr)//@×òî
	//{
		i->PushBlock(size);
		return i;
	//}
	
	//return nullptr;
}

void Heap::FreeMemory(void* offset)
{
	current->RemoveBlock(offset);
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
	//oO
	/*Segment* prev;

	while (current != nullptr)
	{
		prev = current->prev;
		delete prev;
		current = prev;
	}*/
	Segment* i = current;
	while (i)
	{
		Segment* temp = i;
		i = i->prev;
		delete temp;
	}

}
