#pragma once
#include <iostream>
#include "TypeList.h"
#include "Exceptions.h"

class TValue
{
public:

	/// <summary>
	/// Для выделения памяти под переменную или массив.
	/// </summary>
	/// <param name="value">Значение для переменной, значение для элементов для массива.</param>
	/// <param name="type">Тип переменной или тип элементов массива.</param>
	/// <param name="size">Размер массива, 0 для переменной</param>
	TValue(double value, int type, int size = 0);

	/// <summary>
	/// Создание TValue для переменной в уже выделенной памяти.
	/// </summary>
	/// <param name="pointer">Указатель для выделенной памяти.</param>
	/// <param name="type">Тип переменной, на которую указывает pointer, или тип элементов массива.</param>
	TValue(double* pointer, int type);

	/// <summary>
	/// Создание указателя на nullptr типа void*.
	/// </summary>
	TValue();
	
	~TValue();

	/// <summary>
	/// Получает тип переменной или массива.
	/// </summary>
	int GetType();

	/// <summary>
	/// Получает значение переменной или элемента массива.
	/// </summary>
	/// <param name="index">Индекс элемента массива, для переменной не указывается.</param>
	double GetValue(int index = 0);

	/// <summary>
	/// Устанавливает значение переменной или элемента массива.
	/// </summary>
	/// <param name="v">Новое значение переменной или элемента массива.</param>
	/// <param name="index">Индекс элемента массива, для переменной не указывается.</param>
	void SetValue(double v, int index = 0);

	/// <summary>
	/// Получает адрес переменной или первого элемента массива.
	/// </summary>
	double* GetPointer();

	/// <summary>
	/// Устанавливает адрес переменной или первого элемента массива.
	/// </summary>
	/// <param name="pointer">Адрес переменной или массива.</param>
	void SetPointer(double*);

	/// <summary>
	/// Вовзращает переменную, хранащую элемент массива.
	/// </summary>
	/// <param name="pointer">Индекс элемента массива.</param>
	TValue* GetElementTValue(int index);

	bool IsReference();

	void Print();

private:
	int type;
	double* pointer;
	bool referenceType;
};