#pragma once

#include <vector>
#include <Windows.h>
#include <fstream>

#include "FileWalker.h"

template <typename T>
struct MemData
{
	std::vector <BYTE> data;
	DWORD offset;
	T size;
	bool readSize = false;
	friend std::ostream& operator << (std::ostream& os, MemData const& md)
	{
		if (!md.data.empty())
		{
			std::string str(reinterpret_cast<const char*>(md.data.data()), md.data.size() - ((md.data.back() == 0x0) ? 2 : 0));
			os << str;
		}

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
