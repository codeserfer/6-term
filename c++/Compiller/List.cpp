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
	void* data = Heap::Instance().GetMemory(sizeof(elementSize));
	Take(pos, data);
	return data;
}


List::Segment* List::GetSegment (int id)
{
	Segment* i = first;
	int n = 0;
	if (id > segmentCount || id < 0) return nullptr;

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
	int segmentNumber = lastIndex / elementCount;
	int cell = lastIndex % elementCount;

	Segment* segment;
	if (cell == 0)
	{
		segment = NewSegment();
		cell = 0;
	}
	else segment = GetSegment(segmentNumber);

	char* offset = offset = (char*)segment->data + ((cell) * elementSize);
	CopyElement(offset, data);
	lastIndex++;
}

void List::TakeFirst(void* store)
{
	if (Count() == 0)
	{
		store = nullptr;
		return;
	}
	char* source = (char*)first->data + firstIndex*elementSize;
	CopyElement(store, source);
	firstIndex++;

	if (firstIndex == elementCount)
	{
		firstIndex -= elementCount;
		lastIndex -= elementCount;
		DeleteSegment(first);
	}
}

void List::TakeLast(void* store)
{
	if (Count() == 0)
	{
		store = nullptr;
		return;
	}
	char* source = (char*)last->data + ((lastIndex-1) % elementCount)*elementSize;
	CopyElement(store, source);
	lastIndex--;

	if (lastIndex % elementCount == 0)
	{
		DeleteSegment(last);
	}
}

void List::Take(int pos, void* store)
{
	if (Count() == 0)
	{
		store = nullptr;
		return;
	}
	if (pos == 0)
	{
		TakeFirst(store);
		return;
	}
	if (pos == firstIndex + lastIndex-1)
	{
		TakeLast(store);
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

	if (IsFree(currentSegment)) DeleteSegment(currentSegment);


	lastIndex--;
}

bool List::IsFree(Segment* segment)
{
	Segment* i = first;
	int j = 0;
	while (i)
	{
		j++;
		i = i->next;
	}

	if (lastIndex /*+ firstIndex*/ <= (j-1)*elementCount) return true;
	return false;
}

void List::Sort(bool dir, int method)
{

}

int List::Count()
{
	return (lastIndex - firstIndex);
}


bool List::Error()
{
	return error;
}

List::Segment* List::NewSegment()
{
	if (first == nullptr)
	{
		first = last = new Segment();
		first->next = first->prev = nullptr;
		first->data = Heap::Instance().GetMemory(elementCount*elementSize);

		//??
		if (first->data == nullptr)
		{
			error = true;
			return nullptr;
		}
		segmentCount++;
		return first;
	}
	else
	{
		Segment* temp = new Segment();
		last->next = temp;
		temp->prev = last;
		last = temp;
		temp->data = Heap::Instance().GetMemory(elementCount*elementSize);
		//??
		if (first->data == nullptr)
		{
			error = true;
			return nullptr;
		}
		segmentCount++;
		return temp;
	}
	
}

void List::DeleteSegment(Segment* segment)
{
	if (segment == first) first = first->next;
	if (segment == last) last = last->prev;

	if (segment->prev) segment->prev->next = segment->next;
	if (segment->next) segment->next->prev = segment->prev;
	Heap::Instance().FreeMemory(segment->data);
	segmentCount--;
}

void* List::G(int n)
{
	n += firstIndex;
	Segment* currentSegment = GetSegment(n / elementCount);
	int elementIndex = n % elementCount;

	char* source = (char*)currentSegment->data + elementIndex*elementSize;
	return source;
}

void List::S(void* data, int n)
{
	n += firstIndex;
	Segment* currentSegment = GetSegment(n / elementCount);
	int elementIndex = n % elementCount;

	char* source = (char*)currentSegment->data + elementIndex*elementSize;
	CopyElement(source, data);
}

void List::Swap(int a, int b)
{
	//void* temp = G(a);
	void* temp = Heap::Instance().GetMemory(elementSize);
	CopyElement(temp, G(a));
	S(G(b), a);
	S(temp, b);
}