#pragma once

#include <vector>

#include "MemData.h"
#include "SaveBase.h"

class SavePart6 : public SaveInterface
{
	class SavePart6_1_1 : public SaveInterface
	{
	public:
		SavePart6_1_1(FileWalker &fw, const DWORD &a2, const DWORD &a3)
		{
			if (a2 > 0)
			{
				for (DWORD i = 0; i < a2; i++)
					m_vars.push_back(fw.ReadDWord());
			}

			if (a3 > 0)
			{
				for (DWORD i = 0; i < a3; i++)
				{
					MemData<DWORD> memData;
					initMemData(memData, fw);
					m_mds.push_back(memData);
				}
			}
		}

		bool Parse(FileWalker &fw)
		{
			std::cerr << "SavePart6_1_1::Parse() not implemented" << std::endl;
			return false;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			if (!m_vars.empty())
				jd.Dump(m_vars);

			if (!m_mds.empty())
				jd.Dump(m_mds);
		}

	private:
		std::vector<DWORD> m_vars;
		std::vector<MemData<DWORD>> m_mds;
	};

	class SavePart6_1 : public SaveInterface
	{
	public:
		// TODO: When saving this maybe include a flag if var3 was read or not
		SavePart6_1(FileWalker &fw)
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
				m_savePart6_1_1s.push_back(SavePart6_1_1(fw, v36, v35));
		}

		bool Parse(FileWalker &fw)
		{
			std::cerr << "SavePart6_1::Parse() not implemented" << std::endl;
			return false;
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
			m_savePart6_1s.push_back(SavePart6_1(fw));

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

private:
	BYTE m_var1  = 0;
	DWORD m_var2 = 0;
	std::vector<SavePart6_1> m_savePart6_1s;
};
