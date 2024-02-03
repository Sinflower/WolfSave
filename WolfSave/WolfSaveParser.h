#pragma once

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

#include "FileWalker.h"
#include "JsonDumper.h"
#include "Types.h"
#include "Utils.h"

#include "SavePart1.h"
#include "SavePart2.h"
#include "SavePart3.h"
#include "SavePart4.h"
#include "SavePart5.h"
#include "SavePart6.h"
#include "SavePart7.h"

class WolfSaveParser
{
	static constexpr uint32_t SEED_COUNT      = 3;
	static constexpr std::size_t START_OFFSET = 0x14;

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

		m_fw.InitData(data);

		decryptSave();

		m_fw.Seek(0x14);

		if (m_fw.GetOffset() + 1 > m_fw.GetSize())
			return false;

		BYTE v28 = m_fw.ReadByte();

		if (v28 != 0x19)
		{
			std::cerr << "Error: v28 != 0x19 -- This case is not implemented" << std::endl;
			return false;
		}

		if (!validateName())
			return false;

		return parseSave();
	}

	void Dump(const tString &outputPath)
	{
		JsonDumper dumper(outputPath);

		m_savePart1.Dump(dumper);
		m_savePart2.Dump(dumper);
		m_savePart3.Dump(dumper);
		m_savePart4.Dump(dumper);
		m_savePart5.Dump(dumper);
		m_savePart6.Dump(dumper);
		m_savePart7.Dump(dumper);

		dumper.Write2File();
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

		PBYTE pBytes = m_fw.Get();

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

			if (START_OFFSET < m_fw.GetSize())
			{
				for (std::size_t j = START_OFFSET; j < m_fw.GetSize(); j += inc)
					pBytes[j] ^= static_cast<uint8_t>(rand() >> 12);
			}
		}
	}

	bool validateName()
	{
		WORD nameLen = m_fw.ReadWord();
		std::vector<BYTE> name(nameLen);
		m_fw.ReadBytesVec(name);

		m_fileVersion = m_fw.ReadWord();

		if (m_fileVersion > 0x8D)
		{
			std::cerr << "File Version: 0x" << std::hex << m_fileVersion << std::dec << " is not supported" << std::endl;
			return false;
		}

		m_savePart1.SetFileVersion(m_fileVersion);
		m_savePart2.SetFileVersion(m_fileVersion);
		m_savePart3.SetFileVersion(m_fileVersion);
		m_savePart4.SetFileVersion(m_fileVersion);
		m_savePart5.SetFileVersion(m_fileVersion);
		m_savePart6.SetFileVersion(m_fileVersion);
		m_savePart7.SetFileVersion(m_fileVersion);

		return true;
	}

	bool parseSave()
	{
		std::cout << "Starting save parse at offset: 0x" << std::hex << m_fw.GetOffset() << std::dec << std::endl;

		m_savePart1.Parse(m_fw);
		std::cout << "Offset After Step 1: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0x35BA" << std::dec << std::endl;
		m_savePart2.Parse(m_fw);
		std::cout << "Offset After Step 2: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0x3906" << std::dec << std::endl;
		m_savePart3.Parse(m_fw);
		std::cout << "Offset After Step 3: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0xC1EEF" << std::dec << std::endl;
		m_savePart4.Parse(m_fw);
		std::cout << "Offset After Step 4: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0xC23CF" << std::dec << std::endl;
		m_savePart5.Parse(m_fw);
		std::cout << "Offset After Step 5: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0xC6EA7" << std::dec << std::endl;
		m_savePart6.Parse(m_fw);
		std::cout << "Offset After Step 6: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0xFC73D" << std::dec << std::endl;
		m_savePart7.Parse(m_fw);
		std::cout << "Offset After Step 7: 0x" << std::hex << m_fw.GetOffset() << " - Expected: 0xFC73E" << std::dec << std::endl;

		if (m_fw.GetOffset() + 1 != m_fw.GetSize())
		{
			std::cerr << "Error: m_fw.GetOffset() + 1 != m_fw.GetSize()" << std::endl;
			return false;
		}

		BYTE v3 = m_fw.ReadByte();
		if (v3 != 0x19)
		{
			std::cerr << "Error: Invalid final byte - Expected 0x19, got: 0x" << std::hex << v3 << std::dec << std::endl;
			return false;
		}

		return true;
	}

private:
	WORD m_fileVersion = 0;
	FileWalker m_fw    = FileWalker();
	SavePart1 m_savePart1;
	SavePart2 m_savePart2;
	SavePart3 m_savePart3;
	SavePart4 m_savePart4;
	SavePart5 m_savePart5;
	SavePart6 m_savePart6;
	SavePart7 m_savePart7;
};