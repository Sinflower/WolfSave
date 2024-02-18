/*
 *  File: SavePart5.h
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

#include <vector>

#include "MemData.h"
#include "SaveBase.h"

class SavePart5 : public SaveInterface
{
	class SavePart5_1 : public SaveInterface
	{
	public:
		SavePart5_1(const WORD &fileVersion)
		{
			SetFileVersion(fileVersion);
		}

		bool Parse(FileWalker &fw)
		{
			m_var1 = fw.ReadDWord();

			m_var2 = fw.ReadByte();
			m_var3 = fw.ReadByte();
			m_var4 = fw.ReadWord();
			m_var5 = fw.ReadByte();
			m_var6 = fw.ReadByte();

			initMemData(m_md1, fw);

			m_var7 = fw.ReadDWord();
			m_var8 = fw.ReadDWord();

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

			for (uint32_t i = 0; i < 2; i++)
			{
				for (uint32_t j = 0; j < 3; j++)
					m_vals1.push_back(fw.ReadDWord());
			}

			if (m_fileVersion >= 0x69)
			{
				m_var19 = fw.ReadDWord();
				m_var20 = fw.ReadDWord();
				m_var21 = fw.ReadDWord();
				m_var22 = fw.ReadDWord();
				m_var23 = fw.ReadDWord();
				m_var24 = fw.ReadDWord();
				m_var25 = fw.ReadDWord();
				m_var26 = fw.ReadDWord();
				m_var27 = fw.ReadDWord();
				m_var28 = fw.ReadDWord();
				m_var29 = fw.ReadDWord();
				m_var30 = fw.ReadDWord();
				m_var31 = fw.ReadDWord();
				m_var32 = fw.ReadDWord();
				m_var33 = fw.ReadDWord();
				m_var34 = fw.ReadDWord();
				m_var35 = fw.ReadDWord();
				m_var36 = fw.ReadDWord();
			}
			if (m_fileVersion >= 0x6B)
				m_var37 = fw.ReadByte();

			if (m_fileVersion >= 0x72)
			{
				m_var38 = fw.ReadByte();
				m_var39 = fw.ReadByte();

				for (uint32_t i = 0; i < 4; i++)
					m_vals2.push_back(fw.ReadDWord());

				for (uint32_t i = 0; i < 4; i++)
					m_vals3.push_back(fw.ReadDWord());
			}

			if (m_fileVersion >= 0x73)
			{
				m_var40 = fw.ReadDWord();
				m_var41 = fw.ReadDWord();
				m_var42 = fw.ReadDWord();
				m_var43 = fw.ReadDWord();

				if (m_fileVersion >= 0x74)
				{
					m_var44 = fw.ReadDWord();
					m_var45 = fw.ReadDWord();
					m_var46 = fw.ReadDWord();
					m_var47 = fw.ReadDWord();
					m_var48 = fw.ReadDWord();
				}

				if (m_fileVersion >= 0x75)
				{
					m_var49 = fw.ReadDWord();
					m_var50 = fw.ReadDWord();
					m_var51 = fw.ReadDWord();
					m_var52 = fw.ReadDWord();
					m_var53 = fw.ReadDWord();
					m_var54 = fw.ReadDWord();
				}

				m_var55 = fw.ReadDWord();
				m_var56 = fw.ReadDWord();
				m_var57 = fw.ReadDWord();
				m_var58 = fw.ReadDWord();
				m_var59 = fw.ReadDWord();
				m_var60 = fw.ReadDWord();
			}

			if (m_fileVersion >= 0x76)
			{
				m_var61 = fw.ReadDWord();
				m_var62 = fw.ReadDWord();
				m_var63 = fw.ReadDWord();
			}

			if (m_fileVersion < 0x81)
				return true;

			m_var64 = fw.ReadDWord();
			m_var65 = fw.ReadDWord();
			m_var66 = fw.ReadDWord();
			m_var67 = fw.ReadDWord();
			m_var68 = fw.ReadDWord();
			m_var69 = fw.ReadDWord();

			m_var70 = fw.ReadDWord();
			m_var71 = fw.ReadDWord();
			m_var72 = fw.ReadDWord();
			m_var73 = fw.ReadDWord();
			m_var74 = fw.ReadDWord();
			m_var75 = fw.ReadDWord();
			m_var76 = fw.ReadDWord();
			m_var77 = fw.ReadDWord();
			m_var78 = fw.ReadDWord();
			m_var79 = fw.ReadDWord();
			m_var80 = fw.ReadDWord();
			m_var81 = fw.ReadDWord();
			m_var82 = fw.ReadDWord();
			m_var83 = fw.ReadDWord();
			m_var84 = fw.ReadDWord();

			if (m_fileVersion >= 0x87)
			{
				m_var85 = fw.ReadDWord();
				m_var86 = fw.ReadDWord();
				m_var87 = fw.ReadDWord();
				m_var88 = fw.ReadDWord();
				m_var89 = fw.ReadDWord();
			}

			if (m_fileVersion >= 0x89)
			{
				m_var90 = fw.ReadDWord();

				if ((int)m_var90 > 0)
				{
					for (DWORD i = 0; i < m_var90; i++)
					{
						DWORD v = fw.ReadDWord();
						m_vals4.push_back(v);

						if ((int)v > 0)
						{
							for (DWORD j = 0; j < v; j++)
								m_vals5.push_back(fw.ReadDWord());
						}
					}
				}

				m_var91 = fw.ReadDWord();
				m_var92 = fw.ReadDWord();
				m_var93 = fw.ReadDWord();
				m_var94 = fw.ReadDWord();
				m_var95 = fw.ReadDWord();
				m_var96 = fw.ReadDWord();
				m_var97 = fw.ReadDWord();
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			jd.Dump(m_var1);

			jd.Dump(m_var2);
			jd.Dump(m_var3);
			jd.Dump(m_var4);
			jd.Dump(m_var5);
			jd.Dump(m_var6);

			jd.Dump(m_md1);

			jd.Dump(m_var7);
			jd.Dump(m_var8);

			jd.Dump(m_var9);
			jd.Dump(m_var10);

			jd.Dump(m_var11);
			jd.Dump(m_var12);
			jd.Dump(m_var13);
			jd.Dump(m_var14);
			jd.Dump(m_var15);
			jd.Dump(m_var16);
			jd.Dump(m_var17);
			jd.Dump(m_var18);

			jd.Dump(m_vals1);

			if (m_fileVersion >= 0x69)
			{
				jd.Dump(m_var19);
				jd.Dump(m_var20);
				jd.Dump(m_var21);
				jd.Dump(m_var22);
				jd.Dump(m_var23);
				jd.Dump(m_var24);
				jd.Dump(m_var25);
				jd.Dump(m_var26);
				jd.Dump(m_var27);
				jd.Dump(m_var28);
				jd.Dump(m_var29);
				jd.Dump(m_var30);
				jd.Dump(m_var31);
				jd.Dump(m_var32);
				jd.Dump(m_var33);
				jd.Dump(m_var34);
				jd.Dump(m_var35);
				jd.Dump(m_var36);
			}
			if (m_fileVersion >= 0x6B)
				jd.Dump(m_var37);

			if (m_fileVersion >= 0x72)
			{
				jd.Dump(m_var38);
				jd.Dump(m_var39);
				jd.Dump(m_vals2);
				jd.Dump(m_vals3);
			}

			if (m_fileVersion >= 0x73)
			{
				jd.Dump(m_var40);
				jd.Dump(m_var41);
				jd.Dump(m_var42);
				jd.Dump(m_var43);

				if (m_fileVersion >= 0x74)
				{
					jd.Dump(m_var44);
					jd.Dump(m_var45);
					jd.Dump(m_var46);
					jd.Dump(m_var47);
					jd.Dump(m_var48);
				}

				if (m_fileVersion >= 0x75)
				{
					jd.Dump(m_var49);
					jd.Dump(m_var50);
					jd.Dump(m_var51);
					jd.Dump(m_var52);
					jd.Dump(m_var53);
					jd.Dump(m_var54);
				}

				jd.Dump(m_var55);
				jd.Dump(m_var56);
				jd.Dump(m_var57);
				jd.Dump(m_var58);
				jd.Dump(m_var59);
				jd.Dump(m_var60);
			}

			if (m_fileVersion >= 0x76)
			{
				jd.Dump(m_var61);
				jd.Dump(m_var62);
				jd.Dump(m_var63);
			}

			if (m_fileVersion < 0x81)
				return;

			jd.Dump(m_var64);
			jd.Dump(m_var65);
			jd.Dump(m_var66);
			jd.Dump(m_var67);
			jd.Dump(m_var68);
			jd.Dump(m_var69);

			jd.Dump(m_var70);
			jd.Dump(m_var71);
			jd.Dump(m_var72);
			jd.Dump(m_var73);
			jd.Dump(m_var74);
			jd.Dump(m_var75);
			jd.Dump(m_var76);
			jd.Dump(m_var77);
			jd.Dump(m_var78);
			jd.Dump(m_var79);
			jd.Dump(m_var80);
			jd.Dump(m_var81);
			jd.Dump(m_var82);
			jd.Dump(m_var83);
			jd.Dump(m_var84);

			if (m_fileVersion >= 0x87)
			{
				jd.Dump(m_var85);
				jd.Dump(m_var86);
				jd.Dump(m_var87);
				jd.Dump(m_var88);
				jd.Dump(m_var89);
			}

			if (m_fileVersion >= 0x89)
			{
				jd.Dump(m_var90, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

				jd.Dump(m_vals4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
				jd.Dump(m_vals5);

				jd.Dump(m_var91);
				jd.Dump(m_var92);
				jd.Dump(m_var93);
				jd.Dump(m_var94);
				jd.Dump(m_var95);
				jd.Dump(m_var96);
				jd.Dump(m_var97);
			}
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			fw.Write(jr.Read<DWORD>());

			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<WORD>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());

			jr.ReadMemData<WORD>().write(fw);

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			fw.Write(jr.ReadVec<DWORD>());

			if (m_fileVersion >= 0x69)
			{
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
			}
			if (m_fileVersion >= 0x6B)
				fw.Write(jr.Read<BYTE>());

			if (m_fileVersion >= 0x72)
			{
				fw.Write(jr.Read<BYTE>());
				fw.Write(jr.Read<BYTE>());

				fw.Write(jr.ReadVec<DWORD>());
				fw.Write(jr.ReadVec<DWORD>());
			}

			if (m_fileVersion >= 0x73)
			{
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());

				if (m_fileVersion >= 0x74)
				{
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
				}

				if (m_fileVersion >= 0x75)
				{
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
					fw.Write(jr.Read<DWORD>());
				}

				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
			}

			if (m_fileVersion >= 0x76)
			{
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
			}

			if (m_fileVersion < 0x81)
				return;

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			if (m_fileVersion >= 0x87)
			{
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
			}

			if (m_fileVersion >= 0x89)
			{
				DWORD var90 = jr.Read<DWORD>();
				fw.Write(var90);

				std::vector<DWORD> vals4 = jr.ReadVec<DWORD>();
				std::vector<DWORD> vals5 = jr.ReadVec<DWORD>();

				// Get an iterator to the first element of the vector
				auto it4 = vals4.begin();
				auto it5 = vals4.begin();

				for (DWORD i = 0; i < var90; i++)
				{
					fw.Write(*it4);

					if ((int)(*it4) > 0)
					{
						for (DWORD j = 0; j < *it4; j++)
						{
							fw.Write(*it5);
							it5++;
						}

						it4++;
					}
				}

				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
				fw.Write(jr.Read<DWORD>());
			}
		}

	private:
		DWORD m_var1  = 0;
		BYTE m_var2   = 0;
		BYTE m_var3   = 0;
		WORD m_var4   = 0;
		BYTE m_var5   = 0;
		BYTE m_var6   = 0;
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
		DWORD m_var21 = 0;
		DWORD m_var22 = 0;
		DWORD m_var23 = 0;
		DWORD m_var24 = 0;
		DWORD m_var25 = 0;
		DWORD m_var26 = 0;
		DWORD m_var27 = 0;
		DWORD m_var28 = 0;
		DWORD m_var29 = 0;
		DWORD m_var30 = 0;
		DWORD m_var31 = 0;
		DWORD m_var32 = 0;
		DWORD m_var33 = 0;
		DWORD m_var34 = 0;
		DWORD m_var35 = 0;
		DWORD m_var36 = 0;
		BYTE m_var37  = 0;
		BYTE m_var38  = 0;
		BYTE m_var39  = 0;
		DWORD m_var40 = 0;
		DWORD m_var41 = 0;
		DWORD m_var42 = 0;
		DWORD m_var43 = 0;
		DWORD m_var44 = 0;
		DWORD m_var45 = 0;
		DWORD m_var46 = 0;
		DWORD m_var47 = 0;
		DWORD m_var48 = 0;
		DWORD m_var49 = 0;
		DWORD m_var50 = 0;
		DWORD m_var51 = 0;
		DWORD m_var52 = 0;
		DWORD m_var53 = 0;
		DWORD m_var54 = 0;
		DWORD m_var55 = 0;
		DWORD m_var56 = 0;
		DWORD m_var57 = 0;
		DWORD m_var58 = 0;
		DWORD m_var59 = 0;
		DWORD m_var60 = 0;
		DWORD m_var61 = 0;
		DWORD m_var62 = 0;
		DWORD m_var63 = 0;
		DWORD m_var64 = 0;
		DWORD m_var65 = 0;
		DWORD m_var66 = 0;
		DWORD m_var67 = 0;
		DWORD m_var68 = 0;
		DWORD m_var69 = 0;
		DWORD m_var70 = 0;
		DWORD m_var71 = 0;
		DWORD m_var72 = 0;
		DWORD m_var73 = 0;
		DWORD m_var74 = 0;
		DWORD m_var75 = 0;
		DWORD m_var76 = 0;
		DWORD m_var77 = 0;
		DWORD m_var78 = 0;
		DWORD m_var79 = 0;
		DWORD m_var80 = 0;
		DWORD m_var81 = 0;
		DWORD m_var82 = 0;
		DWORD m_var83 = 0;
		DWORD m_var84 = 0;
		DWORD m_var85 = 0;
		DWORD m_var86 = 0;
		DWORD m_var87 = 0;
		DWORD m_var88 = 0;
		DWORD m_var89 = 0;
		DWORD m_var90 = 0;
		DWORD m_var91 = 0;
		DWORD m_var92 = 0;
		DWORD m_var93 = 0;
		DWORD m_var94 = 0;
		DWORD m_var95 = 0;
		DWORD m_var96 = 0;
		DWORD m_var97 = 0;

		MemData<WORD> m_md1;

		std::vector<DWORD> m_vals1;
		std::vector<DWORD> m_vals2;
		std::vector<DWORD> m_vals3;
		std::vector<DWORD> m_vals4;
		std::vector<DWORD> m_vals5;
	};

public:
	SavePart5() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1 = fw.ReadWord();

		if ((m_var1 & 0x8000u) == 0)
		{
			for (WORD i = 0; i < m_var1; i++)
			{
				SavePart5_1 sp(m_fileVersion);
				if (!sp.Parse(fw)) return false;
				m_savePart5_1s.push_back(sp);
			}
		}

		return true;
	}

protected:
	void dump(JsonDumper &jd) const
	{
		jd.Dump(m_var1);

		if ((m_var1 & 0x8000u) == 0)
		{
			for (const SavePart5_1 &sp : m_savePart5_1s)
				sp.Dump(jd);
		}
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		WORD var1 = jr.Read<WORD>();
		fw.Write(var1);

		if ((var1 & 0x8000u) == 0)
		{
			for (WORD i = 0; i < var1; i++)
			{
				SavePart5_1 sp(m_fileVersion);
				sp.Json2Save(jr, fw);
			}
		}
	}

private:
	WORD m_var1 = 0;
	std::vector<SavePart5_1> m_savePart5_1s;
};
