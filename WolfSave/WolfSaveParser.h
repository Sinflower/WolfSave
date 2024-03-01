/*
 *  File: WolfSaveParser.h
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

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

#include "FileWalker.h"
#include "FileWriter.h"
#include "JsonDumper.h"
#include "JsonReader.h"
#include "Types.h"
#include "Utils.h"

#include "SavePart1.h"
#include "SavePart2.h"
#include "SavePart3.h"
#include "SavePart4.h"
#include "SavePart5.h"
#include "VariableDatabase.h"
#include "SavePart7.h"

static const std::string g_version = "0.2.0";

class WolfSaveParser
{
	static constexpr uint32_t SEED_COUNT      = 3;
	static constexpr std::size_t START_OFFSET = 0x14;

	static constexpr WORD HIGHEST_SUPPORTED_VERSION = 0x8E;

public:
	WolfSaveParser() = default;

	bool Parse(const tString &filePath)
	{
		std::vector<BYTE> data;

		if (!read2Buffer(filePath, data))
		{
			tcerr << TEXT("Failed to load input file: ") << filePath << std::endl;
			return false;
		}

		m_fileWalker.InitData(data);

		decryptSave();

		m_fileWalker.Seek(0);

		m_isUTF8   = (m_fileWalker.At(6) == 0x55);

		memData::g_isUTF8 = m_isUTF8;

		m_fileWalker.ReadBytesArr(m_header);

		if (m_fileWalker.GetOffset() + 1 > m_fileWalker.GetSize())
			return false;

		m_var1 = m_fileWalker.ReadByte();

		if (m_var1 != 0x19)
		{
			std::cerr << "Error: m_var1 != 0x19 -- This case is not implemented" << std::endl;
			return false;
		}

		if (!validateName())
			return false;

		return parseSave();
	}

	void Dump(const tString &outputPath)
	{
		tcout << "Dumping Save to JSON: " << outputPath << std::endl;

		JsonDumper dumper(outputPath);

		dump(dumper);

		m_savePart1.Dump(dumper);
		m_savePart2.Dump(dumper);
		m_savePart3.Dump(dumper);
		m_savePart4.Dump(dumper);
		m_savePart5.Dump(dumper);
		m_variableDB.Dump(dumper);
		m_savePart7.Dump(dumper);

		dumper.Write2File();

		tcout << "Dumping finished" << std::endl;
	}

	void Json2Save(const tString &inputPath, const tString &outputPath)
	{
		tcout << "Loading JSON File: " << inputPath << " ... " << std::flush;

		JsonReader reader(inputPath);

		json2Save(reader, m_fileWriter);

		m_savePart1.SetFileVersion(m_fileVersion);
		m_savePart2.SetFileVersion(m_fileVersion);
		m_savePart3.SetFileVersion(m_fileVersion);
		m_savePart4.SetFileVersion(m_fileVersion);
		m_savePart5.SetFileVersion(m_fileVersion);
		m_variableDB.SetFileVersion(m_fileVersion);
		m_savePart7.SetFileVersion(m_fileVersion);

		m_savePart1.Json2Save(reader, m_fileWriter);
		m_savePart2.Json2Save(reader, m_fileWriter);
		m_savePart3.Json2Save(reader, m_fileWriter);
		m_savePart4.Json2Save(reader, m_fileWriter);
		m_savePart5.Json2Save(reader, m_fileWriter);
		m_variableDB.Json2Save(reader, m_fileWriter);
		m_savePart7.Json2Save(reader, m_fileWriter);

		m_fileWriter.Write(m_endByte);

		tcout << "Done" << std::endl;

		tcout << "Fixing Checksum ... " << std::flush;
		fixCheckSum();
		tcout << "Done" << std::endl;

		tcout << "Encrypting Save ... " << std::flush;
		encryptSave();
		tcout << "Done" << std::endl;

		tcout << "Writing Save to file: " << outputPath << " ... " << std::flush;
		m_fileWriter.WriteToFile(outputPath);
		tcout << "Done" << std::endl;
	}

private:
	bool read2Buffer(const tString &pFilePath, std::vector<BYTE> &buffer)
	{
		HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			tcerr << TEXT("ERROR: Unable to open file \"") << pFilePath << TEXT("\".") << std::endl;
			return false;
		}

		DWORD fileSize = GetFileSize(hFile, NULL);

		if (fileSize == INVALID_FILE_SIZE)
		{
			tcerr << TEXT("ERROR: Unable to get file size for \"") << pFilePath << TEXT("\".") << std::endl;
			CloseHandle(hFile);
			return false;
		}

		buffer.resize(fileSize, 0);

		DWORD dwBytesRead = 0;
		BOOL bResult      = ReadFile(hFile, buffer.data(), fileSize, &dwBytesRead, NULL);
		CloseHandle(hFile);

		if (!bResult)
		{
			tcerr << TEXT("ERROR: Unable to read file \"") << pFilePath << TEXT("\".") << std::endl;
			return false;
		}

		return true;
	}

	void decryptSave()
	{
		std::array<uint8_t, SEED_COUNT> seeds;
		uint32_t projSeed = 0;

		DWORD dwBytesRead = 0;

		PBYTE pBytes = m_fileWalker.Get();

		seeds[0] = pBytes[0];
		seeds[1] = pBytes[3];
		seeds[2] = pBytes[9];

		std::cout << "Seeds: " << std::hex << std::flush;
		for (const uint8_t &seed : seeds)
			std::cout << "0x" << static_cast<uint32_t>(seed) << " " << std::flush;

		std::cout << std::dec << std::endl;

		for (std::size_t i = 0; i < seeds.size(); i++)
		{
			srand(seeds[i]);

			std::size_t inc = 1;

			if (i == 1) inc = 2;
			if (i == 2) inc = 5;

			if (START_OFFSET < m_fileWalker.GetSize())
			{
				for (std::size_t j = START_OFFSET; j < m_fileWalker.GetSize(); j += inc)
					pBytes[j] ^= static_cast<uint8_t>(rand() >> 12);
			}
		}
	}

	void encryptSave()
	{
		std::array<uint8_t, SEED_COUNT> seeds;
		uint32_t projSeed = 0;

		DWORD dwBytesRead = 0;

		PBYTE pBytes = m_fileWriter.Get();

		seeds[0] = pBytes[9];
		seeds[1] = pBytes[3];
		seeds[2] = pBytes[0];

		std::cout << "Seeds: " << std::hex << std::flush;
		for (const uint8_t &seed : seeds)
			std::cout << "0x" << static_cast<uint32_t>(seed) << " " << std::flush;

		std::cout << std::dec << std::endl;

		for (std::size_t i = 0; i < seeds.size(); i++)
		{
			srand(seeds[i]);

			std::size_t inc = 5;

			if (i == 1) inc = 2;
			if (i == 2) inc = 1;

			if (START_OFFSET < m_fileWriter.GetSize())
			{
				for (uint64_t j = START_OFFSET; j < m_fileWriter.GetSize(); j += inc)
					pBytes[j] ^= static_cast<uint8_t>(rand() >> 12);
			}
		}
	}

	bool validateName()
	{
		initMemData(m_name, m_fileWalker);

		m_fileVersion = m_fileWalker.ReadWord();

		if (m_fileVersion > HIGHEST_SUPPORTED_VERSION)
		{
			std::cerr << "File Version: 0x" << std::hex << m_fileVersion << " is not supported, highest supported version: 0x" << HIGHEST_SUPPORTED_VERSION << std::dec << std::endl;
			return false;
		}

		m_savePart1.SetFileVersion(m_fileVersion);
		m_savePart2.SetFileVersion(m_fileVersion);
		m_savePart3.SetFileVersion(m_fileVersion);
		m_savePart4.SetFileVersion(m_fileVersion);
		m_savePart5.SetFileVersion(m_fileVersion);
		m_variableDB.SetFileVersion(m_fileVersion);
		m_savePart7.SetFileVersion(m_fileVersion);

		return true;
	}

	bool parseSave()
	{
		std::cout << "Starting save parse at offset: 0x" << std::hex << m_fileWalker.GetOffset() << std::dec << std::endl;

		m_savePart1.Parse(m_fileWalker);
		m_savePart2.Parse(m_fileWalker);
		m_savePart3.Parse(m_fileWalker);
		m_savePart4.Parse(m_fileWalker);
		m_savePart5.Parse(m_fileWalker);
		m_variableDB.Parse(m_fileWalker);
		m_savePart7.Parse(m_fileWalker);

		if (m_fileWalker.GetOffset() + 1 != m_fileWalker.GetSize())
		{
			std::cerr << "Error: m_fileWalker.GetOffset() + 1 != m_fileWalker.GetSize()" << std::endl;
			return false;
		}

		m_endByte = m_fileWalker.ReadByte();
		if (m_endByte != 0x19)
		{
			std::cerr << "Error: Invalid final byte - Expected 0x19, got: 0x" << std::hex << m_endByte << std::dec << std::endl;
			return false;
		}

		return true;
	}

	void dump(JsonDumper &jd) const
	{
		dumpConfig(jd);
		dumpHeader(jd);
	}

	void dumpHeader(JsonDumper &jd) const
	{
		jd.EnterSection("Header");

		jd.Dump(m_header, JsonDumper::DO_NOT_TOUCH);
		jd.Dump(m_var1, JsonDumper::DO_NOT_TOUCH | JsonDumper::MAGIC_NUMBER);

		jd.Dump(m_name);

		jd.Dump(m_fileVersion, JsonDumper::DO_NOT_TOUCH);

		jd.Dump(m_endByte, JsonDumper::DO_NOT_TOUCH | JsonDumper::MAGIC_NUMBER);

		jd.LeaveSection();
	}

	void dumpConfig(JsonDumper &jd) const
	{
		jd.EnterSection("Config");
		jd.Dump(g_version, "Version", JsonDumper::DO_NOT_TOUCH);
		jd.Dump(m_isUTF8, "UTF-8", JsonDumper::DO_NOT_TOUCH);
		jd.LeaveSection();
	}

	void json2Save(JsonReader& jr, FileWriter& fw)
	{
		loadConfig(jr);
		json2SaveHeader(jr, fw);
	}

	void loadConfig(JsonReader &jr)
	{
		jr.EnterSection("Config");

		std::string version = jr.Read<std::string>("Version");
		m_isUTF8            = jr.Read<bool>("UTF-8");
		memData::g_isUTF8 = m_isUTF8;

		if (version != g_version)
		{
			std::cerr << "Error: Invalid version - Expected: " << g_version << ", got: " << version << std::endl;
			exit(-1);
		}

		jr.LeaveSection();
	}

	void json2SaveHeader(JsonReader &jr, FileWriter &fw)
	{
		jr.EnterSection("Header");

		m_header = jr.ReadArr<BYTE, START_OFFSET>();
		m_var1   = jr.Read<BYTE>();

		m_name = jr.ReadMemData<WORD>();

		m_fileVersion = jr.Read<WORD>();

		m_endByte = jr.Read<BYTE>();

		if (m_endByte != 0x19)
		{
			std::cerr << "Error: Invalid final byte - Expected 0x19, got: 0x" << std::hex << m_endByte << std::dec << std::endl;
			exit(-1);
		}

		fw.WriteBytesArr(m_header);
		fw.Write(m_var1);
		m_name.write(fw);
		fw.Write(m_fileVersion);

		jr.LeaveSection();
	}

	void fixCheckSum()
	{
		PBYTE pData = m_fileWriter.Get();

		BYTE checkSum = 0;

		for (std::size_t i = 0x14; i < m_fileWriter.GetSize(); i++)
			checkSum += pData[i];

		m_fileWriter.SetAt(0x2, checkSum);
	}

private:
	FileWalker m_fileWalker = FileWalker();
	FileWriter m_fileWriter = FileWriter();

	std::array<BYTE, START_OFFSET> m_header = { 0 };

	bool m_isUTF8 = true;

	BYTE m_var1     = 0;
	BYTE m_endByte  = 0;
	MemData<WORD> m_name;
	WORD m_fileVersion = 0;

	SavePart1 m_savePart1;
	SavePart2 m_savePart2;
	SavePart3 m_savePart3;
	SavePart4 m_savePart4;
	SavePart5 m_savePart5;
	VariableDatabase m_variableDB;
	SavePart7 m_savePart7;
};