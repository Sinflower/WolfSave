/*
 *  File: SavePart3.h
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

class SavePart3 : public SaveInterface
{
public:
	SavePart3() = default;
	bool Parse(FileWalker& fw)
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

protected:
	void dump(JsonDumper& jd) const
	{
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
	}

	void json2Save(JsonReader& jr, FileWriter& fw) const
	{
		fw.Write(jr.Read<DWORD>());
		DWORD var2 = jr.Read<DWORD>();
		fw.Write(var2);

		if (var2 >= 0)
		{
			std::vector<DWORD> vars1 = jr.ReadVec<DWORD>();
			std::vector<BYTE> vars2  = jr.ReadVec<BYTE>();
			std::vector<DWORD> vars3 = jr.ReadVec<DWORD>();

			// Get an iterator to the first element of the vector
			auto it1 = vars1.begin();
			auto it2 = vars2.begin();
			auto it3 = vars3.begin();

			for (DWORD i = 0; i < var2; i++)
			{
				fw.Write(*it1);

				for (DWORD j = 0; j < *it1; j++)
				{
					fw.Write(*it2);

					for (BYTE k = 0; k < *it2; k++)
					{
						fw.Write(*it3);
						it3++;
					}

					it2++;
				}

				it1++;
			}

			DWORD var3 = jr.Read<DWORD>();
			fw.Write(var3);

			if (var3 <= 0x270F)
			{
				std::vector<DWORD> vars4 = jr.ReadVec<DWORD>();
				std::vector<DWORD> vars5 = jr.ReadVec<DWORD>();

				// Get an iterator to the first element of the vector
				auto it4 = vars4.begin();
				auto it5 = vars5.begin();

				for (DWORD i = 0; i < var3; i++)
				{
					fw.Write(*it4);

					for (DWORD j = 0; j < *it4; j++)
					{
						fw.Write(*it5);
						it5++;
					}

					it4++;
				}

				DWORD var4 = jr.Read<DWORD>();
				fw.Write(var4);

				if (var4 >= 0)
				{
					if (m_fileVersion < 0x6F)
					{
						std::vector<MemData<WORD>> mds = jr.ReadMemDataVec<WORD>();
						for (const auto& md : mds)
							md.write(fw);
					}
					else
					{
						std::vector<MemData<DWORD>> mds = jr.ReadMemDataVec<DWORD>();
						for (const auto& md : mds)
							md.write(fw);
					}

					DWORD var5 = jr.Read<DWORD>();
					fw.Write(var5);

					std::vector<BYTE> vars6  = jr.ReadVec<BYTE>();
					std::vector<DWORD> vars7 = jr.ReadVec<DWORD>();

					// Get an iterator to the first element of the vector
					auto it6 = vars6.begin();
					auto it7 = vars7.begin();

					for (DWORD i = 0; i < var5; i++)
					{
						fw.Write(*it6);

						for (BYTE j = 0; j < *it6; j++)
						{
							fw.Write(*it7);
							it7++;
						}

						it6++;
					}

					DWORD var6 = jr.Read<DWORD>();
					fw.Write(var6);

					if (var6 <= 10000)
					{
						std::vector<BYTE> vars8 = jr.ReadVec<BYTE>();

						std::vector<MemData<WORD>> mdsOld;
						std::vector<MemData<DWORD>> mdsNew;

						if (m_fileVersion < 0x6F)
							mdsOld = jr.ReadMemDataVec<WORD>();
						else
							mdsNew = jr.ReadMemDataVec<DWORD>();

						// Get an iterator to the first element of the vector
						auto itMdsOld = mdsOld.begin();
						auto itMdsNew = mdsNew.begin();

						for (DWORD i = 0; i < var6; i++)
						{
							fw.Write(vars8[i]);

							if (vars8[i])
							{
								for (DWORD j = 0; j < vars8[i]; j++)
								{
									if (m_fileVersion < 0x6F)
									{
										(*itMdsOld).write(fw);
										itMdsOld++;
									}
									else
									{
										(*itMdsNew).write(fw);
										itMdsNew++;
									}
								}
							}
						}
					}
				}
			}
		}
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
