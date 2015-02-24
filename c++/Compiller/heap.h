#include "stdafx.h"
#include <iostream>
using namespace std;
#define SEGMENTSIZE 65539
#define SEGMENTCOUNT 1024

class Heap
{
public:
	Heap (int SegmentSize = SEGMENTSIZE)
	{
		this->SegmentSize = SegmentSize;
		this->Current = 0;
	}
	~Heap ()
	{
		DeleteSegments ();
	}
	//Выделение памяти внутри страницы
	void* GetMem (int size)
	{
		if (size>SEGMENTSIZE)
		{
			//Ошибка - запрашиваемый размер больше допустимого
			return nullptr;
		}
		if (Current->descriptor [Current->DescriptorCount].size<size)
		{
			//В текущей странице недостаточно памяти, надо выделить новую страницу

		}
		//Размер выделяемой памяти меньше размера страницы,
		//и в текущей странице (или выше выделенной) хватает свободной памяти для размещения там нового выделяемого фрагмента
		SegmentDef* temp = new SegmentDef;

	}
	void FreeMem (void*);
private:
	//Ячейка памяти
	struct SegmentDef
	{
		bool used;
		int size;
		void* offset;
	};

	//Страница памяти размером SEGMENTSIZE
	struct Segment
	{
		void* data;
		Segment* prev;
		SegmentDef descriptor [SEGMENTCOUNT];
		int DescriptorCount;
	};
	int SegmentSize;
	Segment* Current;


	//Выделение памяти под новую страницу
	int MakeSegment ()
	{
		Segment* temp = new Segment;
		temp->prev = Current;
		temp->data = malloc(sizeof(SegmentSize));
		temp->descriptor[0].size = SegmentSize;
		temp->descriptor[0].offset = 0x0;
		temp->descriptor[0].used = false;
		temp->DescriptorCount = 0;
		Current = temp;
	}
	//Удаление страницы памяти
	void DeleteSegments ()
	{
		Segment* prev;
		while (Current != nullptr)
		{
			prev = Current->prev;
			delete prev;
			Current = prev;
		}
	}
};
int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}
