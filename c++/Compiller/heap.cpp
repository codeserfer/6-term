#include "stdafx.h"
#include "Heap.h"
#include <iostream>



Heap::Heap(int segmentSize = SEGMENTSIZE)
{
	this->segmentSize = segmentSize;
	this->current = 0;
}
Heap::~Heap(void)
{
}

void* Heap::GetMemory(int size)
{
	if (size > SEGMENTSIZE)
	{
		// ������������� ������ ������ �����������
		return nullptr;
	}

	// ����� � ������� � �� ���� ��������� ������� ���������� �������
	// ���� ������ �� �������, �� �������� ����� ��������
}

void Heap::FreeMemory(void* segment)
{

}

//��������� ������ ��� ����� ��������
int Heap::MakeSegment()
{
	Segment* temp = new Segment;

	temp->prev = current;

	//
	temp->data = malloc(sizeof(segmentSize));
	temp->descriptor[0].size = segmentSize;
	temp->descriptor[0].offset = 0x0;
	temp->descriptor[0].used = false;
	temp->DescriptorCount = 0;

	current = temp;
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
