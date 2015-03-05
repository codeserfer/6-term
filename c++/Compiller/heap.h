#pragma once
#include <iostream>

#define SEGMENTSIZE 65539

class Heap
{
public:
	Heap(int segmentSize = SEGMENTSIZE);
	~Heap(void);

	void* GetMemory(int);
	void FreeMemory(void*);
	static const int segmentSize = SEGMENTSIZE;

private:

	struct Block
	{
		bool	used;
		int		size;
		void*	offset;

		Block* next;
		Block* prev;

	};

	 //Страница памяти размером SEGMENTSIZE
	struct Segment
	{

		void* data;						// Указатель на участок выделенной памяти длинной SEGMENTSIZE
		Segment* prev;					// Указатель на предыдущую страницу

		Block* firstBlock;
		Block* lastBlock;

		Segment(Segment* prev)
		{
			this->prev = prev;

			data = malloc(segmentSize);

			firstBlock = lastBlock = new Block;

			firstBlock->used = false;
			firstBlock->size = segmentSize;
			firstBlock->offset = data;

			firstBlock->prev = firstBlock->next = nullptr;
		}


		void* PushBlock(int blockSize)
		{
			Block* newBlock = new Block;

			newBlock->used = true;
			newBlock->size = blockSize;

			Block* i = firstBlock;
			while (i->next)
			{
				if (!i->used)
				{
					if (i->size > blockSize)
					{
						newBlock->offset = i->offset;
						i->offset = (char*)newBlock->offset + newBlock->size;

						if (newBlock->prev = i->prev)
						{
							newBlock->prev->next = newBlock;
						}
						i->prev = newBlock;

						newBlock->next = i;
					}
					else if (i->size == blockSize)
					{
						newBlock->offset = i->offset;
					}
				}
			}

			return newBlock->offset;
		}

		void RemoveBlock(void* oldBlock)
		{
			Segment* i = this;
			while (i)
			{
				if (i->firstBlock->offset <= oldBlock && (char*)i->firstBlock->offset + SEGMENTSIZE < oldBlock) //@
				{
					Block* j = i->firstBlock;
					while (j)
					{
						if (j->offset == oldBlock)
						{
							RemoveBlock(j);
							return;
						}
						j = j->next;
					}
				}

				i = i->prev;
			}
		}

		void ClearSegment()
		{

		}

private:

		void RemoveBlock(Block* oldBlock)
		{
			oldBlock->used = false;

			if (oldBlock->prev && !oldBlock->prev->used)
			{
				oldBlock->size += oldBlock->prev->size;
				oldBlock->offset = (char*)oldBlock->offset - oldBlock->prev->size;

				if (oldBlock->prev->prev)
				{
					oldBlock->prev->prev->next=oldBlock;
				}
				oldBlock->prev = oldBlock->prev->prev;
			}

			if (oldBlock->next && !oldBlock->next->used)
			{
				oldBlock->size += oldBlock->next->size;

				if (oldBlock->next->next)
				{
					oldBlock->next->next->prev=oldBlock;
				}
				oldBlock->next = oldBlock->next->next;
			}
		}

	};

	
	Segment* current;

	int MakeSegment();
	void DeleteSegments();
};
