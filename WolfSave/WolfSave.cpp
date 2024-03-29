/*
 *  File: WolfSave.cpp
 *  Copyright (c) 2024 Sinflower
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

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
	int32_t nArgs;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (szArglist == nullptr)
	{
		std::cout << "CommandLineToArgvW failed" << std::endl;
		return -1;
	}

	for (int32_t i = 1; i < nArgs; i++)
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
			std::cerr << "Invalid file extension" << std::endl;
		}
	}

	LocalFree(szArglist);
#else
	WolfSaveParser parser;

	if (parser.Parse(TEXT("SaveData04.sav")))
		std::cout << "Parsing successful" << std::endl;
	else
	{
		std::cout << "Parsing failed" << std::endl;
		return -1;
	}

	// try
	{
		parser.Dump(_T("dump.json"));

		parser.Json2Save(_T("dump.json"), _T("SaveData04_new.sav"));
	}
	// catch (std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	// }

#endif

	return 0;
}
