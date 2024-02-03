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
				m_var3 = fw.ReadDWord();
				if (m_var3 < 0) return false;

				for (DWORD j = 0; j < m_var3; j++)
				{
					m_var4 = fw.ReadByte();
					if (m_var4 > 0)
					{
						for (DWORD k = 0; k < m_var4; k++)
							m_vars1.push_back(fw.ReadDWord());
					}
				}
			}

			m_var5 = fw.ReadDWord();

			if (m_var5 <= 0x270F)
			{
				if ((int)m_var5 > 0)
				{
					for (DWORD i = 0; i < m_var5; i++)
					{
						m_var6 = fw.ReadDWord();
						if ((int)m_var6 < 0) return false;

						if ((int)m_var6 > 0)
						{
							for (DWORD j = 0; j < m_var6; j++)
								m_vars2.push_back(fw.ReadDWord());
						}
					}
				}

				m_var7 = fw.ReadDWord();

				if ((int)m_var7 >= 0)
				{
					for (DWORD i = 0; i < m_var7; i++)
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

					m_var8 = fw.ReadDWord();

					if ((int)m_var8 < 0 || m_var8 > 10000)
						return false;

					for (DWORD i = 0; i < m_var8; i++)
					{
						m_var9 = fw.ReadByte();

						for (BYTE j = 0; j < m_var9; j++)
							m_vars3.push_back(fw.ReadDWord());
					}

					m_var10 = fw.ReadDWord();

					if (m_var10 <= 10000)
					{
						if ((int)m_var10 <= 0) return false;

						for (DWORD i = 0; i < m_var10; i++)
						{
							m_var11 = fw.ReadByte();

							if (m_var11)
							{
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

private:
	DWORD m_var1  = 0;
	DWORD m_var2  = 0;
	DWORD m_var3  = 0;
	BYTE m_var4   = 0;
	DWORD m_var5  = 0;
	DWORD m_var6  = 0;
	DWORD m_var7  = 0;
	DWORD m_var8  = 0;
	BYTE m_var9   = 0;
	DWORD m_var10 = 0;
	BYTE m_var11  = 0;

	std::vector<DWORD> m_vars1;
	std::vector<DWORD> m_vars2;
	std::vector<DWORD> m_vars3;
	std::vector<MemData<DWORD>> m_mds1New;
	std::vector<MemData<DWORD>> m_mds2New;
	std::vector<MemData<WORD>> m_mds1Old;
	std::vector<MemData<WORD>> m_mds2Old;
};
