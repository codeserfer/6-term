#pragma once
#include "Defaults.h"
#include "Segment.h"

class Heap
{
public:

	~Heap(void);

	void* GetMemory(int);
	void FreeMemory(void* offset);

	static Heap& Instance();
private:

	Heap(int segmentSize = SEGMENTSIZE);

	Segment* MakeSegment();
	void DeleteSegments();

	void FreeMemory(Segment*);

	int segmentSize;
	Segment* current;
};