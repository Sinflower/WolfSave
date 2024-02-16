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

// Based on: https://gist.github.com/takamin/2752a45c5cb4d0f9d1ff
static inline std::string sjis2utf8(const std::string& sjis)
{
	std::string utf8_string;

	LPCCH pSJIS   = (LPCCH)sjis.c_str();
	int utf16size = ::MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, pSJIS, -1, 0, 0);
	if (utf16size != 0)
	{
		LPWSTR pUTF16 = new WCHAR[utf16size];
		if (::MultiByteToWideChar(932, 0, (LPCCH)pSJIS, -1, pUTF16, utf16size) != 0)
		{
			int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, 0, 0, 0, 0);
			if (utf8size != 0)
			{
				LPSTR pUTF8 = new CHAR[utf8size + 16];
				ZeroMemory(pUTF8, utf8size + 16);
				if (::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, pUTF8, utf8size, 0, 0) != 0)
				{
					utf8_string = std::string(pUTF8);
				}
				delete[] pUTF8;
			}
		}
		delete[] pUTF16;
	}
	return utf8_string;
}

static inline std::string utf82sjis(const std::string& utf8)
{
	std::string sjis_string;

	LPCCH pUTF8 = (LPCCH)utf8.c_str();
	int utf16size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pUTF8, -1, 0, 0);
	if (utf16size != 0)
	{
		LPWSTR pUTF16 = new WCHAR[utf16size];
		if (::MultiByteToWideChar(CP_UTF8, 0, (LPCCH)pUTF8, -1, pUTF16, utf16size) != 0)
		{
			int sjissize = ::WideCharToMultiByte(932, 0, pUTF16, -1, 0, 0, 0, 0);
			if (sjissize != 0)
			{
				LPSTR pSJIS = new CHAR[sjissize + 16];
				ZeroMemory(pSJIS, sjissize + 16);
				if (::WideCharToMultiByte(932, 0, pUTF16, -1, pSJIS, sjissize, 0, 0) != 0)
				{
					sjis_string = std::string(pSJIS);
				}
				delete[] pSJIS;
			}
		}
		delete[] pUTF16;
	}
	return sjis_string;
}