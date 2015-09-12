#include "stdafx.h"
#include "Defaults.h"
#include <iostream>
#include "ASTBuilder.h"
#include "Exceptions.h"
#include "Exception.h"
#include <ctime>

//#define R "test.cpp"
#define R "bullsAndCows.cpp"
//#define R "towelOfHanoi.cpp"

using namespace std;

static char* messages[] = 
{
	"CharError: ошибка конечного автомата.",
	"StringError: ошибка конечного автомата.",
	"FileReaderError: ошибка чтени€ файла.",
	"TypeNameLengthError: слишком длинное им€ типа.",
	"UnknownType: неподдерживаемый тип данных.",
	"UnknownOperation: неподдерживаема€ операци€.",
	"MissingBracket: пропущена скобка.",
	"ArgumentOutOfRange: значение аргумента находитс€ вне допустимого диапазона значений.",
	"InvalidOperation: недопустима€ операци€.",
	"NoTree: необходимо построить синтексное дерево до выполнени€.",
	"TokenNotFound: токен не найден.",
	"VariableNameNotFound: необъ€вленна€ переменна€ или константа.",
	"VariableTypeNotFound: неподдерживаемый тип данных.",
	"RedefinitionVariable: недопустимое переопределение переменной.",
	"ConstInitializationError: ошибка инициализации константы.",
	"VariableInitializationError: ошибка объ€влени€ константы.",
	"ArrayInitializationError: ошибка объ€влени€ массива.",
	"InitializationError: ошибка объ€влени€ или инициализации.",
	"InvalidCharacter: недопустимый символ.",
	"UninitializedConstant: неинициализированна€ константа.",
	"VariableNameIsNotSpecified: не указано им€ переменной.",
	"InvalidVariableName: недопустимое им€ переменной.",
	"ArraySizeIsNotAConstant: размер массива должен быть задан константой.",
	"ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.",
	"IndexIsNotSpecified: не указан индекс элемента массива.",
	"ArrayIndexIsNotAnInteger: индекс массива не €вл€етс€ целым числом.",
	"ConstantReinitialization: изменение значени€ константы невозможно.",
	"SystemFunctionIsNotSupported: неподдерживаема€ системна€ функци€.",
	"MissingArguments: пропущены аргументы при вызове функции.",
	"UnexpectedEndOfFile: неожиданный конец файла."
};

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(nullptr));
	try
	{
		ASTBuilder* builder = new ASTBuilder(R);
		builder->Build();
		builder->Run();
	}
	catch (Exceptions e)
	{
		setlocale(LC_ALL, "Russian");
		cout << messages[e] << endl;
	}
	catch (Exception* eline)
	{
		setlocale(LC_ALL, "Russian");
		cout << eline->GetMessage() << " —трока: " << eline->GetLine() << "." << endl;
	}

	return 0;
}