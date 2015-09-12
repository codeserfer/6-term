#pragma once
#include "Heap.h"
#include "Defaults.h"
#include "SortingMethod.h"

class List
{
public:

	List(int elementSize, int elementCount = LISTSIZE);
	~List();

	void* Get(int);		// Извлечение и исключение из списка
	void* Add(void*);	// Push в конец списка

	// Извлечение элемента в void* и исключение из списка
	void TakeFirst(void*);
	void TakeLast(void*);
	void Take(int, void*);

	void Sort(bool dir = true, SortingMethod method = SortingMethod::QuickSort);

	int Count();
	bool Error();		// true if error in last operation

	void SetAccessor(void*, int);
	void* GetAccessor(int);
protected:

	void Swap(int, int);

	

	struct Segment
	{
		void*    data;
		Segment* prev;
		Segment* next;
	};

	Segment* NewSegment();
	Segment* GetSegment(int);
	void DeleteSegment(Segment*);

	void CopyElement(void*, void*);

	bool IsFree(Segment*);

private:

	Segment* first;
	Segment* last;

	int firstIndex;		//Номер первого элемента
	int lastIndex;		//Номер последнего элемента

	int elementSize;	//Размер одного элемента в байтах
	bool error;			//true, если возникла ошибка, например, при выделении памяти
	int segmentCount;	//Текущее количество сегментов
	int elementCount;	//Количество элементов на один сегмент
};