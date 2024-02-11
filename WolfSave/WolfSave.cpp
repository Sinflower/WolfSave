#include <iostream>

#include "WolfSaveParser.h"

/////////////////////////////////////////////////////
////////////////////// MAIN /////////////////////////
/////////////////////////////////////////////////////

int main()
{
	WolfSaveParser parser;

	if (parser.Parse(TEXT("SaveData02.sav")))
		std::cout << "Parsing successful" << std::endl;
	else
	{
		std::cout << "Parsing failed" << std::endl;
		return -1;
	}

	parser.Dump(_T("dump.json"));

	parser.Json2Save(_T("dump.json"), _T("SaveData02_new.sav"));

	return 0;
}
