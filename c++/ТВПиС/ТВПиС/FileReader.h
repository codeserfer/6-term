#pragma once
#include <fstream>

using namespace std;

class FileReader
{
private:
	ifstream* file;
	void OpenFile(char*);
	void CloseFile();

public:
	FileReader(char*);
	~FileReader();
	char* ReadLine();
	char* ReadFile();
	char* ReadSourceCode();
};

