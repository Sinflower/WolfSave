#include <filesystem>
#include <iostream>
#include <windows.h>

#include "WolfSaveParser.h"

namespace fs = std::filesystem;

/////////////////////////////////////////////////////
////////////////////// MAIN /////////////////////////
/////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
#if 1
	if (argc == 1)
	{
		std::cout << "Usage: " << argv[0] << " <.sav|.json>" << std::endl;
		return 0;
	}

	LPWSTR* szArglist;
	int nArgs;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (szArglist == nullptr)
	{
		std::cout << "CommandLineToArgvW failed" << std::endl;
		return -1;
	}

	for (int i = 0; i < nArgs; i++)
	{
		std::wstring arg = szArglist[i];

		fs::path p = fs::path(arg);

		if (p.extension() == ".sav")
		{
			WolfSaveParser parser;

			if (parser.Parse(arg))
				std::cout << "Parsing successful" << std::endl;
			else
			{
				std::cerr << "Parsing failed" << std::endl;
				continue;
			}

			// Change the extension to .json
			p.replace_extension("json");
			// Dump to a json file
			parser.Dump(p.wstring());
		}
		else if (fs::path(arg).extension() == ".json")
		{
			WolfSaveParser parser;

			p.replace_extension("sav_new");
			parser.Json2Save(arg, p.wstring());
		}
		else
		{
			std::cout << "Invalid file extension" << std::endl;
		}
	}

	LocalFree(szArglist);
#else
	WolfSaveParser parser;

	if (parser.Parse(TEXT("SaveData31.sav")))
		std::cout << "Parsing successful" << std::endl;
	else
	{
		std::cout << "Parsing failed" << std::endl;
		return -1;
	}

	// try
	{
		parser.Dump(_T("dump.json"));

		parser.Json2Save(_T("dump.json"), _T("SaveData31_new.sav"));
	}
	// catch (std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	// }

#endif

	return 0;
}
