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
		std::cout << "Parsing failed" << std::endl;

	return 0;
}
