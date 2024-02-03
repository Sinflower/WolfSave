#pragma once

#include <vector>
#include <Windows.h>
#include <fstream>

#include "FileWalker.h"
#include "Types.h"

template <typename T>
struct MemData
{
	std::vector <BYTE> data;
	DWORD offset;
	T size;
	bool readSize = false;

	std::string toString() const
	{
		std::string str = "";

		if (!data.empty() && data.size() > 1)
			str = std::string(reinterpret_cast<const char*>(data.data()), data.size() - ((data.back() == 0x0) ? 1 : 0));

		return str;
	}

	friend std::ostream& operator << (std::ostream& os, MemData const& md)
	{
		os << md.toString();

		return os;
	}
};

template <typename T>
void initMemData(MemData<T>& memData, FileWalker& fw, const T& size = ~0)
{
	memData.offset = fw.GetOffset();

	if (size == static_cast<T>(~0))
	{
		if (sizeof(T) == 1)
			memData.size = fw.ReadByte();
		else if (sizeof(T) == 2)
			memData.size = fw.ReadWord();
		else if (sizeof(T) == 4)
		memData.size     = static_cast<T>(fw.ReadDWord());
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
