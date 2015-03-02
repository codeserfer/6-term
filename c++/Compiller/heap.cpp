#include "stdafx.h"
#include <iostream>
#include "Heap.h"


class Heap
{
	Heap (int SegmentSize = SEGMENTSIZE)
	{
		this->SegmentSize = SegmentSize;
		this->Current = 0;
		//Создать первую страницу
	}

	~Heap ()
	{
		DeleteSegments(); //Удаление всех страниц памяти
	}

	//Выделение памяти внутри страницы
	void* GetMem (int size)
	{
		if (size>SEGMENTSIZE)
		{
			//Ошибка - запрашиваемый размер больше допустимого
			return nullptr;
		}

		//Поиск в текущей и во всех страницах участка требуемого размера
		//Если такого не нашлось, то выделяем новую страницу

		


	}

	//Очистка памяти внутри страницы, то есть удаление сегмента
	void FreeMem(void* segment)
	{
		

	}

	//Ячейка памяти
	struct SegmentDef
	{
		bool used;
		int size;
		void* offset;
	};


	//Страница памяти размером SEGMENTSIZE
	struct Segment
	{
		struct descriptor {}; //Список ячеек
		void* data; //Указывает на участок выделенной памяти длинной SEGMENTSIZE
		Segment* prev; //Указывает на предыдущую страницу
		descriptor* FirstDescriptor; //Указывает на начало списка участков в сегменте
		descriptor* LastDescriptor; //Указывает на конец списка участков в сегменте
		int SegmentSize;
		


		
		//Список ячеек
		struct descriptor
		{
		private:
			SegmentDef* Segment;
			descriptor* next;
		public:

			//Добавление новой ячейки в список
			descriptor* Push(descriptor* FirstDescriptor, descriptor* LastDescriptor, SegmentDef* NewDescriptor)
			{
				//Если список пуст
				if (FirstDescriptor == NULL)
				{
					LastDescriptor = FirstDescriptor = new descriptor;
					FirstDescriptor->Segment = NewDescriptor;
					FirstDescriptor->next = NULL;
					return FirstDescriptor;
				}

				descriptor* temp = new descriptor;
				temp->next = NULL;
				temp->Segment = NewDescriptor;
				LastDescriptor->next = temp;
				LastDescriptor = temp;
				return temp;
			}

			//Очистка ячейки и слияние ее с соседними, если они тоже свободные
			void Clear(descriptor* a, descriptor* FirstDescriptor)
			{
				//Поиск предыдущей ячейки
				descriptor* prv = FirstDescriptor;
				while (prv->next!=a)
				{
					prv = prv->next;
				}
				
				//Если предыдущая ячейка свободна
				if (prv->Segment->used == false)
				{
					prv->Segment->size += a->Segment->size; //Увеличиваем размер предыдущей ячейки

					//Удаляем текущую ячейку
					prv->next = a->next;
					delete a;
					a = prv;
				}

				//Если следующая ячейка свободна
				if (a->next->Segment->size == false)
				{
					a->Segment->size += a->next->Segment->size; //Увеличиваем размер текущей ячейки

					//Удаляем следующую ячейку
					a->next = a->next->next;
					delete a->next;
				}
				a->Segment->used = false;
			}


		};




	};






	//Выделение памяти под новую страницу
	int MakeSegment()
	{
		Segment* temp = new Segment;

		temp->prev = Current;

		temp->data = malloc(sizeof(SegmentSize));
		temp->descriptor[0].size = SegmentSize;
		temp->descriptor[0].offset = 0x0;
		temp->descriptor[0].used = false;
		temp->DescriptorCount = 0;
		Current = temp;
	}

	//Удаление всех страниц памяти
	void DeleteSegments()
	{
		Segment* prev;
		while (Current != nullptr)
		{
			prev = Current->prev;
			delete prev;
			Current = prev;
		}
	}
};

