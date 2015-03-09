#include "stdafx.h"
#include "Heap.h"
#include <iostream>
#include <exception>

using namespace std;

//����������� � ����� segmentSize
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

	// ����� �� ���� ��������� ������� ���������� �������
	Segment* i = current;
	while (i)
	{
		void* newBlock = i->PushBlock(size);
		if (newBlock != nullptr) return newBlock;
		i = i->prev;
	}

	// ���� ������ �� �������, �� �������� ����� ��������
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

//��������� ������ ��� ����� ��������
Heap::Segment* Heap::MakeSegment()
{
	Segment* temp = new Segment (current);
	current = temp;
	return current;

}

//�������� ���� ������� ������
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
