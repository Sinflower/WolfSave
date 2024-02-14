#pragma once

#include <vector>

#include "MemData.h"
#include "SaveBase.h"
#include "SavePart1.h"

class SavePart4 : public SaveInterface
{
public:
	SavePart4() = default;
	bool Parse(FileWalker& fw)
	{
		if (!check()) return false;

		m_savePart1_1.SetFileVersion(m_fileVersion);
		m_savePart1_1.Parse(fw);

		m_var1 = fw.ReadDWord();

		for (DWORD i = 0; i < m_var1; i++)
		{
			SavePart1_1 sp;
			sp.SetFileVersion(m_fileVersion);
			if (!sp.Parse(fw)) return false;
			m_savePart1_1s.push_back(sp);
		}

		m_var2 = fw.ReadByte();
		m_var3 = fw.ReadByte();

		m_var4 = fw.ReadDWord();

		if ((int)m_var4 > 0)
		{
			for (DWORD i = 0; i < m_var4; i++)
				m_vars1.push_back(fw.ReadDWord());
		}

		if (m_fileVersion < 0x8A)
			return true;

		m_var5 = fw.ReadDWord();

		if ((int)m_var5 > 0)
		{
			for (DWORD i = 0; i < m_var5; i++)
				m_vars2.push_back(fw.ReadQWord());
		}

		return true;
	}

protected:
	void dump(JsonDumper& jd) const
	{
		m_savePart1_1.Dump(jd);

		jd.Dump(m_var1, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
		for (const auto& sp : m_savePart1_1s)
			sp.Dump(jd);

		jd.Dump(m_var2);
		jd.Dump(m_var3);
		jd.Dump(m_var4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

		jd.Dump(m_vars1);

		if (m_fileVersion >= 0x8A)
		{
			jd.Dump(m_var5, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			jd.Dump(m_vars2);
		}
	}

private:
	DWORD m_var1 = 0;
	BYTE m_var2  = 0;
	BYTE m_var3  = 0;
	DWORD m_var4 = 0;
	DWORD m_var5 = 0;

	std::vector<DWORD> m_vars1;
	std::vector<QWORD> m_vars2;

	SavePart1_1 m_savePart1_1;
	std::vector<SavePart1_1> m_savePart1_1s;
};
