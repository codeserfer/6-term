#pragma once

#define SEGMENTSIZE 65539 //Размер в байтах одной страницы памяти
#define SEGMENTCOUNT 1024 //Максимальное количество ячеек в одной странице - бред, который надо убрать!

class Heap
{
public:
	Heap (int);
	~Heap ();
	void* GetMem (int);
	void FreeMem (void*);

private:
	struct SegmentDef {};
	struct Segment {};
	int SegmentSize;
	Segment* Current; //Указывает на последнюю добавленную страницу (текущую)
	int MakeSegment ();
	void DeleteSegments ();




};
