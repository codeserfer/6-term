#pragma once
#include "Heap.h"

class List
{

public:
	List(int elementSize, int elementCount);
	~List();

	void* Get(int);
	void Add(void*);
	void* operator[](int);
	void* G(int);


	// returns and deletes elements
	void TakeFirst(void*);
	void TakeLast(void*);
	void Take(int, void*);
	void Sort(bool dir = true, int method = 0);

	int Count();
	bool Error(); // true if error in last operation
private:
	struct Segment
	{
		void*    data;
		Segment* prev;
		Segment* next;
	};

	Segment* first;
	Segment* last;
	int firstIndex; //����� ������� ��������
	int lastIndex; //����� ���������� ��������



	int elementSize; //������ ������ �������� � ������
	bool error; //true, ���� �������� ������, ��������, ��� ��������� ������
	int segmentCount; //������� ���������� ���������
	int elementCount; //���������� ��������� �� ���� �������

	Segment* NewSegment();
	void DeleteSegment(Segment*);
	Segment* GetSegment(int);
	void CopyElement(void*, void*);
	bool IsFree(Segment*);

};