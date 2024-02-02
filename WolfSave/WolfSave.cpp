#include <array>
#include <iostream>
#include <vector>
#include <windows.h>

#include "FileWalker.h"
#include "MemData.h"
#include "Utils.h"

bool validateName(FileWalker &fw);
bool parseSave(FileWalker &fw, const BYTE &a2, const BYTE &a3);

void parse_save_1(FileWalker &fw, const BYTE &a2);
void parse_save_1_1(FileWalker &fw);
void parse_save_1_1_1(FileWalker &fw);
void parse_save_1_1_1_1(FileWalker &fw);
void parse_save_2(FileWalker &fw);
void parse_save_3(FileWalker &fw, const BYTE &a2);
void parse_save_4(FileWalker &fw);
void parse_save_5(FileWalker &fw, const BYTE &a2);
void parse_save_5_1(FileWalker &fw, const BYTE &a2);
void parse_save_6(FileWalker &fw);
void parse_save_6_1(FileWalker &fw);
void parse_save_6_1_1(FileWalker &fw, const DWORD &a2, const DWORD &a3);
void parse_save_7(FileWalker &fw);

const uint32_t SEED_COUNT      = 3;
const std::size_t START_OFFSET = 0x14;

bool buf_6_eq_85;
WORD g_fileVersion;

void decryptSave(FileWalker &fw)
{
	std::array<uint8_t, SEED_COUNT> seeds;
	uint32_t projSeed = 0;

	DWORD dwBytesRead = 0;

	PBYTE pBytes = fw.Get();

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

		if (START_OFFSET < fw.GetSize())
		{
			for (std::size_t j = START_OFFSET; j < fw.GetSize(); j += inc)
				pBytes[j] ^= static_cast<uint8_t>(rand() >> 12);
		}
	}
}

bool step0(FileWalker &fw)
{
	decryptSave(fw);

	buf_6_eq_85 = (fw.At(6) == 0x55);

	fw.Seek(0x14);

	if (fw.GetOffset() + 1 > fw.GetSize())
		return false;

	BYTE v28           = fw.ReadByte();
	BYTE dword_1378130 = 1;

	if (v28 != 0x19)
	{
		std::cerr << "Error: v28 != 0x19 -- This case is not implemented" << std::endl;
		return false;
	}

	if (!validateName(fw))
		return false;

	BYTE dword_137A534 = 0;
	BYTE v66           = 0;               // a3;
	BYTE v67           = 0;               // a4;
	return parseSave(fw, v66, (char)v67); // v66 and v67 tend to be 0
}

bool validateName(FileWalker &fw)
{
	WORD nameLen = fw.ReadWord();
	std::vector<BYTE> name(nameLen);
	fw.ReadBytesVec(name);

	if (!buf_6_eq_85)
	{
		NOT_IMPLEMENTED
	}

	g_fileVersion = fw.ReadWord();

	if (g_fileVersion > 141)
	{
		std::cerr << "File Version: 0x" << std::hex << g_fileVersion << std::dec << " is not supported" << std::endl;
		return false;
	}

	return true;
}

bool parseSave(FileWalker &fw, const BYTE &a2, const BYTE &a3)
{
	std::cout << "Starting save parse at offset: 0x" << std::hex << fw.GetOffset() << std::dec << std::endl;

	if (a2)
	{
		NOT_IMPLEMENTED
	}
	else
	{
		parse_save_1(fw, 0);
		std::cout << "Offset After Step 1: 0x" << std::hex << fw.GetOffset() << " - Expected: 0x35BA" << std::dec << std::endl;
		parse_save_2(fw);
		std::cout << "Offset After Step 2: 0x" << std::hex << fw.GetOffset() << " - Expected: 0x3906" << std::dec << std::endl;
		//fw.Seek(0x3906);
		parse_save_3(fw, 0);
		std::cout << "Offset After Step 3: 0x" << std::hex << fw.GetOffset() << " - Expected: 0xC1EEF" << std::dec << std::endl;
		// fw.Seek(0xC1EEF);
		parse_save_4(fw);
		std::cout << "Offset After Step 4: 0x" << std::hex << fw.GetOffset() << " - Expected: 0xC23CF" << std::dec << std::endl;
		parse_save_5(fw, 0);
		std::cout << "Offset After Step 5: 0x" << std::hex << fw.GetOffset() << " - Expected: 0xC6EA7" << std::dec << std::endl;
		parse_save_6(fw);
		std::cout << "Offset After Step 6: 0x" << std::hex << fw.GetOffset() << " - Expected: 0xFC73D" << std::dec << std::endl;
		parse_save_7(fw);
		std::cout << "Offset After Step 7: 0x" << std::hex << fw.GetOffset() << " - Expected: 0xFC73E" << std::dec << std::endl;

		if (fw.GetOffset() + 1 != fw.GetSize())
		{
			std::cerr << "Error: fw.GetOffset() + 1 != fw.GetSize()" << std::endl;
			return false;
		}

		BYTE v3 = fw.ReadByte();
		if (v3 != 0x19)
		{
			std::cerr << "Error: Invalid final byte - Expected 0x19, got: 0x" << std::hex << v3 << std::dec << std::endl;
			return false;
		}

		return true;
	}
}

void parse_save_1(FileWalker &fw, const BYTE &a2)
{
	DWORD this_1707 = fw.ReadDWord();
	DWORD this_1708 = fw.ReadDWord();
	DWORD this_1709 = fw.ReadDWord();

	if (g_fileVersion >= 0x69)
	{
		DWORD this_1695 = fw.ReadDWord();
		DWORD this_1696 = fw.ReadDWord();

		// This read is only for the next check
		DWORD v15 = fw.ReadDWord();

		if (v15 != -1)
		{
			// Revert the previous read
			fw.Seek(fw.GetOffset() - 4);

			std::vector<DWORD> data;

			for (DWORD i = 0; i < 3; i++)
			{
				for (DWORD j = 0; j < this_1695; j++)
				{
					for (DWORD k = 0; k < this_1696; k++)
						data.push_back(fw.ReadDWord());
				}
			}
		}
	}

	MemData<DWORD> data0;
	initMemData(data0, fw);

	if (!buf_6_eq_85)
	{
		NOT_IMPLEMENTED
	}

	DWORD v140 = 7;
	if (g_fileVersion >= 0x73)
		v140 = 15;

	if (g_fileVersion >= 0x8A)
		v140 = 31;

	std::vector<MemData<DWORD>> data1;
	DWORD v139 = 0;

	do
	{
		MemData<DWORD> data;
		initMemData(data, fw);
		data1.push_back(data);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		v139++;
	} while (v139 < v140);

	DWORD v50 = fw.ReadDWord();

	if ((int)v50 > 0)
	{
		std::vector<DWORD> dataVecNew;
		std::vector<BYTE> dataVecOld;

		if (g_fileVersion >= 0x64)
		{
			for (DWORD i = 0; i < v50; i++)
				dataVecNew.push_back(fw.ReadDWord());
		}
		else
		{
			for (DWORD i = 0; i < v50; i++)
				dataVecOld.push_back(fw.ReadByte());
		}
	}

	if (g_fileVersion < 0x8A)
	{
		CHECK_IF_NEEDED
	}

	DWORD v75 = fw.ReadDWord();

	if ((int)v75 > 0)
	{
		for (DWORD i = 0; i < v75; i++)
			parse_save_1_1(fw);
	}

	if (g_fileVersion >= 0x72)
	{
		DWORD this_1951 = fw.ReadDWord();
		DWORD this_1952 = fw.ReadDWord();
		DWORD this_1953 = fw.ReadDWord();
		DWORD this_1954 = fw.ReadDWord();
		DWORD this_1955 = fw.ReadDWord();
		DWORD this_1956 = fw.ReadDWord();
		DWORD this_1957 = fw.ReadDWord();
		DWORD this_1958 = fw.ReadDWord();
		DWORD this_1959 = fw.ReadDWord();
		DWORD this_1960 = fw.ReadDWord();
		DWORD this_1961 = fw.ReadDWord();
	}

	DWORD v105 = fw.ReadDWord();

	DWORD v106 = 0;

	if ((int)v105 > 0)
	{
		std::vector<DWORD> data;
		for (DWORD i = 0; i < v105; i++)
			data.push_back(fw.ReadDWord());
	}
}

void parse_save_1_1(FileWalker &fw)
{
	DWORD this_32  = fw.ReadDWord();
	DWORD this_36  = fw.ReadDWord();
	DWORD this_40  = fw.ReadDWord();
	DWORD this_184 = fw.ReadDWord();
	BYTE this_178  = fw.ReadByte();

	DWORD this_60 = fw.ReadDWord();

	MemData<WORD> memData;
	initMemData(memData, fw);

	if (!buf_6_eq_85)
	{
		NOT_IMPLEMENTED
	}

	WORD this_64 = fw.ReadWord();
	WORD this_66 = fw.ReadWord();
	WORD this_68 = fw.ReadWord();
	WORD this_70 = fw.ReadWord();
	BYTE this_78 = fw.ReadByte();
	BYTE this_79 = fw.ReadByte();

	parse_save_1_1_1(fw);
	parse_save_1_1_1(fw);

	WORD this_84  = fw.ReadWord();
	WORD this_86  = fw.ReadWord();
	WORD this_90  = fw.ReadWord();
	WORD this_88  = fw.ReadWord();
	WORD this_156 = fw.ReadWord();
	WORD this_158 = fw.ReadWord();
	BYTE this_162 = fw.ReadByte();
	BYTE this_163 = fw.ReadByte();
	BYTE this_164 = fw.ReadByte();

	DWORD v56 = fw.ReadDWord();

	if ((int)v56 > 0)
	{
		std::vector<DWORD> data;
		for (DWORD i = 0; i < v56; i++)
			data.push_back(fw.ReadDWord());
	}

	if (g_fileVersion >= 0x70)
	{
		BYTE this_160 = fw.ReadByte();
		BYTE this_161 = fw.ReadByte();
	}

	if (g_fileVersion >= 0x73)
	{
		DWORD this_248 = fw.ReadDWord();
		DWORD this_252 = fw.ReadDWord();
		DWORD this_256 = fw.ReadDWord();
		DWORD this_260 = fw.ReadDWord();
	}

	if (g_fileVersion >= 0x78)
		DWORD this_168 = fw.ReadDWord();

	if (g_fileVersion >= 0x85)
	{
		DWORD this_296 = fw.ReadDWord();
		DWORD this_308 = fw.ReadDWord();
		DWORD this_304 = fw.ReadDWord();
	}

	if (g_fileVersion >= 0x8A)
	{
		WORD v107 = fw.ReadWord();

		if ((int16_t)v107 > 0)
		{
			std::vector<MemData<WORD>> memData;
			for (WORD i = 0; i < v107; i++)
			{
				MemData<WORD> dat;
				initMemData(dat, fw);
			}
		}

		DWORD this_324 = fw.ReadDWord();
		BYTE this_336  = fw.ReadByte();
		DWORD this_344 = fw.ReadDWord();
		DWORD this_348 = fw.ReadDWord();
	}

	if (g_fileVersion >= 0x8B)
		DWORD this_328 = fw.ReadDWord();

	if (g_fileVersion < 0x8C)
		return;

	DWORD this_72 = fw.ReadDWord();
}

void parse_save_1_1_1(FileWalker &fw)
{
	BYTE this_1 = fw.ReadByte();
	BYTE this_2 = fw.ReadByte();
	BYTE this_3 = fw.ReadByte();
	BYTE this_0 = fw.ReadByte();
	BYTE this_4 = fw.ReadByte();
	BYTE this_5 = fw.ReadByte();

	DWORD v11 = fw.ReadDWord();

	if (v11 > 0x10000)
		return;

	for (DWORD i = 0; i < v11; i++)
		parse_save_1_1_1_1(fw);

	return;
}

void parse_save_1_1_1_1(FileWalker &fw)
{
	BYTE this_0 = fw.ReadByte();

	BYTE v26 = fw.ReadByte();

	if (v26)
	{
		std::vector<DWORD> data;

		for (BYTE i = 0; i < v26; i++)
			data.push_back(fw.ReadDWord());
	}

	BYTE v35 = fw.ReadByte();

	if (v35)
	{
		std::vector<BYTE> data;

		for (BYTE i = 0; i < v35; i++)
			data.push_back(fw.ReadByte());
	}
}

void parse_save_2(FileWalker &fw)
{
	BYTE this_2212  = fw.ReadByte();
	BYTE this_2213  = fw.ReadByte();
	DWORD this_2216 = fw.ReadDWord();
	DWORD this_2220 = fw.ReadDWord();
	DWORD this_2228 = fw.ReadDWord();
	DWORD this_2232 = fw.ReadDWord();
	DWORD skip1 = fw.ReadDWord();
	DWORD skip2 = fw.ReadDWord();
	DWORD this_2244 = fw.ReadDWord();
	DWORD this_2248 = fw.ReadDWord();
	DWORD this_2252 = fw.ReadDWord();
	DWORD this_2256 = fw.ReadDWord();
	DWORD this_2260 = fw.ReadDWord();
	DWORD this_2264 = fw.ReadDWord();
	DWORD this_2268 = fw.ReadDWord();
	DWORD this_2272 = fw.ReadDWord();
	DWORD this_2276 = fw.ReadDWord();
	DWORD this_2280 = fw.ReadDWord();
	DWORD this_2284 = fw.ReadDWord();
	DWORD this_2288 = fw.ReadDWord();
	DWORD this_2292 = fw.ReadDWord();
	WORD this_2296  = fw.ReadWord();
	WORD this_2298  = fw.ReadWord();
	WORD this_2300  = fw.ReadWord();
	DWORD this_2302 = fw.ReadDWord();
	WORD this_2306  = fw.ReadWord();
	WORD this_2308  = fw.ReadWord();
	WORD this_2310  = fw.ReadWord();
	DWORD this_2320 = fw.ReadDWord();
	DWORD this_2324 = fw.ReadDWord();
	DWORD this_2328 = fw.ReadDWord();
	DWORD this_2332 = fw.ReadDWord();
	DWORD this_2336 = fw.ReadDWord();
	DWORD this_2340 = fw.ReadDWord();
	DWORD this_2344 = fw.ReadDWord();
	DWORD this_2348 = fw.ReadDWord();
	DWORD this_2352 = fw.ReadDWord();
	BYTE this_2372  = fw.ReadByte();
	WORD this_2356  = fw.ReadWord();
	DWORD this_2360 = fw.ReadDWord();
	DWORD this_2364 = fw.ReadDWord();
	WORD this_2374  = fw.ReadWord();
	WORD this_2376  = fw.ReadWord();
	WORD this_2388  = fw.ReadWord();
	WORD this_2390  = fw.ReadWord();
	DWORD this_2392 = fw.ReadDWord();
	DWORD this_2396 = fw.ReadDWord();
	DWORD this_2400 = fw.ReadDWord();
	BYTE this_2404  = fw.ReadByte();

	if (g_fileVersion <= 96)
		DWORD skip = fw.ReadDWord();

	DWORD this_2408 = fw.ReadDWord();

	if (g_fileVersion >= 98)
	{
		DWORD this_2412 = fw.ReadDWord();
		DWORD this_2416 = fw.ReadDWord();
	}

	if (g_fileVersion >= 100)
	{
		DWORD this_2420 = fw.ReadDWord();
		DWORD this_2424 = fw.ReadDWord();
		BYTE this_2428 = fw.ReadByte();

		DWORD v348 = fw.ReadDWord();

		if ((int)v348 > 0)
		{
			for (DWORD i = 0; i < v348; i++)
			{
				MemData<DWORD> memData;
				initMemData(memData, fw);

				if (!buf_6_eq_85)
				{
					NOT_IMPLEMENTED
				}
			}
		}

		DWORD v73 = fw.ReadDWord();

		if ((int)v73 > 0)
		{
			std::vector<DWORD> data;
			for (DWORD i = 0; i < v73; i++)
				data.push_back(fw.ReadDWord());
		}

		DWORD this_2456 = fw.ReadDWord();
		DWORD this_2460 = fw.ReadDWord();
		DWORD this_2464 = fw.ReadDWord();
		DWORD this_2468 = fw.ReadDWord();
		DWORD this_2472 = fw.ReadDWord();
	}
	if (g_fileVersion >= 101)
	{
		DWORD this_2476 = fw.ReadDWord();
		DWORD this_2488 = fw.ReadDWord();
		DWORD this_2480 = fw.ReadDWord();
		DWORD this_2492 = fw.ReadDWord();
		DWORD this_2484 = fw.ReadDWord();
		DWORD this_2496 = fw.ReadDWord();
	}
	if (g_fileVersion >= 102)
		WORD this_2380 = fw.ReadWord();

	if (g_fileVersion >= 103)
	{
		MemData<WORD> memData1;
		initMemData(memData1, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		MemData<WORD> memData2;
		initMemData(memData2, fw);

		if (!buf_6_eq_85) // this is 1 -- figure out where it is set
		{
			NOT_IMPLEMENTED
		}
	}

	if (g_fileVersion >= 104)
		DWORD this_2500 = fw.ReadDWord();

	if (g_fileVersion >= 106)
	{
		DWORD this_2552 = fw.ReadDWord();
		DWORD this_2556 = fw.ReadDWord();
		DWORD this_2560 = fw.ReadDWord();
	}

	if (g_fileVersion >= 108)
	{
		MemData<WORD> memData1;
		initMemData(memData1, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		DWORD this_2612 = fw.ReadDWord();
		DWORD this_2616 = fw.ReadDWord();
		DWORD this_2620 = fw.ReadDWord();

		MemData<WORD> memData2;
		initMemData(memData2, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		DWORD this_2624 = fw.ReadDWord();
		DWORD this_2628 = fw.ReadDWord();
		DWORD this_2632 = fw.ReadDWord();
	}

	if (g_fileVersion >= 109)
	{
		MemData<WORD> memData1;
		initMemData(memData1, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		MemData<WORD> memData2;
		initMemData(memData2, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		BYTE this_2756 = fw.ReadByte();
	}

	if (g_fileVersion >= 110)
		BYTE this_2757 = fw.ReadByte();

	if (g_fileVersion >= 119)
	{
		MemData<WORD> memData1;
		initMemData(memData1, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		MemData<WORD> memData2;
		initMemData(memData2, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		MemData<WORD> memData3;
		initMemData(memData3, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}
	}

	if (g_fileVersion >= 121)
		DWORD this_2760 = fw.ReadDWord();

	if (g_fileVersion >= 122)
		DWORD this_2764 = fw.ReadDWord();

	if (g_fileVersion >= 124)
		DWORD this_2768 = fw.ReadDWord();

	if (g_fileVersion >= 126)
		DWORD this_2772 = fw.ReadDWord();

	if (g_fileVersion >= 128)
		DWORD this_2776 = fw.ReadDWord();

	if (g_fileVersion >= 129)
	{
		DWORD this_2828 = fw.ReadDWord();
		DWORD this_2836 = fw.ReadDWord();
	}

	if (g_fileVersion >= 130)
		DWORD this_2832 = fw.ReadDWord();

	if (g_fileVersion >= 131)
		DWORD this_2840 = fw.ReadDWord();

	if (g_fileVersion >= 132)
	{
		DWORD this_2844 = fw.ReadDWord();
		DWORD this_2848 = fw.ReadDWord();
		DWORD this_2852 = fw.ReadDWord();
		DWORD this_2856 = fw.ReadDWord();
	}

	if (g_fileVersion >= 134)
		BYTE this_2860 = fw.ReadByte();

	if (g_fileVersion >= 136)
		BYTE this_2861 = fw.ReadByte();

	if (g_fileVersion >= 137)
	{
		DWORD this_2888 = fw.ReadDWord();
		DWORD this_2896 = fw.ReadDWord();
		DWORD this_2900 = fw.ReadDWord();
		DWORD this_2904 = fw.ReadDWord();

		std::vector<DWORD> data;

		for (DWORD i = 0; i < 2; i++)
		{
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
			data.push_back(fw.ReadDWord());
		}
	}

	if (g_fileVersion >= 138)
	{
		DWORD this_3052 = fw.ReadDWord();
		DWORD this_3056 = fw.ReadDWord();
		DWORD this_3060 = fw.ReadDWord();
		DWORD this_3136 = fw.ReadDWord();

		MemData<DWORD> memData;
		initMemData(memData, fw);

		if (!buf_6_eq_85)
		{
			NOT_IMPLEMENTED
		}

		DWORD v293 = fw.ReadDWord();

		if ((int)v293 > 0)
		{
			for (DWORD i = 0; i < v293; i++)
			{
				MemData<DWORD> memData;
				initMemData(memData, fw);

				if (!buf_6_eq_85)
				{
					NOT_IMPLEMENTED
				}
			}
		}
	}

	if (g_fileVersion < 141) return;

	BYTE byte0  = fw.ReadByte();
	BYTE byte1  = fw.ReadByte();
	BYTE byte2  = fw.ReadByte();
	BYTE byte3  = fw.ReadByte();
	BYTE byte4  = fw.ReadByte();
	BYTE byte5  = fw.ReadByte();
	BYTE byte6  = fw.ReadByte();
	BYTE byte7  = fw.ReadByte();
	BYTE byte8  = fw.ReadByte();
	BYTE byte9  = fw.ReadByte();
	BYTE byte10 = fw.ReadByte();
	BYTE byte11 = fw.ReadByte();
	BYTE byte12 = fw.ReadByte();
	BYTE byte13 = fw.ReadByte();

	std::array<BYTE, 0x100> bytes;
	fw.ReadBytesArr(bytes);

	BYTE byte14 = fw.ReadByte();
	BYTE byte15 = fw.ReadByte();
}

void parse_save_3(FileWalker &fw, const BYTE &a2)
{
	DWORD v8 = fw.ReadDWord();
	DWORD v9 = fw.ReadDWord();

	if ((int)v9 >= 0)
	{
		if (v9 > 0)
		{
			for (DWORD i = 0; i < v9; i++)
			{
				DWORD v14 = fw.ReadDWord();
				if (v14 < 0) return;

				for (DWORD j = 0; j < v14; j++)
				{
					BYTE v160 = fw.ReadByte();
					if (v160 > 0)
					{
						std::vector<DWORD> data;
						for (DWORD k = 0; k < v160; k++)
							data.push_back(fw.ReadDWord());
					}
				}
			}
		}

		DWORD v33 = fw.ReadDWord();

		if (v33 <= 0x270F)
		{
			if ((int)v33 > 0)
			{
				for (DWORD i = 0; i < v33; i++)
				{
					DWORD v40 = fw.ReadDWord();
					if ((int)v40 < 0) return;

					if ((int)v40 > 0)
					{
						std::vector<DWORD> data;
						for (DWORD j = 0; j < v40; j++)
							data.push_back(fw.ReadDWord());
					}
				}
			}

			DWORD v54 = fw.ReadDWord();

			if ((int)v54 >= 0)
			{
				if ((int)v54 > 0)
				{
					for (DWORD i = 0; i < v54; i++)
					{
						if (g_fileVersion < 111)
						{
							// This is a WORD read not a DWORD
							MemData<WORD> memData;
							initMemData(memData, fw);

							if (!buf_6_eq_85)
							{
								NOT_IMPLEMENTED
							}
						}
						else
						{
							MemData<DWORD> memData;
							initMemData(memData, fw);

							if (!buf_6_eq_85)
							{
								NOT_IMPLEMENTED
							}
						}
					}
				}

				DWORD v93 = fw.ReadDWord();

				if ((int)v93 < 0 || v93 > 10000)
					return;

				for (DWORD i = 0; i < v93; i++)
				{
					BYTE v100 = fw.ReadByte();

					std::vector<DWORD> data;

					for (BYTE j = 0; j < v100; j++)
						data.push_back(fw.ReadDWord());
				}

				DWORD v108 = fw.ReadDWord();

				if (v108 <= 10000)
				{
					if ((int)v108 <= 0) return;

					for (DWORD i = 0; i < v108; i++)
					{
						BYTE v160 = fw.ReadByte();

						if (v160)
						{
							if (g_fileVersion < 111)
							{
								for (DWORD j = 0; j < v160; j++)
								{
									MemData<WORD> memData;
									initMemData(memData, fw);

									if (!buf_6_eq_85)
									{
										NOT_IMPLEMENTED
									}
								}
							}
							else
							{
								for (DWORD j = 0; j < v160; j++)
								{
									MemData<DWORD> memData;
									initMemData(memData, fw);

									if (!buf_6_eq_85)
									{
										NOT_IMPLEMENTED
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return;
}

void parse_save_4(FileWalker &fw)
{
	parse_save_1_1(fw);

	DWORD v3 = fw.ReadDWord();

	for (DWORD i = 0; i < v3; i++)
		parse_save_1_1(fw);

	BYTE this_433 = fw.ReadByte();
	BYTE this_180 = fw.ReadByte();

	DWORD v11 = fw.ReadDWord();

	if ((int)v11 > 0)
	{
		std::vector<DWORD> data;

		for (DWORD i = 0; i < v11; i++)
			data.push_back(fw.ReadDWord());
	}

	if (g_fileVersion < 138)
		return;

	DWORD v20 = fw.ReadDWord();

	if ((int)v20 > 0)
	{
		std::vector<QWORD> data2;

		for (DWORD i = 0; i < v20; i++)
			data2.push_back(fw.ReadQWord());
	}
}

void parse_save_5(FileWalker &fw, const BYTE &a2)
{
	WORD v5 = fw.ReadWord();

	if ((v5 & 0x8000u) == 0)
	{
		for (WORD i = 0; i < v5; i++)
			parse_save_5_1(fw, 0);
	}
}

void parse_save_5_1(FileWalker &fw, const BYTE &a2)
{
	DWORD this_167 = fw.ReadDWord();

	BYTE this_680 = fw.ReadByte();
	BYTE this_681 = fw.ReadByte();
	WORD this_332 = fw.ReadWord();
	BYTE this_660 = fw.ReadByte();
	BYTE this_661 = fw.ReadByte();

	MemData<WORD> memData;
	initMemData(memData, fw);

	if (!buf_6_eq_85)
	{
		NOT_IMPLEMENTED
	}

	DWORD this_44 = fw.ReadDWord();
	DWORD this_46 = fw.ReadDWord();

	DWORD this_168 = fw.ReadDWord();
	DWORD this_169 = fw.ReadDWord();

	DWORD this_96  = fw.ReadDWord();
	DWORD this_98  = fw.ReadDWord();
	DWORD this_100 = fw.ReadDWord();
	DWORD this_102 = fw.ReadDWord();
	DWORD this_103 = fw.ReadDWord();
	DWORD this_55  = fw.ReadDWord();
	DWORD this_57  = fw.ReadDWord();
	DWORD this_104 = fw.ReadDWord();

	for (uint32_t i = 0; i < 2; i++)
	{
		std::vector<DWORD> data;
		for (uint32_t j = 0; j < 3; j++)
			data.push_back(fw.ReadDWord());
	}

	if (g_fileVersion >= 0x69)
	{
		DWORD this_203 = fw.ReadDWord();
		DWORD this_204 = fw.ReadDWord();
		DWORD this_206 = fw.ReadDWord();
		DWORD this_205 = fw.ReadDWord();
		DWORD this_207 = fw.ReadDWord();
		DWORD this_209 = fw.ReadDWord();
		DWORD this_208 = fw.ReadDWord();
		DWORD this_210 = fw.ReadDWord();
		DWORD this_212 = fw.ReadDWord();
		DWORD this_211 = fw.ReadDWord();
		DWORD this_220 = fw.ReadDWord();
		DWORD this_221 = fw.ReadDWord();
		DWORD this_222 = fw.ReadDWord();
		DWORD this_223 = fw.ReadDWord();
		DWORD this_227 = fw.ReadDWord();
		DWORD this_226 = fw.ReadDWord();
		DWORD this_224 = fw.ReadDWord();
		DWORD this_225 = fw.ReadDWord();
	}
	if (g_fileVersion >= 0x6B)
		BYTE this_662 = fw.ReadByte();

	if (g_fileVersion >= 0x72)
	{
		BYTE this_663 = fw.ReadByte();
		BYTE this_632 = fw.ReadByte();

		std::vector<DWORD> data1;
		std::vector<DWORD> data2;

		for (uint32_t i = 0; i < 4; i++)
			data1.push_back(fw.ReadDWord());

		for (uint32_t i = 0; i < 4; i++)
			data2.push_back(fw.ReadDWord());
	}

	if (g_fileVersion >= 0x73)
	{
		DWORD this_228 = fw.ReadDWord();
		DWORD this_229 = fw.ReadDWord();
		DWORD this_230 = fw.ReadDWord();
		DWORD this_231 = fw.ReadDWord();

		if (g_fileVersion >= 0x74)
		{
			DWORD this_232 = fw.ReadDWord();
			DWORD this_233 = fw.ReadDWord();
			DWORD this_234 = fw.ReadDWord();
			DWORD this_235 = fw.ReadDWord();
			DWORD this_236 = fw.ReadDWord();
		}

		if (g_fileVersion >= 0x75)
		{
			DWORD this_237 = fw.ReadDWord();
			DWORD this_238 = fw.ReadDWord();
			DWORD this_239 = fw.ReadDWord();
			DWORD this_240 = fw.ReadDWord();
			DWORD this_241 = fw.ReadDWord();
			DWORD this_242 = fw.ReadDWord();
		}

		DWORD this_264 = fw.ReadDWord();
		DWORD this_265 = fw.ReadDWord();
		DWORD this_266 = fw.ReadDWord();
		DWORD this_268 = fw.ReadDWord();
		DWORD this_269 = fw.ReadDWord();
		DWORD this_270 = fw.ReadDWord();
	}

	if (g_fileVersion >= 0x76)
	{
		DWORD this_271 = fw.ReadDWord();
		DWORD this_272 = fw.ReadDWord();
		DWORD this_273 = fw.ReadDWord();
	}

	if (g_fileVersion < 0x81)
		return;

	DWORD this_243 = fw.ReadDWord();
	DWORD this_244 = fw.ReadDWord();
	DWORD this_245 = fw.ReadDWord();
	DWORD this_246 = fw.ReadDWord();
	DWORD this_247 = fw.ReadDWord();
	DWORD this_248 = fw.ReadDWord();

	DWORD this_249 = fw.ReadDWord();
	DWORD this_250 = fw.ReadDWord();
	DWORD this_251 = fw.ReadDWord();
	DWORD this_252 = fw.ReadDWord();
	DWORD this_253 = fw.ReadDWord();
	DWORD this_254 = fw.ReadDWord();
	DWORD this_255 = fw.ReadDWord();
	DWORD this_256 = fw.ReadDWord();
	DWORD this_257 = fw.ReadDWord();
	DWORD this_258 = fw.ReadDWord();
	DWORD this_274 = fw.ReadDWord();
	DWORD this_259 = fw.ReadDWord();
	DWORD this_260 = fw.ReadDWord();
	DWORD this_261 = fw.ReadDWord();
	DWORD this_262 = fw.ReadDWord();

	if (g_fileVersion >= 0x87)
	{
		DWORD this_275 = fw.ReadDWord();
		DWORD this_276 = fw.ReadDWord();
		DWORD this_277 = fw.ReadDWord();
		DWORD this_278 = fw.ReadDWord();
		DWORD this_279 = fw.ReadDWord();
	}

	if (g_fileVersion >= 0x89)
	{
		DWORD v198 = fw.ReadDWord();

		if ((int)v198 > 0)
		{
			for (DWORD i = 0; i < v198; i++)
			{
				DWORD v202 = fw.ReadDWord();

				if ((int)v202 > 0)
				{
					std::vector<DWORD> data;

					for (DWORD j = 0; j < v202; j++)
						data.push_back(fw.ReadDWord());
				}
			}
		}

		DWORD this_293 = fw.ReadDWord();
		DWORD this_294 = fw.ReadDWord();
		DWORD this_296 = fw.ReadDWord();
		DWORD this_297 = fw.ReadDWord();
		DWORD this_298 = fw.ReadDWord();
		DWORD this_299 = fw.ReadDWord();
		DWORD this_230 = fw.ReadDWord();
	}
}

BYTE byte_1DA80F5 = 0;

void parse_save_6(FileWalker &fw)
{
	// Is here 1 byte just skipped?
	fw.ReadByte();
	byte_1DA80F5 = 1;

	DWORD v2 = fw.ReadDWord();
	for (DWORD i = 0; i < v2; i++)
		parse_save_6_1(fw);

	byte_1DA80F5 = 0;

	// TODO: Here soemthing is done with CDataBase -- Maybe mapping of custom variables, look into it later
	// sub_69CDF0("BasicData/CDataBase");
	// sub_69CDF0(".project");
}

void parse_save_6_1(FileWalker &fw)
{
	DWORD v6;

	if (byte_1DA80F5)
		v6 = fw.ReadDWord();
	else
	{
		NOT_IMPLEMENTED
	}

	if ((int)v6 <= -1)
	{
		if ((int)v6 <= -2)
		{
			if (byte_1DA80F5)
				DWORD this_7 = fw.ReadDWord();
			else
			{
				NOT_IMPLEMENTED
			}
		}
		bool v9 = (byte_1DA80F5 == 0);

		DWORD this_6 = v6;

		if (byte_1DA80F5)
			v6 = fw.ReadDWord();
		else
		{
			NOT_IMPLEMENTED
		}
	}

	DWORD v36 = 0;
	DWORD v35 = 0;

	if ((int)v6 > 0)
	{
		std::vector<DWORD> data;
		for (DWORD i = 0; i < v6; i++)
		{
			if (byte_1DA80F5)
			{
				DWORD val = fw.ReadDWord();
				data.push_back(val);

				if (val < 0x7D0)
					v36++;
				else
					v35++;
			}
			else
			{
				NOT_IMPLEMENTED
			}
		}
	}

	DWORD v27 = 0;

	if (byte_1DA80F5)
		v27 = fw.ReadDWord();
	else
	{
		NOT_IMPLEMENTED
	}

	for (DWORD i = 0; i < v27; i++)
		parse_save_6_1_1(fw, v36, v35);
}

void parse_save_6_1_1(FileWalker &fw, const DWORD &a2, const DWORD &a3)
{
	if (a2 > 0)
	{
		std::vector<DWORD> data;

		for (DWORD i = 0; i < a2; i++)
		{
			if (byte_1DA80F5)
				data.push_back(fw.ReadDWord());
			else
			{
				NOT_IMPLEMENTED
			}
		}
	}

	if (a3 > 0)
	{
		for (DWORD i = 0; i < a3; i++)
		{
			if (byte_1DA80F5)
			{
				MemData<DWORD> memData;
				initMemData(memData, fw);

				// DWORD v5 = fw.ReadDWord();

				// if (v5 <= 1600)
				//{
				//	MemData<WORD> memData;
				//	initMemData(memData, fw);

				//	if (!buf_6_eq_85)
				//	{
				//		NOT_IMPLEMENTED
				//	}
				//}
				// else
				//{
				//	MemData<WORD> memData;
				//	initMemData(memData, fw);

				//	if (!buf_6_eq_85)
				//	{
				//		NOT_IMPLEMENTED
				//	}
				//}
			}
			else
			{
				NOT_IMPLEMENTED
			}
		}
	}
}

void parse_save_7(FileWalker &fw)
{
	BYTE v1 = fw.ReadByte();
	if (v1 != 1)
		return;

	DWORD v4 = fw.ReadDWord();

	for (DWORD i = 0; i < v4; i++)
	{
		BYTE v8 = fw.ReadByte();

		if (v8 < 0xFAu)
			BYTE v14 = fw.ReadByte();
		else
			DWORD v11 = fw.ReadDWord();
	}
}

bool read2Buffer(const TCHAR *pFilePath, std::vector<BYTE> &buffer)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

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

int main()
{
	std::vector<BYTE> data;
	const TCHAR pFileName[] = TEXT("SaveData02.sav");
	if (!read2Buffer(pFileName, data))
	{
		tcerr << TEXT("Failed to load input file: ") << pFileName << std::endl;
		return -1;
	}

	FileWalker fw(data);

	if (step0(fw))
		std::cout << "Parsing successful" << std::endl;
	else
		std::cout << "Parsing failed" << std::endl;

	return 0;
}
