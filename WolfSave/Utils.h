#pragma once

#include <Windows.h>
#include <codecvt>
#include <stdint.h>
#include <string>

#define NOT_IMPLEMENTED                                                                                  \
	{                                                                                                    \
		std::cerr << __FILE__ << ":" << __LINE__ << " ERROR This has not been implemented" << std::endl; \
		exit(-1);                                                                                        \
	}

#define CHECK_IF_NEEDED                                                                              \
	{                                                                                                \
		std::cerr << __FILE__ << ":" << __LINE__ << " WARNING Check if this is needed" << std::endl; \
	}

#define ERROR_SWW                                                                               \
	{                                                                                           \
		std::cerr << __FILE__ << ":" << __LINE__ << " ERROR Something went wrong" << std::endl; \
		exit(-1);                                                                               \
	}

#define BYTEn(x, n) (*((BYTE*)&(x) + n))

#define BYTE0(x) BYTEn(x, 0)
#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define BYTE3(x) BYTEn(x, 3)

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
	std::string utf8String = "";

	LPCCH pSJIS   = sjis.c_str();
	const int32_t utf16Size = ::MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, pSJIS, -1, 0, 0);
	if (utf16Size != 0)
	{
		LPWSTR pUTF16 = new WCHAR[utf16Size];
		if (::MultiByteToWideChar(932, 0, pSJIS, -1, pUTF16, utf16Size) != 0)
		{
			const int32_t utf8Size = ::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, 0, 0, 0, 0);
			if (utf8Size != 0)
			{
				LPSTR pUTF8 = new CHAR[utf8Size + 16];
				ZeroMemory(pUTF8, utf8Size + 16);
				if (::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, pUTF8, utf8Size, 0, 0) != 0)
					utf8String = std::string(pUTF8);

				delete[] pUTF8;
			}
		}

		delete[] pUTF16;
	}

	return utf8String;
}

static inline std::string utf82sjis(const std::string& utf8)
{
	std::string sjisString = "";

	LPCCH pUTF8   = utf8.c_str();
	const int32_t utf16Size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pUTF8, -1, 0, 0);
	if (utf16Size != 0)
	{
		LPWSTR pUTF16 = new WCHAR[utf16Size];
		if (::MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, pUTF16, utf16Size) != 0)
		{
			const int32_t sjisSize = ::WideCharToMultiByte(932, 0, pUTF16, -1, 0, 0, 0, 0);
			if (sjisSize != 0)
			{
				LPSTR pSJIS = new CHAR[sjisSize + 16];
				ZeroMemory(pSJIS, sjisSize + 16);
				if (::WideCharToMultiByte(932, 0, pUTF16, -1, pSJIS, sjisSize, 0, 0) != 0)
					sjisString = std::string(pSJIS);

				delete[] pSJIS;
			}
		}

		delete[] pUTF16;
	}

	return sjisString;
}