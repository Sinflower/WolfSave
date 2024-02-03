#pragma once

#include <vector>

#include "SaveBase.h"
#include "MemData.h"

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
		BYTE m_var1  = 0;
		BYTE m_var2  = 0;
		BYTE m_var3  = 0;
		BYTE m_var4  = 0;
		BYTE m_var5  = 0;
		BYTE m_var6  = 0;
		DWORD m_var7 = 0;

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
	DWORD m_var1  = 0;
	DWORD m_var2  = 0;
	DWORD m_var3  = 0;
	DWORD m_var4  = 0;
	BYTE m_var5   = 0;
	DWORD m_var6  = 0;
	WORD m_var7   = 0;
	WORD m_var8   = 0;
	WORD m_var9   = 0;
	WORD m_var10  = 0;
	BYTE m_var11  = 0;
	BYTE m_var12  = 0;
	WORD m_var13  = 0;
	WORD m_var14  = 0;
	WORD m_var15  = 0;
	WORD m_var16  = 0;
	WORD m_var17  = 0;
	WORD m_var18  = 0;
	BYTE m_var19  = 0;
	BYTE m_var20  = 0;
	BYTE m_var21  = 0;
	DWORD m_var22 = 0;
	BYTE m_var23  = 0;
	BYTE m_var24  = 0;
	DWORD m_var25 = 0;
	DWORD m_var26 = 0;
	DWORD m_var27 = 0;
	DWORD m_var28 = 0;
	DWORD m_var29 = 0;
	DWORD m_var30 = 0;
	DWORD m_var31 = 0;
	DWORD m_var32 = 0;
	WORD m_var33  = 0;
	DWORD m_var34 = 0;
	BYTE m_var35  = 0;
	DWORD m_var36 = 0;
	DWORD m_var37 = 0;
	DWORD m_var38 = 0;
	DWORD m_var39 = 0;

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
	DWORD m_var1  = 0;
	DWORD m_var2  = 0;
	DWORD m_var3  = 0;
	DWORD m_var4  = 0;
	DWORD m_var5  = 0;
	DWORD m_var6  = 0;
	DWORD m_var7  = 0;
	DWORD m_var8  = 0;
	DWORD m_var9  = 0;
	DWORD m_var10 = 0;
	DWORD m_var11 = 0;
	DWORD m_var12 = 0;
	DWORD m_var13 = 0;
	DWORD m_var14 = 0;
	DWORD m_var15 = 0;
	DWORD m_var16 = 0;
	DWORD m_var17 = 0;
	DWORD m_var18 = 0;
	DWORD m_var19 = 0;
	DWORD m_var20 = 0;

	std::vector<DWORD> m_vars1;
	std::vector<DWORD> m_vars2New;
	std::vector<BYTE> m_vars2Old;
	std::vector<DWORD> m_vars3;

	MemData<DWORD> m_md1;
	std::vector<MemData<DWORD>> m_mds1;

	std::vector<SavePart1_1> m_savePart1_1s;
};
