#include "stdafx.h"
#pragma once
#include "FileReader.h"
#include "Exceptions.h"
#include "Defaults.h" 
#include <iostream>

FileReader::FileReader(char* fileName)
{
	file = new ifstream();
	OpenFile(fileName);
}

FileReader::~FileReader()
{
	CloseFile();
}

void FileReader::OpenFile(char* fileName)
{
	if (fileName != nullptr)
		file->open(fileName);

	if (!file->is_open()) throw Exceptions::FileReaderError;
}

char* FileReader::ReadLine()
{
	if (!file || file->eof())
	{
		return nullptr;
	}

	char* temp = new char[LINESIZE];
	file->getline(temp, LINESIZE);

	return temp;
}

char* FileReader::ReadFile()
{
	if (!file || file->eof())
	{
		return nullptr;
	}
	char* file = new char[1];
	file[0] = '\0';

	while (char* line = ReadLine())
	{
		char* temp = new char[strlen(file) + strlen(line) + 2];
		Copy(temp, file, strlen(file));
		Copy(temp + strlen(file), line, strlen(line));
		Copy(temp + strlen(temp), "\n", 1);
		delete[] file;
		delete[] line;
		file = temp;
	}

	return file;
}

char* FileReader::ReadSourceCode()
{
	char* temp = ReadFile();

	//”бираем табул€цию
	//»справл€ем переносы строк
	/*int i, j;
	for (i = 0, j = 0; i < strlen(temp); i++)
	{
		if (temp[i] == '\t') temp[j++] = ' ';
		//if (temp[i] == '\\' && temp[i + 1] == 'n' && i + 1 < strlen (temp))
		//{
		//	temp[j++] = '\n';
		//	i++;
		//}
		//else
		//{
			temp[j++] = temp[i];
		//}
	}
	temp[i] = '\0';*/

	char* temp2 = new char[strlen(temp) + 1];

	int i, j;
	for (i = 0, j = 0; i < strlen(temp); i++)
	{
		if (temp[i] == '\t')
		{
			temp2[j++] = ' ';
		}

		else if (temp[i] == '\\' && i + 1 < strlen(temp) && temp[i + 1] == 'n')
		{
			temp2[j++] = '\n';
			i++;
		}
		else
		{
			temp2[j++] = temp[i];
		}
	}
	temp2[j] = '\0';

	return temp2;
}

void FileReader::CloseFile()
{
	if (file) file->close();
}