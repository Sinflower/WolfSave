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

#include "Database.h"
#include "MemData.h"
#include "SaveBase.h"

class VariableDatabase : public SaveInterface
{
	class TypeData : public SaveInterface
	{
		struct FieldData
		{
			uint32_t ID           = 0;
			uint32_t Type         = 1; // 1 - Number, 2 - String
			int32_t Number        = 0;
			MemData<DWORD> String = MemData<DWORD>();
		};

	public:
		TypeData(const wolfrpg::Type& type, const std::vector<uint32_t> &typeDefinition) :
			m_type(type),
			m_typeDefinition(typeDefinition)
		{
		}

		bool Parse(FileWalker &fw)
		{
			uint32_t id = 0;
			// First all numbers, then all strings
			for (const uint32_t &def : m_typeDefinition)
			{
				if (def < 2000)
					m_fieldData.push_back({ id, 1, fw.ReadInt32() });

				id++;
			}

			id = 0;

			for (const uint32_t &def : m_typeDefinition)
			{
				if (def >= 2000)
					m_fieldData.push_back({ id, 2, 0, initMemData<DWORD>(fw) });

				id++;
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			// Not the best in terms of performance or style but ¯\_(ツ)_/¯
			for (const FieldData &fd : m_fieldData)
			{
				if (fd.Type == 1)
					jd.Dump(fd.Number, m_type.GetFieldName(fd.ID));
			}

			for (const FieldData &fd : m_fieldData)
			{
				if (fd.Type == 2)
					jd.Dump(fd.String, m_type.GetFieldName(fd.ID));
			}
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			uint32_t id = 0;
			for (const uint32_t &def : m_typeDefinition)
			{
				if (def < 2000)
					fw.Write(jr.Read<DWORD>(m_type.GetFieldName(id)));

				id++;
			}

			id = 0;

			for (const uint32_t &def : m_typeDefinition)
			{
				if (def >= 2000)
					jr.ReadMemData<DWORD>(m_type.GetFieldName(id)).write(fw);

				id++;
			}
		}

	private:
		wolfrpg::Type m_type;
		std::vector<FieldData> m_fieldData = {};
		std::vector<uint32_t> m_typeDefinition;
	};

	class VariableType : public SaveInterface
	{
	public:
		// TODO: When saving this maybe include a flag if var3 was read or not
		VariableType(const wolfrpg::Type &type) :
			m_type(type)
		{
		}

		bool Parse(FileWalker &fw)
		{
			m_var1     = fw.ReadInt32();
			int32_t v6 = m_var1;

			if (m_var1 <= -1)
			{
				if (m_var1 <= -2)
					m_var2 = fw.ReadInt32();

				m_var3 = fw.ReadUInt32();
				v6     = m_var3;
			}

			if (v6 > 0)
			{
				for (int32_t i = 0; i < v6; i++)
					m_vars.push_back(fw.ReadUInt32());
			}

			m_var4 = fw.ReadUInt32();

			for (uint32_t i = 0; i < m_var4; i++)
			{
				TypeData td(m_type, m_vars);
				td.Parse(fw);
				m_typeDatas.push_back(td);
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			jd.Dump(m_var1, "Unknown Flag #1", JsonDumper::DO_NOT_TOUCH);
			if ((int)m_var1 <= -1)
			{
				if ((int)m_var1 <= -2)
					jd.Dump(m_var2, "Unknown Flag #2", JsonDumper::DO_NOT_TOUCH);

				jd.Dump(m_var3, "Field Count", JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			}

			if (!m_vars.empty())
				jd.Dump(m_vars, "Field Configuration", JsonDumper::DO_NOT_TOUCH);

			jd.Dump(m_var4, "TypeData Count", JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (const TypeData &td : m_typeDatas)
				td.Dump(jd);
		}

		std::string name() const
		{
			return m_type.GetName();
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			int32_t var1 = jr.Read<int32_t>("Unknown Flag #1");
			fw.Write(var1);

			if (var1 <= -1)
			{
				if (var1 <= -2)
					fw.Write(jr.Read<int32_t>("Unknown Flag #2"));

				var1 = jr.Read<int32_t>("Field Count");
				fw.Write(var1);
			}

			std::vector<uint32_t> vars;

			if (var1 > 0)
			{
				vars = jr.ReadVec<uint32_t>("Field Configuration");
				fw.Write(vars);
			}

			uint32_t var4 = jr.Read<uint32_t>("TypeData Count");
			fw.Write(var4);

			for (uint32_t i = 0; i < var4; i++)
			{
				TypeData td(m_type, vars);
				td.Json2Save(jr, fw);
			}
		}

	private:
		int32_t m_var1  = 0;
		int32_t m_var2  = 0;
		int32_t m_var3  = 0;
		uint32_t m_var4 = 0;

		std::vector<uint32_t> m_vars;
		std::vector<TypeData> m_typeDatas;
		wolfrpg::Type m_type;
	};

public:
	VariableDatabase()
	{
		m_db = wolfrpg::Database(_T("CDataBase.project"));
	}

	bool Parse(FileWalker &fw)
	{
		if (!check()) return false;

		// Is here 1 byte just skipped?
		m_unknown = fw.ReadUInt8();

		m_typeCount = fw.ReadUInt32();
		for (uint32_t i = 0; i < m_typeCount; i++)
		{
			VariableType vt(m_db.GetType(i));
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
		jd.Dump(m_unknown, JsonDumper::DO_NOT_TOUCH);
		jd.Dump(m_typeCount, "Number of Types", JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

		for (const VariableType &vt : m_variableTypes)
			vt.Dump(jd);
	}

	void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		fw.Write(jr.Read<uint8_t>());

		uint32_t size = jr.Read<uint32_t>("Number of Types");
		fw.Write(size);

		for (uint32_t i = 0; i < size; i++)
		{
			VariableType vt(m_db.GetType(i));
			vt.Json2Save(jr, fw);
		}
	}

private:
	uint8_t m_unknown  = 0;
	uint32_t m_typeCount = 0;
	std::vector<VariableType> m_variableTypes;

	wolfrpg::Database m_db;
};
