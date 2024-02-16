#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>

#include "FileWalker.h"
#include "Types.h"

namespace memData
{
static bool g_isUTF8 = true;
}

template<typename T>
struct MemData
{
	std::vector<BYTE> data = {};
	T size                 = 0;
	bool readSize          = false;

	std::string toString() const
	{
		std::string str = "";

		if (!data.empty() && data.size() > 1)
			str = std::string(reinterpret_cast<const char*>(data.data()), data.size() - ((data.back() == 0x0) ? 1 : 0));

		if (!memData::g_isUTF8)
			str = sjis2utf8(str);

		return str;
	}

	void fromString(const std::string& str)
	{
		std::string s = str;
		if (!memData::g_isUTF8)
			s = utf82sjis(str);

		size = static_cast<T>(s.size()) + 1;
		data.resize(size);
		std::copy(s.begin(), s.end(), data.begin());

		data.back() = 0x0;
	}

	void write(FileWriter& fw) const
	{
		if (readSize)
			fw.Write(size);

		if (size > 0)
			fw.WriteBytesVec(data);
	}

	friend std::ostream& operator<<(std::ostream& os, MemData const& md)
	{
		os << md.toString();

		return os;
	}
};

template<typename T>
void initMemData(MemData<T>& memData, FileWalker& fw, const T& size = ~0)
{
	if (size == static_cast<T>(~0))
	{
		if (sizeof(T) == 1)
			memData.size = fw.ReadByte();
		else if (sizeof(T) == 2)
			memData.size = fw.ReadWord();
		else if (sizeof(T) == 4)
			memData.size = static_cast<T>(fw.ReadDWord());
		memData.readSize = true;
	}
	else
		memData.size = size;

	if (memData.size)
	{
		memData.data.resize(memData.size);
		fw.ReadBytesVec(memData.data);
	}
}

template<typename T>
MemData<T> initFromData(const std::string& str, const bool& readSize)
{
	MemData<T> memData;
	memData.fromString(str);
	memData.readSize = readSize;

	return memData;
}