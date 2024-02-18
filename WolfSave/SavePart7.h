/*
 *  File: SavePart7.h
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

using SP7_BBD = std::tuple<BYTE, BYTE, DWORD>;

class SavePart7 : public SaveInterface
{
public:
	SavePart7() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1 = fw.ReadByte();
		if (m_var1 != 1)
			return true;

		m_var2 = fw.ReadDWord();

		for (DWORD i = 0; i < m_var2; i++)
		{
			BYTE v = fw.ReadByte();

			if (v < 0xFAu)
				m_sp7_bbds.push_back(SP7_BBD(v, fw.ReadByte(), 0));
			else
				m_sp7_bbds.push_back(SP7_BBD(v, 0, fw.ReadDWord()));
		}

		return true;
	}

protected:
	void dump(JsonDumper &jd) const
	{
		jd.Dump(m_var1, JsonDumper::DO_NOT_TOUCH);

		if (m_var1 == 1)
		{
			jd.Dump(m_var2, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (const auto &bbd : m_sp7_bbds)
			{
				jd.Dump(std::get<0>(bbd), JsonDumper::DO_NOT_TOUCH);
				if (std::get<0>(bbd) < 0xFAu)
					jd.Dump(std::get<1>(bbd));
				else
					jd.Dump(std::get<2>(bbd));
				jd.LeaveSection();
			}
		}
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		BYTE var1 = jr.Read<BYTE>();
		fw.Write(var1);

		if (var1 == 1)
		{
			DWORD var2 = jr.Read<DWORD>();
			fw.Write(var2);

			for (DWORD i = 0; i < var2; i++)
			{
				BYTE v = jr.Read<BYTE>();
				fw.Write(v);

				if (v < 0xFAu)
					fw.Write(jr.Read<BYTE>());
				else
					fw.Write(jr.Read<DWORD>());
			}
		}
	}

private:
	BYTE m_var1  = 0;
	DWORD m_var2 = 0;
	std::vector<SP7_BBD> m_sp7_bbds;
};
