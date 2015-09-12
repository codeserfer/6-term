#pragma once
#include "Defaults.h"
#include "Block.h"

class Segment
{
public:
	Segment* prev;

	Segment(Segment* prev, int segmentSize = SEGMENTSIZE);
	~Segment(void);
	
	void*	PushBlock(int blockSize);
	void	RemoveBlock(void* oldBlock);

private:
	void	RemoveBlock(Block* oldBlock);
	void	ClearSegment();

	bool	IsEmpty();

	void*	data;

	int		segmentSize;

	Block*	firstBlock;
	Block*	lastBlock;
};

