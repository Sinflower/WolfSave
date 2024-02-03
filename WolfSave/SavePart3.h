#pragma once

#include <vector>

#include "MemData.h"
#include "SaveBase.h"

class SavePart3 : public SaveInterface
{
public:
	SavePart3() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		m_var1 = fw.ReadDWord();
		m_var2 = fw.ReadDWord();

		if ((int)m_var2 >= 0)
		{
			for (DWORD i = 0; i < m_var2; i++)
			{
				DWORD v1 = fw.ReadDWord();
				if ((int)v1 < 0) return false;
				m_vars1.push_back(v1);

				for (DWORD j = 0; j < v1; j++)
				{
					BYTE v2 = fw.ReadByte();
					m_vars2.push_back(v2);
					for (BYTE k = 0; k < v2; k++)
						m_vars3.push_back(fw.ReadDWord());
				}
			}

			m_var3 = fw.ReadDWord();

			if (m_var3 <= 0x270F)
			{
				if ((int)m_var3 > 0)
				{
					for (DWORD i = 0; i < m_var3; i++)
					{
						DWORD v = fw.ReadDWord();
						if ((int)v < 0) return false;

						m_vars4.push_back(v);

						for (DWORD j = 0; j < v; j++)
							m_vars5.push_back(fw.ReadDWord());
					}
				}

				m_var4 = fw.ReadDWord();

				if ((int)m_var4 >= 0)
				{
					for (DWORD i = 0; i < m_var4; i++)
					{
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

					m_var5 = fw.ReadDWord();

					if ((int)m_var5 < 0 || m_var5 > 10000)
						return false;

					for (DWORD i = 0; i < m_var5; i++)
					{
						BYTE v = fw.ReadByte();
						m_vars6.push_back(v);

						for (BYTE j = 0; j < v; j++)
							m_vars7.push_back(fw.ReadDWord());
					}

					m_var6 = fw.ReadDWord();

					if (m_var6 <= 10000)
					{
						if ((int)m_var6 <= 0) return false;

						for (DWORD i = 0; i < m_var6; i++)
						{
							BYTE v = fw.ReadByte();
							m_vars8.push_back(v);

							if (v)
							{
								if (m_fileVersion < 0x6F)
								{
									for (DWORD j = 0; j < v; j++)
									{
										MemData<WORD> memData;
										initMemData(memData, fw);
										m_mds2Old.push_back(memData);
									}
								}
								else
								{
									for (DWORD j = 0; j < v; j++)
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

	void Dump(JsonDumper &jd) const
	{
		jd.EnterSection(Name());

		jd.Dump(m_var1);
		jd.Dump(m_var2, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

		if (m_var2 >= 0)
		{
			jd.Dump(m_vars1);
			jd.Dump(m_vars2);
			jd.Dump(m_vars3);

			jd.Dump(m_var3, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			if (m_var3 <= 0x270F)
			{
				jd.Dump(m_vars4);
				jd.Dump(m_vars5);

				jd.Dump(m_var4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

				if (m_var4 >= 0)
				{
					if (m_fileVersion < 0x6F)
						jd.Dump(m_mds1Old);
					else
						jd.Dump(m_mds1New);

					jd.Dump(m_var5, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
					jd.Dump(m_vars6);
					jd.Dump(m_vars7);

					jd.Dump(m_var6, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

					if (m_var6 <= 10000)
					{
						jd.Dump(m_vars8);

						if (m_fileVersion < 0x6F)
							jd.Dump(m_mds2Old);
						else
							jd.Dump(m_mds2New);
					}
				}
			}
		}

		jd.LeaveSection();
	}

private:
	DWORD m_var1 = 0;
	DWORD m_var2 = 0;
	DWORD m_var3 = 0;
	DWORD m_var4 = 0;
	DWORD m_var5 = 0;
	DWORD m_var6 = 0;

	std::vector<DWORD> m_vars1;
	std::vector<BYTE> m_vars2;
	std::vector<DWORD> m_vars3;
	std::vector<DWORD> m_vars4;
	std::vector<DWORD> m_vars5;
	std::vector<BYTE> m_vars6;
	std::vector<DWORD> m_vars7;
	std::vector<BYTE> m_vars8;
	std::vector<MemData<DWORD>> m_mds1New;
	std::vector<MemData<DWORD>> m_mds2New;
	std::vector<MemData<WORD>> m_mds1Old;
	std::vector<MemData<WORD>> m_mds2Old;
};
