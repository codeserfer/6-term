#include "Heap.h"
#include "Defaults.h"



class List
{

public:
	List(int elementSize, int elementCount = LISTSIZE);
	~List();

	void* Get(int);
	void Add(void*);



	// returns and deletes elements
	void TakeFirst(void*);
	void TakeLast(void*);
	void Take(int, void*);
	void Sort(bool, int);

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
	int firstIndex; //Номер первого элемента
	int lastIndex; //Номер последнего элемента



	int elementSize; //Размер одного элемента в байтах
	int elementCount; //Количество элементов на один сегмент
	bool error; //true, если возникла ошибка, допустим, при выделении памяти
	int segmentCount; //Текущее количество сегментов

	void NewSegment();
	void DeleteSegment(Segment*);
	Segment* GetSegment(int);
	void CopyElement(void*, void*);
	
};