#include "stdafx.h"
#include "List.h"
#include <iostream> //!!!


List::List(int elementSize, int elementCount)
{
	this->elementSize = elementSize;
	this->elementCount = elementCount;
	this->error = false;
	this->segmentCount = 0;
	this->first = this->last = nullptr;
	this->firstIndex = this->lastIndex = 0;
	NewSegment();
}


List::~List()
{
	Segment* i = first;
	while (i)
	{
		DeleteSegment(i);
		i = i->next;
	}
	first = last = nullptr;
	firstIndex = lastIndex = 0;
	elementSize = elementCount = 0;
	segmentCount = 0;
}

void* List::Get(int pos)
{
	return nullptr; //Заглушка
}


List::Segment* List::GetSegment (int id)
{
	Segment* i = first;
	int n = 0;
	if (id > segmentCount) return nullptr;

	while (n != id)
	{
		i = i->next;
		n++;
	}
	return i;
}

void List::CopyElement(void* destination, void* source)
{
	for (int i = 0; i < elementSize; i++)
		*((char*)destination + i) = *((char*)source + i);
}

void List::Add(void* data)
{
	//int segmentNumber = lastIndex ? lastIndex / elementCount : 0;

	int segmentNumber = lastIndex / elementCount;
	int cell = lastIndex % elementCount;
	Segment* segment = GetSegment(segmentNumber);
	//!!!Проверить, есть ли вообще место в данном сегменте, или надо выделить новый?

	char* offset = offset = (char*)segment->data + ((cell/* + 1*/) * elementSize); //!!! +1 или нет? Надо проверить формулу!
	CopyElement(offset, data);
	/*for (int i = 0, j = cell*elementSize; i < elementSize; i++, j++)
	{
		offset[j] = ((char*)data)[i];
	}*/
	lastIndex++;
}

void List::TakeFirst(void* store)
{
	char* source = (char*)first->data + firstIndex*elementSize;
	CopyElement(store, source);
	elementCount--;
	firstIndex++;
}

void List::TakeLast(void* store)
{

}

void List::Take(int pos, void* store)
{
	if (pos == 0)
	{
		TakeFirst(store);
		return;
	}

	pos += firstIndex;
	Segment* currentSegment = GetSegment(pos / elementCount);
	int elementIndex = pos % elementCount;

	char* source = (char*)currentSegment->data + elementIndex*elementSize;
	CopyElement(store, source);

	char* destination;

	while (pos != lastIndex)
	{
		destination = (char*)currentSegment->data + elementIndex*elementSize;

		if (elementIndex < elementCount - 1)
			source = (char*)currentSegment->data + (elementIndex + 1)*elementSize;
		else
			source = (char*)currentSegment->next->data;

		CopyElement(destination, source);
		elementIndex++;
		pos++;

		if (elementIndex == elementCount)
		{
			currentSegment = currentSegment->next;
			elementIndex = 0;
		}
	}

	if (currentSegment->next != nullptr && currentSegment->next->next != nullptr) //???
		DeleteSegment(currentSegment->next->next);

	lastIndex--;
}

void List::Sort(bool dir = true, int method = 0)
{

}

int List::Count()
{
	return (lastIndex - firstIndex + 1);
}



bool List::Error()
{
	return error;
}

void List::NewSegment()
{
	if (first == nullptr)
	{
		first = last = new Segment();
		first->next = first->prev = nullptr;
		first->data = Heap::Instance().GetMemory(elementCount*elementSize);
		
	}
	else
	{
		Segment* temp = new Segment();
		last->next = temp;
		temp->prev = last;
		last = temp;
		temp->data = Heap::Instance().GetMemory(sizeof(elementCount*elementSize));
	}
	segmentCount++;
}

void List::DeleteSegment(Segment* segment)
{
	if (segment->prev) segment->prev->next = segment->next;
	Heap::Instance().FreeMemory(segment->data);
	segmentCount--;
}