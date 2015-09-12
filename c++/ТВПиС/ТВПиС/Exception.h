#pragma once

class Exception
{
	char* _message;
	int _line;

public:
	Exception(char* message, int line);
	~Exception();

	char* GetMessage();
	int GetLine();
};