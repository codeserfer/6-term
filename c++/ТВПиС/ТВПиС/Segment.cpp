#include "stdafx.h"
#include "Defaults.h"
#include "Block.h"
#include "Segment.h"
#include <iostream>
#include <exception>

using namespace std;

Segment::Segment(Segment* prev, int segmentSize)
{
	this->prev = prev;
			
	firstBlock = lastBlock = new Block;

	if (!(firstBlock->offset = data = malloc(segmentSize)))
	{
		throw new bad_alloc();
	}

	firstBlock->used = false;
	firstBlock->size = this->segmentSize = segmentSize;

	firstBlock->prev = firstBlock->next = nullptr;
}

Segment::~Segment(void)
{
	ClearSegment();
}

void* Segment::PushBlock(int blockSize)
{
	if (blockSize > segmentSize)
	{
		throw new bad_alloc();
	}

	Block* i = firstBlock;
	while (i)
	{
		if (!i->used)
		{
			if (i->size > blockSize)
			{
				Block* newBlock = new Block;

				newBlock->used = true;
				newBlock->size = blockSize;

				newBlock->offset = i->offset;
				i->offset = (char*)newBlock->offset + newBlock->size;

				i->size -= blockSize;

				if (newBlock->prev = i->prev)
				{
					newBlock->prev->next = newBlock;
				}
				i->prev = newBlock;

				newBlock->next = i;
				if (i == firstBlock) firstBlock = newBlock;
				return newBlock->offset;
			}
			else if (i->size == blockSize)
			{
				i->used = true;
				return i->offset;
			}
		}
		i = i->next;
	}

	return nullptr;	//bad_alloc() ?
}

void Segment::RemoveBlock(void* oldBlock)
{
	Segment* i = this;
	while (i)
	{
		if (i->firstBlock->offset <= oldBlock && (char*)i->firstBlock->offset + SEGMENTSIZE > oldBlock)
		{
			Block* j = i->firstBlock;
			while (j)
			{
				if (j->offset == oldBlock)
				{
					RemoveBlock(j);

					//≈сли в странице не осталось больше зан€тых блоков и это не последн€€ страница, то удал€ем ее
					/*if (i->IsEmpty() && i->prev)
					{
						i->ClearSegment(); //Ќе хорошо.
					}*/

					return;
				}
				j = j->next;
			}
		}

		i = i->prev;
	}
}

void Segment::RemoveBlock(Block* oldBlock)
{
	oldBlock->used = false;

	if (oldBlock->prev && !oldBlock->prev->used)
	{
		if (oldBlock->prev == firstBlock) firstBlock = oldBlock;
		oldBlock->size += oldBlock->prev->size;
		oldBlock->offset = (char*)oldBlock->offset - oldBlock->prev->size;

		if (oldBlock->prev->prev)
		{
			oldBlock->prev->prev->next = oldBlock;
		}
		oldBlock->prev = oldBlock->prev->prev;
	}

	if (oldBlock->next && !oldBlock->next->used)
	{
		oldBlock->size += oldBlock->next->size;

		if (oldBlock->next == lastBlock) lastBlock = oldBlock;

		if (oldBlock->next->next)
		{
			oldBlock->next->next->prev = oldBlock;
		}
		oldBlock->next = oldBlock->next->next;
	}
}

void Segment::ClearSegment()
{
	free(this->data);

	this->firstBlock = this->lastBlock = nullptr;
	this->data = this->prev = nullptr;
	this->segmentSize = 0;
}

bool Segment::IsEmpty()
{
	if (this->firstBlock == this->lastBlock && this->firstBlock->size == false)
		return true;
	return false;
}

