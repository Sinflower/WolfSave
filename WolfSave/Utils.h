#pragma once

#include <Windows.h>
#include <string>
#include <stdint.h>
#include <codecvt>

#define NOT_IMPLEMENTED \
{ \
	std::cerr << __FILE__ << ":" << __LINE__ << " ERROR This has not been implemented" << std::endl; \
	exit(-1); \
}

#define CHECK_IF_NEEDED \
{ \
	std::cerr << __FILE__ << ":" << __LINE__ << " WARNING Check if this is needed" << std::endl; \
}

#define ERROR_SWW \
{ \
	std::cerr << __FILE__ << ":" << __LINE__ << " ERROR Something went wrong" << std::endl; \
	exit(-1); \
}

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))

#define BYTE0(x)   BYTEn(x,  0)
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)

static inline std::wstring s2ws(const std::string& str)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

static inline std::string ws2s(const std::wstring& wstr)
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}

std::basic_ostream<TCHAR>& tcout =
#ifdef _UNICODE
	std::wcout;
#else
	std::cout;
#endif // _UNICODE

std::basic_ostream<TCHAR>& tcerr =
#ifdef _UNICODE
	std::wcerr;
#else
	std::cerr;
#endif // _UNICODE
