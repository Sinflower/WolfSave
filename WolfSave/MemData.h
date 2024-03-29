/*
 *  File: MemData.h
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

#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>

#include "FileWalker.h"
#include "FileWriter.h"
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
			memData.size = fw.ReadUInt8();
		else if (sizeof(T) == 2)
			memData.size = fw.ReadUInt16();
		else if (sizeof(T) == 4)
			memData.size = static_cast<T>(fw.ReadUInt32());
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
MemData<T> initMemData(FileWalker& fw, const T& size = ~0)
{
	MemData<T> memData;
	initMemData(memData, fw, size);

	return memData;
}

template<typename T>
MemData<T> initFromData(const std::string& str, const bool& readSize = true)
{
	MemData<T> memData;
	memData.fromString(str);
	memData.readSize = readSize;

	return memData;
}

template<typename T>
using MemDataVec = std::vector<MemData<T>>;