#pragma once

#include <tchar.h>
#include <vector>
#include <ostream>
#include <iostream>
#include <cstdint>
#include <sstream>

static inline std::basic_ostream <TCHAR>& tcout =
#ifdef _UNICODE
std::wcout;
#else
std::cout;
#endif // _UNICODE

static inline std::basic_ostream <TCHAR>& tcerr =
#ifdef _UNICODE
std::wcerr;
#else
std::cerr;
#endif // _UNICODE


#ifdef _UNICODE
using tString = std::wstring;
using tOstream = std::wostream;
using tStringStream = std::wstringstream;
#else
using tString = std::string;
using tOstream = std::ostream;
using tStringStream = std::stringstream;
#endif // _UNICODE

using tStrings = std::vector<tString>;