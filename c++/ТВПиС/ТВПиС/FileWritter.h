#pragma once
#include <fstream>

using namespace std;

class FileWritter
{
private:
	ofstream* file;

	void CloseFile();

public:
	FileWritter(char*);
	~FileWritter();

	void WriteLine(char*);
};

