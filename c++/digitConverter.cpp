#include "stdafx.h"
#include <iostream>

//true	дробь
//false	не дробь 
bool IsFraction(char* s)
{
	bool e = false;
	bool x = false;

	for (int i = 0; i < strlen(s); i++)
	{
		if (s[i] == '.') return true;
		if (s[i] == 'e' || s[i] == 'E') e = true;
		if (s[i] == 'x' || s[i] == 'X') x = true;
	}

	if (e && !x) return true;
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char s[1024];
	
	while (true)
	{
		std::cin >> s;
		
		if (IsFraction(s))
		{
			double digit = atof(s);
			std::cout << digit << std::endl;
		}
		else
		{
			int digit = strtol(s, nullptr, 0);
			std::cout << digit << std::endl;
		}

	}


	return 0;
}
