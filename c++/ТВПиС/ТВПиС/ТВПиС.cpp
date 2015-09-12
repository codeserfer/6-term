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
	"CharError: ������ ��������� ��������.",
	"StringError: ������ ��������� ��������.",
	"FileReaderError: ������ ������ �����.",
	"TypeNameLengthError: ������� ������� ��� ����.",
	"UnknownType: ���������������� ��� ������.",
	"UnknownOperation: ���������������� ��������.",
	"MissingBracket: ��������� ������.",
	"ArgumentOutOfRange: �������� ��������� ��������� ��� ����������� ��������� ��������.",
	"InvalidOperation: ������������ ��������.",
	"NoTree: ���������� ��������� ���������� ������ �� ����������.",
	"TokenNotFound: ����� �� ������.",
	"VariableNameNotFound: ������������� ���������� ��� ���������.",
	"VariableTypeNotFound: ���������������� ��� ������.",
	"RedefinitionVariable: ������������ ��������������� ����������.",
	"ConstInitializationError: ������ ������������� ���������.",
	"VariableInitializationError: ������ ���������� ���������.",
	"ArrayInitializationError: ������ ���������� �������.",
	"InitializationError: ������ ���������� ��� �������������.",
	"InvalidCharacter: ������������ ������.",
	"UninitializedConstant: �������������������� ���������.",
	"VariableNameIsNotSpecified: �� ������� ��� ����������.",
	"InvalidVariableName: ������������ ��� ����������.",
	"ArraySizeIsNotAConstant: ������ ������� ������ ���� ����� ����������.",
	"ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.",
	"IndexIsNotSpecified: �� ������ ������ �������� �������.",
	"ArrayIndexIsNotAnInteger: ������ ������� �� �������� ����� ������.",
	"ConstantReinitialization: ��������� �������� ��������� ����������.",
	"SystemFunctionIsNotSupported: ���������������� ��������� �������.",
	"MissingArguments: ��������� ��������� ��� ������ �������.",
	"UnexpectedEndOfFile: ����������� ����� �����."
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
		cout << eline->GetMessage() << " ������: " << eline->GetLine() << "." << endl;
	}

	return 0;
}