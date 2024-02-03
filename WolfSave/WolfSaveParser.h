#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "FileWalker.h"
#include "MemData.h"
#include "Types.h"
#include "Utils.h"

class SaveInterface
{
public:
	virtual void SetFileVersion(const WORD &fileVersion)
	{
		m_fileVersion = fileVersion;
	}

	virtual bool Parse(FileWalker &fw) = 0;

protected:
	bool check() const
	{
		if (m_fileVersion == 0)
		{
			std::cerr << "Error: File Version is not set" << std::endl;
			return false;
		}

		return true;
	}

protected:
	WORD m_fileVersion = 0;
};

class SavePart1_1 : public SaveInterface
{
	class SavePart1_1_1_1
	{
	public:
		SavePart1_1_1_1(FileWalker &fw)
		{
			m_var1 = fw.ReadByte();

			m_var2 = fw.ReadByte();

			for (BYTE i = 0; i < m_var2; i++)
				m_vars1.push_back(fw.ReadDWord());

			m_var3 = fw.ReadByte();

			for (BYTE i = 0; i < m_var3; i++)
				m_vars2.push_back(fw.ReadByte());
		}

	private:
		BYTE m_var1;
		BYTE m_var2;
		BYTE m_var3;
		std::vector<DWORD> m_vars1;
		std::vector<BYTE> m_vars2;
	};

	class SavePart1_1_1
	{
	public:
		SavePart1_1_1() = default;

		void Parse(FileWalker &fw)
		{
			m_var1 = fw.ReadByte();
			m_var2 = fw.ReadByte();
			m_var3 = fw.ReadByte();
			m_var4 = fw.ReadByte();
			m_var5 = fw.ReadByte();
			m_var6 = fw.ReadByte();

			m_var7 = fw.ReadDWord();

			if (m_var7 > 0x10000)
				return;

			for (DWORD i = 0; i < m_var7; i++)
				m_savePart1_1_1_1s.push_back(SavePart1_1_1_1(fw));
		}

	private:
		BYTE m_var1;
		BYTE m_var2;
		BYTE m_var3;
		BYTE m_var4;
		BYTE m_var5;
		BYTE m_var6;
		DWORD m_var7;

		std::vector<SavePart1_1_1_1> m_savePart1_1_1_1s;
	};

public:
	SavePart1_1() = default;

	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1 = fw.ReadDWord();
		m_var2 = fw.ReadDWord();
		m_var3 = fw.ReadDWord();
		m_var4 = fw.ReadDWord();
		m_var5 = fw.ReadByte();

		m_var6 = fw.ReadDWord();

		initMemData(m_md1, fw);

		m_var7  = fw.ReadWord();
		m_var8  = fw.ReadWord();
		m_var9  = fw.ReadWord();
		m_var10 = fw.ReadWord();
		m_var11 = fw.ReadByte();
		m_var12 = fw.ReadByte();

		m_sp1.Parse(fw);

		m_sp2.Parse(fw);

		m_var13 = fw.ReadWord();
		m_var14 = fw.ReadWord();
		m_var15 = fw.ReadWord();
		m_var16 = fw.ReadWord();
		m_var17 = fw.ReadWord();
		m_var18 = fw.ReadWord();
		m_var19 = fw.ReadByte();
		m_var20 = fw.ReadByte();
		m_var21 = fw.ReadByte();

		m_var22 = fw.ReadDWord();

		if ((int)m_var22 > 0)
		{
			for (DWORD i = 0; i < m_var22; i++)
				m_vars1.push_back(fw.ReadDWord());
		}

		if (m_fileVersion >= 0x70)
		{
			m_var23 = fw.ReadByte();
			m_var24 = fw.ReadByte();
		}

		if (m_fileVersion >= 0x73)
		{
			m_var25 = fw.ReadDWord();
			m_var26 = fw.ReadDWord();
			m_var27 = fw.ReadDWord();
			m_var28 = fw.ReadDWord();
		}

		if (m_fileVersion >= 0x78)
			m_var29 = fw.ReadDWord();

		if (m_fileVersion >= 0x85)
		{
			m_var30 = fw.ReadDWord();
			m_var31 = fw.ReadDWord();
			m_var32 = fw.ReadDWord();
		}

		if (m_fileVersion >= 0x8A)
		{
			m_var33 = fw.ReadWord();

			if ((int16_t)m_var33 > 0)
			{
				for (WORD i = 0; i < m_var33; i++)
				{
					MemData<WORD> dat;
					initMemData(dat, fw);
					m_mds1.push_back(dat);
				}
			}

			m_var34 = fw.ReadDWord();
			m_var35 = fw.ReadByte();
			m_var36 = fw.ReadDWord();
			m_var37 = fw.ReadDWord();
		}

		if (m_fileVersion >= 0x8B)
			m_var38 = fw.ReadDWord();

		if (m_fileVersion < 0x8C)
			return true;

		m_var39 = fw.ReadDWord();

		return true;
	}

private:
	DWORD m_var1;
	DWORD m_var2;
	DWORD m_var3;
	DWORD m_var4;
	BYTE m_var5;
	DWORD m_var6;
	WORD m_var7;
	WORD m_var8;
	WORD m_var9;
	WORD m_var10;
	BYTE m_var11;
	BYTE m_var12;
	WORD m_var13;
	WORD m_var14;
	WORD m_var15;
	WORD m_var16;
	WORD m_var17;
	WORD m_var18;
	BYTE m_var19;
	BYTE m_var20;
	BYTE m_var21;
	DWORD m_var22;
	BYTE m_var23;
	BYTE m_var24;
	DWORD m_var25;
	DWORD m_var26;
	DWORD m_var27;
	DWORD m_var28;
	DWORD m_var29;
	DWORD m_var30;
	DWORD m_var31;
	DWORD m_var32;
	WORD m_var33;
	DWORD m_var34;
	BYTE m_var35;
	DWORD m_var36;
	DWORD m_var37;
	DWORD m_var38;
	DWORD m_var39;

	MemData<WORD> m_md1;

	SavePart1_1_1 m_sp1;
	SavePart1_1_1 m_sp2;

	std::vector<DWORD> m_vars1;
	std::vector<MemData<WORD>> m_mds1;

	std::vector<SavePart1_1_1> m_savePart1_1_1s;
};

class SavePart1 : public SaveInterface
{
public:
	SavePart1() = default;

	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1 = fw.ReadDWord();
		m_var2 = fw.ReadDWord();
		m_var3 = fw.ReadDWord();

		if (m_fileVersion >= 0x69)
		{
			m_var4 = fw.ReadDWord();
			m_var5 = fw.ReadDWord();

			// This read is only for the next check
			m_var6 = fw.ReadDWord();

			if (m_var6 != -1)
			{
				// Revert the previous read
				fw.Seek(fw.GetOffset() - 4);

				for (DWORD i = 0; i < 3; i++)
				{
					for (DWORD j = 0; j < m_var4; j++)
					{
						for (DWORD k = 0; k < m_var5; k++)
							m_vars1.push_back(fw.ReadDWord());
					}
				}
			}
		}

		initMemData(m_md1, fw);

		DWORD v140 = 7;
		if (m_fileVersion >= 0x73)
			v140 = 15;

		if (m_fileVersion >= 0x8A)
			v140 = 31;

		for (DWORD i = 0; i < v140; i++)
		{
			MemData<DWORD> data;
			initMemData(data, fw);
			m_mds1.push_back(data);
		}

		m_var7 = fw.ReadDWord();

		if ((int)m_var7 > 0)
		{
			if (m_fileVersion >= 0x64)
			{
				for (DWORD i = 0; i < m_var7; i++)
					m_vars2New.push_back(fw.ReadDWord());
			}
			else
			{
				for (DWORD i = 0; i < m_var7; i++)
					m_vars2Old.push_back(fw.ReadByte());
			}
		}

		m_var8 = fw.ReadDWord();

		if ((int)m_var8 > 0)
		{
			for (DWORD i = 0; i < m_var8; i++)
			{
				SavePart1_1 sp;
				sp.SetFileVersion(m_fileVersion);
				if (!sp.Parse(fw)) return false;
				m_savePart1_1s.push_back(sp);
			}
		}

		if (m_fileVersion >= 0x72)
		{
			m_var9  = fw.ReadDWord();
			m_var10 = fw.ReadDWord();
			m_var11 = fw.ReadDWord();
			m_var12 = fw.ReadDWord();
			m_var13 = fw.ReadDWord();
			m_var14 = fw.ReadDWord();
			m_var15 = fw.ReadDWord();
			m_var16 = fw.ReadDWord();
			m_var17 = fw.ReadDWord();
			m_var18 = fw.ReadDWord();
			m_var19 = fw.ReadDWord();
		}

		m_var20 = fw.ReadDWord();

		if ((int)m_var20 > 0)
		{
			for (DWORD i = 0; i < m_var20; i++)
				m_vars3.push_back(fw.ReadDWord());
		}

		return true;
	}

private:
	DWORD m_var1;
	DWORD m_var2;
	DWORD m_var3;
	DWORD m_var4;
	DWORD m_var5;
	DWORD m_var6;
	DWORD m_var7;
	DWORD m_var8;
	DWORD m_var9;
	DWORD m_var10;
	DWORD m_var11;
	DWORD m_var12;
	DWORD m_var13;
	DWORD m_var14;
	DWORD m_var15;
	DWORD m_var16;
	DWORD m_var17;
	DWORD m_var18;
	DWORD m_var19;
	DWORD m_var20;

	std::vector<DWORD> m_vars1;
	std::vector<DWORD> m_vars2New;
	std::vector<BYTE> m_vars2Old;
	std::vector<DWORD> m_vars3;

	MemData<DWORD> m_md1;
	std::vector<MemData<DWORD>> m_mds1;

	std::vector<SavePart1_1> m_savePart1_1s;
};

class SavePart2 : public SaveInterface
{
public:
	SavePart2() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1  = fw.ReadByte();
		m_var2  = fw.ReadByte();
		m_var3  = fw.ReadDWord();
		m_var4  = fw.ReadDWord();
		m_var5  = fw.ReadDWord();
		m_var6  = fw.ReadDWord();
		m_var7  = fw.ReadDWord();
		m_var8  = fw.ReadDWord();
		m_var9  = fw.ReadDWord();
		m_var10 = fw.ReadDWord();
		m_var11 = fw.ReadDWord();
		m_var12 = fw.ReadDWord();
		m_var13 = fw.ReadDWord();
		m_var14 = fw.ReadDWord();
		m_var15 = fw.ReadDWord();
		m_var16 = fw.ReadDWord();
		m_var17 = fw.ReadDWord();
		m_var18 = fw.ReadDWord();
		m_var19 = fw.ReadDWord();
		m_var20 = fw.ReadDWord();
		m_var21 = fw.ReadDWord();
		m_var22 = fw.ReadWord();
		m_var23 = fw.ReadWord();
		m_var24 = fw.ReadWord();
		m_var25 = fw.ReadDWord();
		m_var26 = fw.ReadWord();
		m_var27 = fw.ReadWord();
		m_var28 = fw.ReadWord();
		m_var29 = fw.ReadDWord();
		m_var30 = fw.ReadDWord();
		m_var31 = fw.ReadDWord();
		m_var32 = fw.ReadDWord();
		m_var33 = fw.ReadDWord();
		m_var34 = fw.ReadDWord();
		m_var35 = fw.ReadDWord();
		m_var36 = fw.ReadDWord();
		m_var37 = fw.ReadDWord();
		m_var38 = fw.ReadByte();
		m_var39 = fw.ReadWord();
		m_var40 = fw.ReadDWord();
		m_var41 = fw.ReadDWord();
		m_var42 = fw.ReadWord();
		m_var43 = fw.ReadWord();
		m_var44 = fw.ReadWord();
		m_var45 = fw.ReadWord();
		m_var46 = fw.ReadDWord();
		m_var47 = fw.ReadDWord();
		m_var48 = fw.ReadDWord();
		m_var49 = fw.ReadByte();

		if (m_fileVersion <= 96)
			m_var50 = fw.ReadDWord();

		m_var51 = fw.ReadDWord();

		if (m_fileVersion >= 98)
		{
			m_var52 = fw.ReadDWord();
			m_var53 = fw.ReadDWord();
		}

		if (m_fileVersion >= 100)
		{
			m_var54 = fw.ReadDWord();
			m_var55 = fw.ReadDWord();
			m_var56 = fw.ReadByte();

			m_var57 = fw.ReadDWord();

			if ((int)m_var57 > 0)
			{
				for (DWORD i = 0; i < m_var57; i++)
				{
					MemData<DWORD> memData;
					initMemData(memData, fw);
					m_mds1.push_back(memData);
				}
			}

			m_var58 = fw.ReadDWord();

			if ((int)m_var58 > 0)
			{
				for (DWORD i = 0; i < m_var58; i++)
					m_vars1.push_back(fw.ReadDWord());
			}

			m_var59 = fw.ReadDWord();
			m_var60 = fw.ReadDWord();
			m_var61 = fw.ReadDWord();
			m_var62 = fw.ReadDWord();
			m_var63 = fw.ReadDWord();
		}
		if (m_fileVersion >= 101)
		{
			m_var64 = fw.ReadDWord();
			m_var65 = fw.ReadDWord();
			m_var66 = fw.ReadDWord();
			m_var67 = fw.ReadDWord();
			m_var68 = fw.ReadDWord();
			m_var69 = fw.ReadDWord();
		}
		if (m_fileVersion >= 102)
			m_var70 = fw.ReadWord();

		if (m_fileVersion >= 103)
		{
			initMemData(m_md1, fw);

			initMemData(m_md2, fw);
		}

		if (m_fileVersion >= 104)
			m_var71 = fw.ReadDWord();

		if (m_fileVersion >= 106)
		{
			m_var72 = fw.ReadDWord();
			m_var73 = fw.ReadDWord();
			m_var74 = fw.ReadDWord();
		}

		if (m_fileVersion >= 108)
		{
			initMemData(m_md3, fw);

			m_var75 = fw.ReadDWord();
			m_var76 = fw.ReadDWord();
			m_var77 = fw.ReadDWord();

			initMemData(m_md4, fw);

			m_var78 = fw.ReadDWord();
			m_var79 = fw.ReadDWord();
			m_var80 = fw.ReadDWord();
		}

		if (m_fileVersion >= 109)
		{
			initMemData(m_md5, fw);

			initMemData(m_md6, fw);

			m_var81 = fw.ReadByte();
		}

		if (m_fileVersion >= 110)
			m_var82 = fw.ReadByte();

		if (m_fileVersion >= 119)
		{
			initMemData(m_md7, fw);

			initMemData(m_md8, fw);

			initMemData(m_md9, fw);
		}

		if (m_fileVersion >= 121)
			m_var83 = fw.ReadDWord();

		if (m_fileVersion >= 122)
			m_var84 = fw.ReadDWord();

		if (m_fileVersion >= 124)
			m_var85 = fw.ReadDWord();

		if (m_fileVersion >= 126)
			m_var86 = fw.ReadDWord();

		if (m_fileVersion >= 128)
			m_var87 = fw.ReadDWord();

		if (m_fileVersion >= 129)
		{
			m_var88 = fw.ReadDWord();
			m_var89 = fw.ReadDWord();
		}

		if (m_fileVersion >= 130)
			m_var90 = fw.ReadDWord();

		if (m_fileVersion >= 131)
			m_var91 = fw.ReadDWord();

		if (m_fileVersion >= 132)
		{
			m_var92 = fw.ReadDWord();
			m_var93 = fw.ReadDWord();
			m_var94 = fw.ReadDWord();
			m_var95 = fw.ReadDWord();
		}

		if (m_fileVersion >= 134)
			m_var96 = fw.ReadByte();

		if (m_fileVersion >= 136)
			m_var97 = fw.ReadByte();

		if (m_fileVersion >= 137)
		{
			m_var98  = fw.ReadDWord();
			m_var99  = fw.ReadDWord();
			m_var100 = fw.ReadDWord();
			m_var101 = fw.ReadDWord();

			for (DWORD i = 0; i < 2; i++)
			{
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
				m_vars2.push_back(fw.ReadDWord());
			}
		}

		if (m_fileVersion >= 0x8A)
		{
			m_var102 = fw.ReadDWord();
			m_var103 = fw.ReadDWord();
			m_var104 = fw.ReadDWord();
			m_var105 = fw.ReadDWord();

			initMemData(m_md10, fw);

			m_var106 = fw.ReadDWord();

			if ((int)m_var106 > 0)
			{
				for (DWORD i = 0; i < m_var106; i++)
				{
					MemData<DWORD> memData;
					initMemData(memData, fw);
					m_mds2.push_back(memData);
				}
			}
		}

		if (m_fileVersion < 0x8D) return true;

		m_var107 = fw.ReadByte();
		m_var108 = fw.ReadByte();
		m_var109 = fw.ReadByte();
		m_var110 = fw.ReadByte();
		m_var111 = fw.ReadByte();
		m_var112 = fw.ReadByte();
		m_var113 = fw.ReadByte();
		m_var114 = fw.ReadByte();
		m_var115 = fw.ReadByte();
		m_var116 = fw.ReadByte();
		m_var117 = fw.ReadByte();
		m_var118 = fw.ReadByte();
		m_var119 = fw.ReadByte();
		m_var120 = fw.ReadByte();

		fw.ReadBytesArr(m_bytes);

		m_var121 = fw.ReadByte();
		m_var122 = fw.ReadByte();
		return true;
	}

private:
	BYTE m_var1;
	BYTE m_var2;
	DWORD m_var3;
	DWORD m_var4;
	DWORD m_var5;
	DWORD m_var6;
	DWORD m_var7;
	DWORD m_var8;
	DWORD m_var9;
	DWORD m_var10;
	DWORD m_var11;
	DWORD m_var12;
	DWORD m_var13;
	DWORD m_var14;
	DWORD m_var15;
	DWORD m_var16;
	DWORD m_var17;
	DWORD m_var18;
	DWORD m_var19;
	DWORD m_var20;
	DWORD m_var21;
	WORD m_var22;
	WORD m_var23;
	WORD m_var24;
	DWORD m_var25;
	WORD m_var26;
	WORD m_var27;
	WORD m_var28;
	DWORD m_var29;
	DWORD m_var30;
	DWORD m_var31;
	DWORD m_var32;
	DWORD m_var33;
	DWORD m_var34;
	DWORD m_var35;
	DWORD m_var36;
	DWORD m_var37;
	BYTE m_var38;
	WORD m_var39;
	DWORD m_var40;
	DWORD m_var41;
	WORD m_var42;
	WORD m_var43;
	WORD m_var44;
	WORD m_var45;
	DWORD m_var46;
	DWORD m_var47;
	DWORD m_var48;
	BYTE m_var49;
	DWORD m_var50;
	DWORD m_var51;
	DWORD m_var52;
	DWORD m_var53;
	DWORD m_var54;
	DWORD m_var55;
	BYTE m_var56;
	DWORD m_var57;
	DWORD m_var58;
	DWORD m_var59;
	DWORD m_var60;
	DWORD m_var61;
	DWORD m_var62;
	DWORD m_var63;
	DWORD m_var64;
	DWORD m_var65;
	DWORD m_var66;
	DWORD m_var67;
	DWORD m_var68;
	DWORD m_var69;
	WORD m_var70;
	DWORD m_var71;
	DWORD m_var72;
	DWORD m_var73;
	DWORD m_var74;
	DWORD m_var75;
	DWORD m_var76;
	DWORD m_var77;
	DWORD m_var78;
	DWORD m_var79;
	DWORD m_var80;
	BYTE m_var81;
	BYTE m_var82;
	DWORD m_var83;
	DWORD m_var84;
	DWORD m_var85;
	DWORD m_var86;
	DWORD m_var87;
	DWORD m_var88;
	DWORD m_var89;
	DWORD m_var90;
	DWORD m_var91;
	DWORD m_var92;
	DWORD m_var93;
	DWORD m_var94;
	DWORD m_var95;
	BYTE m_var96;
	BYTE m_var97;
	DWORD m_var98;
	DWORD m_var99;
	DWORD m_var100;
	DWORD m_var101;
	DWORD m_var102;
	DWORD m_var103;
	DWORD m_var104;
	DWORD m_var105;
	DWORD m_var106;
	BYTE m_var107;
	BYTE m_var108;
	BYTE m_var109;
	BYTE m_var110;
	BYTE m_var111;
	BYTE m_var112;
	BYTE m_var113;
	BYTE m_var114;
	BYTE m_var115;
	BYTE m_var116;
	BYTE m_var117;
	BYTE m_var118;
	BYTE m_var119;
	BYTE m_var120;
	BYTE m_var121;
	BYTE m_var122;

	MemData<WORD> m_md1;
	MemData<WORD> m_md2;
	MemData<WORD> m_md3;
	MemData<WORD> m_md4;
	MemData<WORD> m_md5;
	MemData<WORD> m_md6;
	MemData<WORD> m_md7;
	MemData<WORD> m_md8;
	MemData<WORD> m_md9;
	MemData<DWORD> m_md10;

	std::array<BYTE, 0x100> m_bytes;
	std::vector<DWORD> m_vars1;
	std::vector<DWORD> m_vars2;
	std::vector<MemData<DWORD>> m_mds1;
	std::vector<MemData<DWORD>> m_mds2;
};

class SavePart3 : public SaveInterface
{
public:
	SavePart3() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		DWORD m_var1 = fw.ReadDWord();
		DWORD m_var2 = fw.ReadDWord();

		if ((int)m_var2 >= 0)
		{
			for (DWORD i = 0; i < m_var2; i++)
			{
				DWORD m_var3 = fw.ReadDWord();
				if (m_var3 < 0) return false;

				for (DWORD j = 0; j < m_var3; j++)
				{
					BYTE m_var4 = fw.ReadByte();
					if (m_var4 > 0)
					{
						std::vector<DWORD> m_vars1;
						for (DWORD k = 0; k < m_var4; k++)
							m_vars1.push_back(fw.ReadDWord());
					}
				}
			}

			DWORD m_var5 = fw.ReadDWord();

			if (m_var5 <= 0x270F)
			{
				if ((int)m_var5 > 0)
				{
					for (DWORD i = 0; i < m_var5; i++)
					{
						DWORD m_var6 = fw.ReadDWord();
						if ((int)m_var6 < 0) return false;

						if ((int)m_var6 > 0)
						{
							std::vector<DWORD> m_vars2;
							for (DWORD j = 0; j < m_var6; j++)
								m_vars2.push_back(fw.ReadDWord());
						}
					}
				}

				DWORD m_var7 = fw.ReadDWord();

				if ((int)m_var7 >= 0)
				{
					for (DWORD i = 0; i < m_var7; i++)
					{
						std::vector<MemData<WORD>> m_mds1Old;
						std::vector<MemData<DWORD>> m_mds1New;

						if (m_fileVersion < 0x6F)
						{
							// This is a WORD read not a DWORD
							MemData<WORD> memData;
							initMemData(memData, fw);
							m_mds1Old.push_back(memData);
						}
						else
						{
							MemData<DWORD> memData;
							initMemData(memData, fw);
							m_mds1New.push_back(memData);
						}
					}

					DWORD m_var8 = fw.ReadDWord();

					if ((int)m_var8 < 0 || m_var8 > 10000)
						return false;

					for (DWORD i = 0; i < m_var8; i++)
					{
						BYTE m_var9 = fw.ReadByte();

						std::vector<DWORD> m_vars3;

						for (BYTE j = 0; j < m_var9; j++)
							m_vars3.push_back(fw.ReadDWord());
					}

					DWORD m_var10 = fw.ReadDWord();

					if (m_var10 <= 10000)
					{
						if ((int)m_var10 <= 0) return false;

						for (DWORD i = 0; i < m_var10; i++)
						{
							BYTE m_var11 = fw.ReadByte();

							if (m_var11)
							{
								std::vector<MemData<WORD>> m_mds2Old;
								std::vector<MemData<DWORD>> m_mds2New;

								if (m_fileVersion < 0x6F)
								{
									for (DWORD j = 0; j < m_var11; j++)
									{
										MemData<WORD> memData;
										initMemData(memData, fw);
										m_mds2Old.push_back(memData);
									}
								}
								else
								{
									for (DWORD j = 0; j < m_var11; j++)
									{
										MemData<DWORD> memData;
										initMemData(memData, fw);
										m_mds2New.push_back(memData);
									}
								}
							}
						}
					}
				}
			}
		}

		return true;
	}
};

class SavePart4 : public SaveInterface
{
public:
	SavePart4() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		SavePart1_1 m_savePart1_1;
		m_savePart1_1.SetFileVersion(m_fileVersion);
		m_savePart1_1.Parse(fw);

		DWORD m_var1 = fw.ReadDWord();

		for (DWORD i = 0; i < m_var1; i++)
		{
			SavePart1_1 sp;
			sp.SetFileVersion(m_fileVersion);
			if (!sp.Parse(fw)) return false;
			m_savePart1_1s.push_back(sp);
		}

		BYTE m_var2 = fw.ReadByte();
		BYTE m_var3 = fw.ReadByte();

		DWORD m_var4 = fw.ReadDWord();

		if ((int)m_var4 > 0)
		{
			std::vector<DWORD> m_vars1;

			for (DWORD i = 0; i < m_var4; i++)
				m_vars1.push_back(fw.ReadDWord());
		}

		if (m_fileVersion < 0x8A)
			return true;

		DWORD m_var5 = fw.ReadDWord();

		if ((int)m_var5 > 0)
		{
			std::vector<QWORD> m_vars2;

			for (DWORD i = 0; i < m_var5; i++)
				m_vars2.push_back(fw.ReadQWord());
		}

		return true;
	}

private:
	std::vector<SavePart1_1> m_savePart1_1s;
};

class SavePart5 : public SaveInterface
{
	class SavePart5_1
	{
	public:
		SavePart5_1(const WORD &fileVersion, FileWalker &fw)
		{
			DWORD m_var1 = fw.ReadDWord();

			BYTE m_var2 = fw.ReadByte();
			BYTE m_var3 = fw.ReadByte();
			WORD m_var4 = fw.ReadWord();
			BYTE m_var5 = fw.ReadByte();
			BYTE m_var6 = fw.ReadByte();

			MemData<WORD> m_md1;
			initMemData(m_md1, fw);

			DWORD m_var7 = fw.ReadDWord();
			DWORD m_var8 = fw.ReadDWord();

			DWORD m_var9  = fw.ReadDWord();
			DWORD m_var10 = fw.ReadDWord();

			DWORD m_var11 = fw.ReadDWord();
			DWORD m_var12 = fw.ReadDWord();
			DWORD m_var13 = fw.ReadDWord();
			DWORD m_var14 = fw.ReadDWord();
			DWORD m_var15 = fw.ReadDWord();
			DWORD m_var16 = fw.ReadDWord();
			DWORD m_var17 = fw.ReadDWord();
			DWORD m_var18 = fw.ReadDWord();

			for (uint32_t i = 0; i < 2; i++)
			{
				std::vector<DWORD> m_vals1;
				for (uint32_t j = 0; j < 3; j++)
					m_vals1.push_back(fw.ReadDWord());
			}

			if (fileVersion >= 0x69)
			{
				DWORD m_var19 = fw.ReadDWord();
				DWORD m_var20 = fw.ReadDWord();
				DWORD m_var21 = fw.ReadDWord();
				DWORD m_var22 = fw.ReadDWord();
				DWORD m_var23 = fw.ReadDWord();
				DWORD m_var24 = fw.ReadDWord();
				DWORD m_var25 = fw.ReadDWord();
				DWORD m_var26 = fw.ReadDWord();
				DWORD m_var27 = fw.ReadDWord();
				DWORD m_var28 = fw.ReadDWord();
				DWORD m_var29 = fw.ReadDWord();
				DWORD m_var30 = fw.ReadDWord();
				DWORD m_var31 = fw.ReadDWord();
				DWORD m_var32 = fw.ReadDWord();
				DWORD m_var33 = fw.ReadDWord();
				DWORD m_var34 = fw.ReadDWord();
				DWORD m_var35 = fw.ReadDWord();
				DWORD m_var36 = fw.ReadDWord();
			}
			if (fileVersion >= 0x6B)
				BYTE m_var37 = fw.ReadByte();

			if (fileVersion >= 0x72)
			{
				BYTE m_var38 = fw.ReadByte();
				BYTE m_var39 = fw.ReadByte();

				std::vector<DWORD> m_vals2;
				std::vector<DWORD> m_vals3;

				for (uint32_t i = 0; i < 4; i++)
					m_vals2.push_back(fw.ReadDWord());

				for (uint32_t i = 0; i < 4; i++)
					m_vals3.push_back(fw.ReadDWord());
			}

			if (fileVersion >= 0x73)
			{
				DWORD m_var40 = fw.ReadDWord();
				DWORD m_var41 = fw.ReadDWord();
				DWORD m_var42 = fw.ReadDWord();
				DWORD m_var43 = fw.ReadDWord();

				if (fileVersion >= 0x74)
				{
					DWORD m_var44 = fw.ReadDWord();
					DWORD m_var45 = fw.ReadDWord();
					DWORD m_var46 = fw.ReadDWord();
					DWORD m_var47 = fw.ReadDWord();
					DWORD m_var48 = fw.ReadDWord();
				}

				if (fileVersion >= 0x75)
				{
					DWORD m_var49 = fw.ReadDWord();
					DWORD m_var50 = fw.ReadDWord();
					DWORD m_var51 = fw.ReadDWord();
					DWORD m_var52 = fw.ReadDWord();
					DWORD m_var53 = fw.ReadDWord();
					DWORD m_var54 = fw.ReadDWord();
				}

				DWORD m_var55 = fw.ReadDWord();
				DWORD m_var56 = fw.ReadDWord();
				DWORD m_var57 = fw.ReadDWord();
				DWORD m_var58 = fw.ReadDWord();
				DWORD m_var59 = fw.ReadDWord();
				DWORD m_var60 = fw.ReadDWord();
			}

			if (fileVersion >= 0x76)
			{
				DWORD m_var61 = fw.ReadDWord();
				DWORD m_var62 = fw.ReadDWord();
				DWORD m_var63 = fw.ReadDWord();
			}

			if (fileVersion < 0x81)
				return;

			DWORD m_var64 = fw.ReadDWord();
			DWORD m_var65 = fw.ReadDWord();
			DWORD m_var66 = fw.ReadDWord();
			DWORD m_var67 = fw.ReadDWord();
			DWORD m_var68 = fw.ReadDWord();
			DWORD m_var69 = fw.ReadDWord();

			DWORD m_var70 = fw.ReadDWord();
			DWORD m_var71 = fw.ReadDWord();
			DWORD m_var72 = fw.ReadDWord();
			DWORD m_var73 = fw.ReadDWord();
			DWORD m_var74 = fw.ReadDWord();
			DWORD m_var75 = fw.ReadDWord();
			DWORD m_var76 = fw.ReadDWord();
			DWORD m_var77 = fw.ReadDWord();
			DWORD m_var78 = fw.ReadDWord();
			DWORD m_var79 = fw.ReadDWord();
			DWORD m_var80 = fw.ReadDWord();
			DWORD m_var81 = fw.ReadDWord();
			DWORD m_var82 = fw.ReadDWord();
			DWORD m_var83 = fw.ReadDWord();
			DWORD m_var84 = fw.ReadDWord();

			if (fileVersion >= 0x87)
			{
				DWORD m_var85 = fw.ReadDWord();
				DWORD m_var86 = fw.ReadDWord();
				DWORD m_var87 = fw.ReadDWord();
				DWORD m_var88 = fw.ReadDWord();
				DWORD m_var89 = fw.ReadDWord();
			}

			if (fileVersion >= 0x89)
			{
				DWORD m_var90 = fw.ReadDWord();

				if ((int)m_var90 > 0)
				{
					for (DWORD i = 0; i < m_var90; i++)
					{
						DWORD m_var91 = fw.ReadDWord();

						if ((int)m_var91 > 0)
						{
							std::vector<DWORD> m_vals4;

							for (DWORD j = 0; j < m_var91; j++)
								m_vals4.push_back(fw.ReadDWord());
						}
					}
				}

				DWORD m_var92 = fw.ReadDWord();
				DWORD m_var93 = fw.ReadDWord();
				DWORD m_var94 = fw.ReadDWord();
				DWORD m_var95 = fw.ReadDWord();
				DWORD m_var96 = fw.ReadDWord();
				DWORD m_var97 = fw.ReadDWord();
				DWORD m_var98 = fw.ReadDWord();
			}
		};
	};

public:
	SavePart5() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		WORD m_var1 = fw.ReadWord();

		if ((m_var1 & 0x8000u) == 0)
		{
			for (WORD i = 0; i < m_var1; i++)
				m_savePart5_1s.push_back(SavePart5_1(m_fileVersion, fw));
		}

		return true;
	}

private:
	std::vector<SavePart5_1> m_savePart5_1s;
};

class SavePart6 : public SaveInterface
{
	class SavePart6_1_1
	{
	public:
		SavePart6_1_1(FileWalker &fw, const DWORD &a2, const DWORD &a3)
		{
			if (a2 > 0)
			{
				std::vector<DWORD> m_vars;

				for (DWORD i = 0; i < a2; i++)
					m_vars.push_back(fw.ReadDWord());
			}

			if (a3 > 0)
			{
				std::vector<MemData<DWORD>> m_mds;
				for (DWORD i = 0; i < a3; i++)
				{
					MemData<DWORD> memData;
					initMemData(memData, fw);
					m_mds.push_back(memData);
				}
			}
		};
	};

	class SavePart6_1
	{
	public:
		// TODO: When saving this maybe include a flag if var3 was read or not
		SavePart6_1(FileWalker &fw)
		{
			DWORD m_var1 = fw.ReadDWord();
			DWORD v6     = m_var1;

			if ((int)m_var1 <= -1)
			{
				if ((int)m_var1 <= -2)
					DWORD m_var2 = fw.ReadDWord();

				DWORD m_var3 = fw.ReadDWord();
				v6           = m_var3;
			}

			DWORD v36 = 0;
			DWORD v35 = 0;

			if ((int)v6 > 0)
			{
				std::vector<DWORD> m_vars;
				for (DWORD i = 0; i < v6; i++)
				{
					DWORD val = fw.ReadDWord();
					m_vars.push_back(val);

					if (val < 0x7D0)
						v36++;
					else
						v35++;
				}
			}

			DWORD m_vars4 = fw.ReadDWord();

			for (DWORD i = 0; i < m_vars4; i++)
				m_savePart6_1_1s.push_back(SavePart6_1_1(fw, v36, v35));
		};

	private:
		std::vector<SavePart6_1_1> m_savePart6_1_1s;
	};

public:
	SavePart6() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		// Is here 1 byte just skipped?
		BYTE m_var1 = fw.ReadByte();

		DWORD m_var2 = fw.ReadDWord();
		for (DWORD i = 0; i < m_var2; i++)
			m_savePart6_1s.push_back(SavePart6_1(fw));

		// TODO: Here soemthing is done with CDataBase -- Maybe mapping of custom variables, look into it later
		// sub_69CDF0("BasicData/CDataBase");
		// sub_69CDF0(".project");

		return true;
	}

private:
	std::vector<SavePart6_1> m_savePart6_1s;
};

class SavePart7 : public SaveInterface
{
public:
	SavePart7() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		BYTE m_var1 = fw.ReadByte();
		if (m_var1 != 1)
			return true;

		DWORD m_var2 = fw.ReadDWord();

		for (DWORD i = 0; i < m_var2; i++)
		{
			BYTE m_var3 = fw.ReadByte();

			if (m_var3 < 0xFAu)
				BYTE m_var4Old = fw.ReadByte();
			else
				DWORD m_var4New = fw.ReadDWord();
		}

		return true;
	}
};

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