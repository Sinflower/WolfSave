/*
 *  File: SavePart6.h
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

class SavePart6 : public SaveInterface
{
	class SavePart6_1_1 : public SaveInterface
	{
	public:
		SavePart6_1_1(const DWORD &a2 = 0, const DWORD &a3 = 0) :
			m_a2(a2),
			m_a3(a3)
		{
		}

		bool Parse(FileWalker &fw)
		{
			if (m_a2 > 0)
			{
				for (DWORD i = 0; i < m_a2; i++)
					m_vars.push_back(fw.ReadDWord());
			}

			if (m_a3 > 0)
			{
				for (DWORD i = 0; i < m_a3; i++)
				{
					MemData<DWORD> memData;
					initMemData(memData, fw);
					m_mds.push_back(memData);
				}
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			if (!m_vars.empty())
				jd.Dump(m_vars);

			if (!m_mds.empty())
				jd.Dump(m_mds);
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			if (m_a2 > 0)
			{
				for (const DWORD &val : jr.ReadVec<DWORD>())
					fw.Write(val);
			}

			if (m_a3 > 0)
			{
				for (const MemData<DWORD> &md : jr.ReadMemDataVec<DWORD>())
					md.write(fw);
			}
		}

	private:
		std::vector<DWORD> m_vars;
		std::vector<MemData<DWORD>> m_mds;
		DWORD m_a2;
		DWORD m_a3;
	};

	class SavePart6_1 : public SaveInterface
	{
	public:
		// TODO: When saving this maybe include a flag if var3 was read or not
		SavePart6_1()
		{
		}

		bool Parse(FileWalker &fw)
		{
			m_var1   = fw.ReadDWord();
			DWORD v6 = m_var1;

			if ((int)m_var1 <= -1)
			{
				if ((int)m_var1 <= -2)
					m_var2 = fw.ReadDWord();

				m_var3 = fw.ReadDWord();
				v6     = m_var3;
			}

			DWORD v36 = 0;
			DWORD v35 = 0;

			if ((int)v6 > 0)
			{
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

			m_var4 = fw.ReadDWord();

			for (DWORD i = 0; i < m_var4; i++)
			{
				SavePart6_1_1 sp(v36, v35);
				sp.Parse(fw);
				m_savePart6_1_1s.push_back(sp);
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			jd.Dump(m_var1);
			if ((int)m_var1 <= -1)
			{
				if ((int)m_var1 <= -2)
					jd.Dump(m_var2);

				jd.Dump(m_var3);
			}

			if (!m_vars.empty())
				jd.Dump(m_vars);

			jd.Dump(m_var4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (const SavePart6_1_1 &sp : m_savePart6_1_1s)
				sp.Dump(jd);
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			DWORD var1 = jr.Read<DWORD>();
			fw.Write(var1);

			if ((int)var1 <= -1)
			{
				if ((int)var1 <= -2)
					fw.Write(jr.Read<DWORD>());

				var1 = jr.Read<DWORD>();
				fw.Write(var1);
			}

			DWORD v36 = 0;
			DWORD v35 = 0;

			if ((int)var1 > 0)
			{
				std::vector<DWORD> vars = jr.ReadVec<DWORD>();

				for (const DWORD &val : vars)
				{
					fw.Write(val);

					if (val < 0x7D0)
						v36++;
					else
						v35++;
				}
			}

			DWORD var4 = jr.Read<DWORD>();
			fw.Write(var4);

			for (DWORD i = 0; i < var4; i++)
			{
				SavePart6_1_1 sp(v36, v35);
				sp.Json2Save(jr, fw);
			}
		}

	private:
		DWORD m_var1 = 0;
		DWORD m_var2 = 0;
		DWORD m_var3 = 0;
		DWORD m_var4 = 0;

		std::vector<DWORD> m_vars;
		std::vector<SavePart6_1_1> m_savePart6_1_1s;
	};

public:
	SavePart6() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		// Is here 1 byte just skipped?
		m_var1 = fw.ReadByte();

		m_var2 = fw.ReadDWord();
		for (DWORD i = 0; i < m_var2; i++)
		{
			SavePart6_1 sp;
			sp.Parse(fw);
			m_savePart6_1s.push_back(sp);
		}

		// TODO: Here soemthing is done with CDataBase -- Maybe mapping of custom variables, look into it later
		// sub_69CDF0("BasicData/CDataBase");
		// sub_69CDF0(".project");

		return true;
	}

protected:
	void dump(JsonDumper &jd) const
	{
		jd.Dump(m_var1);
		jd.Dump(m_var2, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

		for (const SavePart6_1 &sp : m_savePart6_1s)
			sp.Dump(jd);
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		fw.Write(jr.Read<BYTE>());

		DWORD size = jr.Read<DWORD>();
		fw.Write(size);

		for (DWORD i = 0; i < size; i++)
		{
			SavePart6_1 sp;
			sp.Json2Save(jr, fw);
		}
	}

private:
	BYTE m_var1  = 0;
	DWORD m_var2 = 0;
	std::vector<SavePart6_1> m_savePart6_1s;
};
