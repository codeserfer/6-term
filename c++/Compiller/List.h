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
	int firstIndex; //����� ������� ��������
	int lastIndex; //����� ���������� ��������



	int elementSize; //������ ������ �������� � ������
	int elementCount; //���������� ��������� �� ���� �������
	bool error; //true, ���� �������� ������, ��������, ��� ��������� ������
	int segmentCount; //������� ���������� ���������

	void NewSegment();
	void DeleteSegment(Segment*);
	Segment* GetSegment(int);
	void CopyElement(void*, void*);
	
};