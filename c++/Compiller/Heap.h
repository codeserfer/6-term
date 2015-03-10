#pragma once
#include "Defaults.h"
#include "Segment.h"

class Heap
{
public:
	Heap(int segmentSize = SEGMENTSIZE);
	~Heap(void);

	void* GetMemory(int);

private:

	int segmentSize;
	Segment* current;

	Segment* MakeSegment();
	void DeleteSegments();

	void FreeMemory(Segment*);

};

