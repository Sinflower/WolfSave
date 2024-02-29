/*
 *  File: VariableDatabase.h
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

/*
Save_Part6 looks like it's the Variable DataBase
m_var2 is the number of Types
SavePart6_1 is the container for a Type
 - m_var0 = ? -- Negative number -> Signed
 - m_var1 = Data ID Specification
 - m_var2 = Number of Fields
 - m_var3 = Array containing the Field configuration, i.e., if string (2xxx) or number (1xxx) -- Lower numbers are the Field IDs
 - m_var4 = Number of Data entries for the current Type
*/

#pragma once

#include <vector>

#include "MemData.h"
#include "SaveBase.h"

class VariableDatabase : public SaveInterface
{
	class TypeData : public SaveInterface
	{
		struct FieldData
		{
			DWORD ID = 0;
			DWORD Type = 1; // 1 - Number, 2 - String
			DWORD Number = 0;
			MemData<DWORD> String = MemData<DWORD>();
		};

	public:
		TypeData(const std::vector<DWORD>& typeDefinition) :
			m_typeDefinition(typeDefinition)
		{
		}

		bool Parse(FileWalker &fw)
		{
			// First all numbers, then all strings
			for (const DWORD& def : m_typeDefinition)
			{
				if (def >= 2000) continue;

				m_fieldData.push_back({def - 1000, 1,  fw.ReadDWord() });
			}

			for (const DWORD &def : m_typeDefinition)
			{
				if (def < 2000) continue;

				m_fieldData.push_back({ def - 2000, 2, 0, initMemData<DWORD>(fw) });
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			// Not the best in terms of performance or style but ¯\_(ツ)_/¯
			for (const FieldData& fd : m_fieldData)
			{
				if (fd.Type == 1)
					jd.Dump(fd.Number);
			}

			for (const FieldData& fd : m_fieldData)
			{
				if (fd.Type == 2)
					jd.Dump(fd.String);
			}
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			for (const DWORD &def : m_typeDefinition)
			{
				if (def >= 2000) continue;

				fw.Write(jr.Read<DWORD>());
			}

			for (const DWORD &def : m_typeDefinition)
			{
				if (def < 2000) continue;

				jr.ReadMemData<DWORD>().write(fw);
			}
		}

	private:
		std::vector<FieldData> m_fieldData;
		std::vector<DWORD> m_typeDefinition;
	};

	class VariableType : public SaveInterface
	{
	public:
		// TODO: When saving this maybe include a flag if var3 was read or not
		VariableType() = default;

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

			DWORD numberCount = 0;
			DWORD stringCount = 0;

			if ((int)v6 > 0)
			{
				for (DWORD i = 0; i < v6; i++)
					m_vars.push_back(fw.ReadDWord());
			}

			m_var4 = fw.ReadDWord();

			for (DWORD i = 0; i < m_var4; i++)
			{
				TypeData td(m_vars);
				td.Parse(fw);
				m_typeDatas.push_back(td);
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

				jd.Dump(m_var3, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			}

			if (!m_vars.empty())
				jd.Dump(m_vars);

			jd.Dump(m_var4, JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (const TypeData &td : m_typeDatas)
				td.Dump(jd);
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

			std::vector<DWORD> vars;

			if ((int)var1 > 0)
			{
				vars = jr.ReadVec<DWORD>();
				fw.Write(vars);
			}

			DWORD var4 = jr.Read<DWORD>();
			fw.Write(var4);

			for (DWORD i = 0; i < var4; i++)
			{
				TypeData td(vars);
				td.Json2Save(jr, fw);
			}
		}

	private:
		DWORD m_var1 = 0;
		DWORD m_var2 = 0;
		DWORD m_var3 = 0;
		DWORD m_var4 = 0;

		std::vector<DWORD> m_vars;
		std::vector<TypeData> m_typeDatas;
	};

public:
	VariableDatabase() = default;
	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		// Is here 1 byte just skipped?
		m_var1 = fw.ReadByte();

		m_var2 = fw.ReadDWord();
		for (DWORD i = 0; i < m_var2; i++)
		{
			VariableType vt;
			vt.Parse(fw);
			m_variableTypes.push_back(vt);
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

		for (const VariableType &vt : m_variableTypes)
			vt.Dump(jd);
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		fw.Write(jr.Read<BYTE>());

		DWORD size = jr.Read<DWORD>();
		fw.Write(size);

		for (DWORD i = 0; i < size; i++)
		{
			VariableType vt;
			vt.Json2Save(jr, fw);
		}
	}

private:
	BYTE m_var1  = 0;
	DWORD m_var2 = 0;
	std::vector<VariableType> m_variableTypes;
};
