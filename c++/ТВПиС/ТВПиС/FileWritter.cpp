#include "stdafx.h"
#include "FileWritter.h"

void FileWritter::CloseFile()
{
	if (file) file->close();
}

FileWritter::FileWritter(char* fileName)
{
	file = new ofstream();
	file->open(fileName);
}

FileWritter::~FileWritter()
{
	CloseFile();
}

void FileWritter::WriteLine(char* s)
{
	*file << s << endl;
}