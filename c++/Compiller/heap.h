#pragma once
#include <iostream>

#define SEGMENTSIZE 65536

class Heap
{
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


		//Возвращает ссылку на блок, или NULL, если блок требуемого размера не найден
		void* PushBlock(int blockSize)
		{
			Block* newBlock = new Block;

			newBlock->used = true;
			newBlock->size = blockSize;

			Block* i = firstBlock;
			while (i)
			{
				if (!i->used)
				{
					if (i->size > blockSize)
					{
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
						//newBlock->offset = i->offset;
						newBlock = i;
						i->used = true;
						return newBlock->offset;
					}
				}
				i = i->next;
			}

			return nullptr;
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
							//Если после всех манипуляций у нас остался один большой свободный блок, то страницу надо удалить
							if (i->firstBlock->used == false && i->firstBlock->next == nullptr)
							{
								i->ClearSegment();
							}
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
			free(this->data);
			this->firstBlock = this->lastBlock = nullptr;
			this->data = this->prev = nullptr;
		}

	private:

		void RemoveBlock(Block* oldBlock)
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

	};

	Segment* current;

	Segment* MakeSegment();
	void DeleteSegments();

	public:
		Heap(int segmentSize = SEGMENTSIZE);
		~Heap(void);

		void* GetMemory(int);
		void FreeMemory(Segment*);
		static const int segmentSize = SEGMENTSIZE;


		int GetCountOfSegments()
		{
			Segment* i = current;
			int count = 0;
			while (i)
			{
				count++;
				i = i->prev;
			}
			return count;
		}
};
