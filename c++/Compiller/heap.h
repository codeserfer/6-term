#pragma once

#define SEGMENTSIZE 65539

class Heap
{
public:
	Heap(int);
	~Heap(void);

	void* GetMemory(int);
	void FreeMemory(void*);

private:

	struct Block
	{
		bool	used;
		int		size;
		void*	offset;

		Block* next;
		Block* prev;

	};

	// �������� ������ �������� SEGMENTSIZE
	struct Segment
	{

		void* data;						// ��������� �� ������� ���������� ������ ������� SEGMENTSIZE
		Segment* prev;					// ��������� �� ���������� ��������

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
				if (!i->used && i->size <= blockSize)
				{

				}
			}


			return nullptr;
		}

		void RemoveBlock()
		{

		}

		void ClearSegment()
		{

		}


	};

	static int segmentSize; // ������ ���������� ��������
	Segment* current;

	int MakeSegment();
	void DeleteSegments();
};

//struct Descriptor {};			// ������ �����
		//Descriptor* firstDescriptor;	// ��������� �� ������ ������ �������� � �������� //@
		//Descriptor* lastDescriptor;		// ��������� �� ����� ������ �������� � �������� //@
		//// ������ �����
		//struct Descriptor
		//{
		//	Block* block;
		//	Descriptor* next;
		//public:
		//	Descriptor* Push(Descriptor* firstDescriptor, Descriptor* lastDescriptor, Block* newDescriptor)
		//	{
		//		if (firstDescriptor == NULL)
		//		{
		//			lastDescriptor = firstDescriptor = new Descriptor;
		//			firstDescriptor->block = newDescriptor;
		//			firstDescriptor->next = NULL;
		//			return firstDescriptor;
		//		}
		//		Descriptor* temp = new Descriptor;
		//		temp->next = NULL;
		//		temp->block = newDescriptor;
		//		lastDescriptor->next = temp;
		//		lastDescriptor = temp;
		//		return temp;
		//	}
		//	//������� ������ � ������� �� � ���������, ���� ��� ���� ���������
		//	void Clear(Descriptor* a, Descriptor* firstDescriptor)
		//	{
		//		//����� ���������� ������
		//		Descriptor* prv = firstDescriptor;
		//	
		//		while (prv->next!=a)
		//		{
		//			prv = prv->next;
		//		}
		//		
		//		//���� ���������� ������ ��������
		//		if (prv->block->used == false)
		//		{
		//		
		//			prv->block->size += a->block->size; //����������� ������ ���������� ������
		//			//������� ������� ������
		//			prv->next = a->next;
		//			delete a;
		//			a = prv;
		//		}
		//		//���� ��������� ������ ��������
		//		if (a->next->block->size == false)
		//		{
		//			a->block->size += a->next->block->size; //����������� ������ ������� ������
		//			//������� ��������� ������
		//			a->next = a->next->next;
		//			delete a->next;
		//		}
		//		a->block->used = false;
		//	}
		//};