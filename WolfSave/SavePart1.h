/*
 *  File: SavePart1.h
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

class SavePart1_1 : public SaveInterface
{
	class SavePart1_1_1_1 : public SaveInterface
	{
	public:
		SavePart1_1_1_1()
		{
		}

		bool Parse(FileWalker &fw)
		{
			m_var1 = fw.ReadUInt8();

			m_var2 = fw.ReadUInt8();

			for (BYTE i = 0; i < m_var2; i++)
				m_vars1.push_back(fw.ReadUInt32());

			m_var3 = fw.ReadUInt8();

			for (BYTE i = 0; i < m_var3; i++)
				m_vars2.push_back(fw.ReadUInt8());

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			jd.Dump(m_var1);
			jd.Dump(m_var2, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_vars1);
			jd.Dump(m_var3, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_vars2);
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.ReadVec<DWORD>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.ReadVec<BYTE>());
		}

	private:
		BYTE m_var1 = 0;
		BYTE m_var2 = 0;
		BYTE m_var3 = 0;
		std::vector<DWORD> m_vars1;
		std::vector<BYTE> m_vars2;
	};

	class SavePart1_1_1 : public SaveInterface
	{
	public:
		SavePart1_1_1() = default;

		bool Parse(FileWalker &fw)
		{
			m_var1 = fw.ReadUInt8();
			m_var2 = fw.ReadUInt8();
			m_var3 = fw.ReadUInt8();
			m_var4 = fw.ReadUInt8();
			m_var5 = fw.ReadUInt8();
			m_var6 = fw.ReadUInt8();

			m_var7 = fw.ReadUInt32();

			if (m_var7 > 0x10000)
				return false;

			for (DWORD i = 0; i < m_var7; i++)
			{
				SavePart1_1_1_1 sp;
				sp.Parse(fw);
				m_savePart1_1_1_1s.push_back(sp);
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

			jd.Dump(m_var7, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (DWORD i = 0; i < m_var7; i++)
				m_savePart1_1_1_1s[i].Dump(jd);
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());

			DWORD var = jr.Read<DWORD>();
			fw.Write(var);

			if (var > 0x10000) return;

			for (DWORD i = 0; i < var; i++)
			{
				SavePart1_1_1_1 sp;
				sp.Json2Save(jr, fw);
			}
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

		m_var1 = fw.ReadUInt32();
		m_var2 = fw.ReadUInt32();
		m_var3 = fw.ReadUInt32();
		m_var4 = fw.ReadUInt32();
		m_var5 = fw.ReadUInt8();

		m_var6 = fw.ReadUInt32();

		initMemData(m_md1, fw);

		m_var7  = fw.ReadUInt16();
		m_var8  = fw.ReadUInt16();
		m_var9  = fw.ReadUInt16();
		m_var10 = fw.ReadUInt16();
		m_var11 = fw.ReadUInt8();
		m_var12 = fw.ReadUInt8();

		m_sp1.Parse(fw);

		m_sp2.Parse(fw);

		m_var13 = fw.ReadUInt16();
		m_var14 = fw.ReadUInt16();
		m_var15 = fw.ReadUInt16();
		m_var16 = fw.ReadUInt16();
		m_var17 = fw.ReadUInt16();
		m_var18 = fw.ReadUInt16();
		m_var19 = fw.ReadUInt8();
		m_var20 = fw.ReadUInt8();
		m_var21 = fw.ReadUInt8();

		m_var22 = fw.ReadUInt32();

		if ((int)m_var22 > 0)
		{
			for (DWORD i = 0; i < m_var22; i++)
				m_vars1.push_back(fw.ReadUInt32());
		}

		if (m_fileVersion >= 0x70)
		{
			m_var23 = fw.ReadUInt8();
			m_var24 = fw.ReadUInt8();
		}

		if (m_fileVersion >= 0x73)
		{
			m_var25 = fw.ReadUInt32();
			m_var26 = fw.ReadUInt32();
			m_var27 = fw.ReadUInt32();
			m_var28 = fw.ReadUInt32();
		}

		if (m_fileVersion >= 0x78)
			m_var29 = fw.ReadUInt32();

		if (m_fileVersion >= 0x85)
		{
			m_var30 = fw.ReadUInt32();
			m_var31 = fw.ReadUInt32();
			m_var32 = fw.ReadUInt32();
		}

		if (m_fileVersion >= 0x8A)
		{
			m_var33 = fw.ReadUInt16();

			if ((int16_t)m_var33 > 0)
			{
				for (WORD i = 0; i < m_var33; i++)
				{
					MemData<WORD> dat;
					initMemData(dat, fw);
					m_mds1.push_back(dat);
				}
			}

			m_var34 = fw.ReadUInt32();
			m_var35 = fw.ReadUInt8();
			m_var36 = fw.ReadUInt32();
			m_var37 = fw.ReadUInt32();
		}

		if (m_fileVersion >= 0x8B)
			m_var38 = fw.ReadUInt32();

		if (m_fileVersion < 0x8C)
			return true;

		m_var39 = fw.ReadUInt32();

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

		m_sp1.Dump(jd);
		m_sp2.Dump(jd);

		jd.Dump(m_var13);
		jd.Dump(m_var14);
		jd.Dump(m_var15);
		jd.Dump(m_var16);
		jd.Dump(m_var17);
		jd.Dump(m_var18);
		jd.Dump(m_var19);
		jd.Dump(m_var20);
		jd.Dump(m_var21);
		jd.Dump(m_var22, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

		jd.Dump(m_vars1);
		if (m_fileVersion >= 0x70)
		{
			jd.Dump(m_var23);
			jd.Dump(m_var24);
		}
		if (m_fileVersion >= 0x73)
		{
			jd.Dump(m_var25);
			jd.Dump(m_var26);
			jd.Dump(m_var27);
			jd.Dump(m_var28);
		}
		if (m_fileVersion >= 0x78)
			jd.Dump(m_var29);

		if (m_fileVersion >= 0x85)
		{
			jd.Dump(m_var30);
			jd.Dump(m_var31);
			jd.Dump(m_var32);
		}
		if (m_fileVersion >= 0x8A)
		{
			jd.Dump(m_var33, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_mds1);

			jd.Dump(m_var34);
			jd.Dump(m_var35);
			jd.Dump(m_var36);
			jd.Dump(m_var37);
		}

		if (m_fileVersion >= 0x8B)
			jd.Dump(m_var38);

		if (m_fileVersion >= 0x8C)
			jd.Dump(m_var39);
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<BYTE>());
		fw.Write(jr.Read<DWORD>());

		jr.ReadMemData<WORD>().write(fw);

		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<BYTE>());
		fw.Write(jr.Read<BYTE>());

		SavePart1_1_1 sp1;
		sp1.Json2Save(jr, fw);

		SavePart1_1_1 sp2;
		sp2.Json2Save(jr, fw);

		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<WORD>());
		fw.Write(jr.Read<BYTE>());
		fw.Write(jr.Read<BYTE>());
		fw.Write(jr.Read<BYTE>());

		fw.Write(jr.Read<DWORD>());

		std::vector<DWORD> vars = jr.ReadVec<DWORD>();
		for (const DWORD &var : vars)
			fw.Write(var);

		if (m_fileVersion >= 0x70)
		{
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<BYTE>());
		}

		if (m_fileVersion >= 0x73)
		{
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
		}

		if (m_fileVersion >= 0x78)
			fw.Write(jr.Read<DWORD>());

		if (m_fileVersion >= 0x85)
		{
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
		}

		if (m_fileVersion >= 0x8A)
		{
			fw.Write(jr.Read<WORD>());

			std::vector<MemData<WORD>> mds = jr.ReadMemDataVec<WORD>();
			for (const MemData<WORD> &md : mds)
				md.write(fw);

			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<BYTE>());
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());
		}

		if (m_fileVersion >= 0x8B)
			fw.Write(jr.Read<DWORD>());

		if (m_fileVersion < 0x8C)
			return;

		fw.Write(jr.Read<DWORD>());
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

		m_var1 = fw.ReadUInt32();
		m_var2 = fw.ReadUInt32();
		m_var3 = fw.ReadUInt32();

		if (m_fileVersion >= 0x69)
		{
			m_var4 = fw.ReadUInt32();
			m_var5 = fw.ReadUInt32();

			// This read is only for the next check
			m_var6 = fw.ReadUInt32();

			if (m_var6 != -1)
			{
				// Revert the previous read
				fw.Seek(fw.GetOffset() - 4);

				for (DWORD i = 0; i < 3; i++)
				{
					for (DWORD j = 0; j < m_var4; j++)
					{
						for (DWORD k = 0; k < m_var5; k++)
							m_vars1.push_back(fw.ReadUInt32());
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

		m_var7 = fw.ReadUInt32();

		if ((int)m_var7 > 0)
		{
			if (m_fileVersion >= 0x64)
			{
				for (DWORD i = 0; i < m_var7; i++)
					m_vars2New.push_back(fw.ReadUInt32());
			}
			else
			{
				for (DWORD i = 0; i < m_var7; i++)
					m_vars2Old.push_back(fw.ReadUInt8());
			}
		}

		m_var8 = fw.ReadUInt32();

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
			m_var9  = fw.ReadUInt32();
			m_var10 = fw.ReadUInt32();
			m_var11 = fw.ReadUInt32();
			m_var12 = fw.ReadUInt32();
			m_var13 = fw.ReadUInt32();
			m_var14 = fw.ReadUInt32();
			m_var15 = fw.ReadUInt32();
			m_var16 = fw.ReadUInt32();
			m_var17 = fw.ReadUInt32();
			m_var18 = fw.ReadUInt32();
			m_var19 = fw.ReadUInt32();
		}

		m_var20 = fw.ReadUInt32();

		if ((int)m_var20 > 0)
		{
			for (DWORD i = 0; i < m_var20; i++)
				m_vars3.push_back(fw.ReadUInt32());
		}

		return true;
	}

protected:
	void dump(JsonDumper &jd) const
	{
		jd.Dump(m_var1);
		jd.Dump(m_var2);
		jd.Dump(m_var3);

		if (m_fileVersion >= 0x69)
		{
			jd.Dump(m_var4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_var5, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_var6, JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_vars1);
		}

		jd.Dump(m_md1);
		jd.Dump(m_mds1);
		jd.Dump(m_var7, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
		if (m_fileVersion >= 0x64)
			jd.Dump(m_vars2New);
		else
			jd.Dump(m_vars2Old);

		jd.Dump(m_var8, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
		for (const SavePart1_1 &sp : m_savePart1_1s)
			sp.Dump(jd);

		if (m_fileVersion >= 0x72)
		{
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
			jd.Dump(m_var19);
		}

		jd.Dump(m_var20, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
		jd.Dump(m_vars3);
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<DWORD>());
		fw.Write(jr.Read<DWORD>());

		if (m_fileVersion >= 0x69)
		{
			fw.Write(jr.Read<DWORD>());
			fw.Write(jr.Read<DWORD>());

			// This read is only for the next check
			DWORD v6 = jr.Read<DWORD>();

			if (m_var6 != -1)
			{
				std::vector<DWORD> vars = jr.ReadVec<DWORD>();
				for (const DWORD &var : vars)
					fw.Write(var);
			}
			else
				fw.Write(v6);
		}

		jr.ReadMemData<DWORD>().write(fw);

		std::vector<MemData<DWORD>> mds = jr.ReadMemDataVec<DWORD>();
		for (const MemData<DWORD> &md : mds)
			md.write(fw);

		fw.Write(jr.Read<DWORD>());

		if (m_fileVersion >= 0x64)
		{
			std::vector<DWORD> vars = jr.ReadVec<DWORD>();
			for (const DWORD &var : vars)
				fw.Write(var);
		}
		else
		{
			std::vector<BYTE> vars = jr.ReadVec<BYTE>();
			for (const BYTE &var : vars)
				fw.Write(var);
		}

		DWORD v = jr.Read<DWORD>();
		fw.Write(v);

		for (DWORD i = 0; i < v; i++)
		{
			SavePart1_1 sp;
			sp.SetFileVersion(m_fileVersion);
			sp.Json2Save(jr, fw);
		}

		if (m_fileVersion >= 0x72)
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
		}

		fw.Write(jr.Read<DWORD>());

		std::vector<DWORD> vars = jr.ReadVec<DWORD>();
		for (const DWORD &var : vars)
			fw.Write(var);
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
