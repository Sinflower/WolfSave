﻿/*
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
		TypeData(const wolfrpg::Type &type, const std::vector<uint32_t> &typeConfig, const uint32_t& idx, const int32_t& dis) :
			m_type(type),
			m_typeConfig(typeConfig),
			m_idx(idx),
			m_dis(dis)
		{
		}

		bool Parse(FileWalker &fw)
		{
			uint32_t id = 0;
			// First all numbers, then all strings
			for (const uint32_t &def : m_typeConfig)
			{
				if (def < 2000)
					m_fieldData.push_back({ id, 1, fw.ReadInt32() });

				id++;
			}

			id = 0;

			for (const uint32_t &def : m_typeConfig)
			{
				if (def >= 2000)
					m_fieldData.push_back({ id, 2, 0, initMemData<DWORD>(fw) });

				id++;
			}

			// Sort the fieldData vector by ID
			std::sort(m_fieldData.begin(), m_fieldData.end(), [](const FieldData &a, const FieldData &b) { return a.ID < b.ID; });

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			for (const FieldData &fd : m_fieldData)
			{
				if (fd.Type == 1)
					jd.Dump(fd.Number, m_type.GetFieldName(fd.ID));
				else
					jd.Dump(fd.String, m_type.GetFieldName(fd.ID));
			}
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			std::vector<FieldData> fieldData = {};

			uint32_t id = 0;
			for (const uint32_t &def : m_typeConfig)
			{
				if (def < 2000)
					fieldData.push_back({ id, 1, jr.Read<int32_t>(m_type.GetFieldName(id)) });
				else if (def >= 2000)
					fieldData.push_back({ id, 2, 0, jr.ReadMemData<DWORD>(m_type.GetFieldName(id)) });

				id++;
			}
			
			id = 0;
			for (const uint32_t &def : m_typeConfig)
			{
				if (def < 2000)
					fw.Write(fieldData[id].Number);

				id++;
			}

			id = 0;

			for (const uint32_t &def : m_typeConfig)
			{
				if (def >= 2000)
					fieldData[id].String.write(fw);

				id++;
			}
		}

		std::string name() const
		{
			// TODO: Implement type name processing for other dis types
			// Will require reference to previous type / other databases
			if (m_dis > 1)
				return SaveInterface::name();

			// If the type is empty, call the base class method
			if (m_type.GetDataStr(m_idx).empty())
				return SaveInterface::name();

			return m_type.GetDataStr(m_idx);
		}


	private:
		wolfrpg::Type m_type;
		std::vector<FieldData> m_fieldData = {};
		std::vector<uint32_t> m_typeConfig;
		uint32_t m_idx = 0;
		int32_t m_dis = 0;
	};

	class VariableType : public SaveInterface
	{
	public:
		VariableType(const wolfrpg::Type &type) :
			m_type(type)
		{
		}

		bool Parse(FileWalker &fw)
		{
			m_unknown = fw.ReadInt32();

			int32_t fieldCount = m_unknown;

			if (m_unknown <= -1)
			{
				if (m_unknown <= -2)
					m_dis = fw.ReadInt32();

				m_fieldCount = fw.ReadUInt32();
				fieldCount   = m_fieldCount;
			}

			if (fieldCount > 0)
			{
				for (int32_t i = 0; i < fieldCount; i++)
					m_typeConfig.push_back(fw.ReadUInt32());
			}

			m_typeDataCount = fw.ReadUInt32();

			for (uint32_t i = 0; i < m_typeDataCount; i++)
			{
				TypeData td(m_type, m_typeConfig, i, m_dis);
				td.Parse(fw);
				m_typeData.push_back(td);
			}

			return true;
		}

	protected:
		void dump(JsonDumper &jd) const
		{
			jd.Dump(m_unknown, "Unknown Flag", JsonDumper::DO_NOT_TOUCH);
			if (m_unknown <= -1)
			{
				if (m_unknown <= -2)
					jd.Dump(m_dis, "Data ID Specification", JsonDumper::DO_NOT_TOUCH);

				jd.Dump(m_fieldCount, "Field Count", JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);
			}

			if (!m_typeConfig.empty())
				jd.Dump(m_typeConfig, "Type Configuration", JsonDumper::DO_NOT_TOUCH);

			jd.Dump(m_typeDataCount, "TypeData Count", JsonDumper::COUNTER | JsonDumper::DO_NOT_TOUCH);

			for (const TypeData &td : m_typeData)
				td.Dump(jd);
		}

		std::string name() const
		{
			// If the type is empty, call the base class method
			if (m_type.GetName().empty())
				return SaveInterface::name();

			return m_type.GetName();
		}

		void json2Save(JsonReader &jr, FileWriter &fw) const
		{
			int32_t var1 = jr.Read<int32_t>("Unknown Flag");
			fw.Write(var1);

			int32_t dis = 0;

			if (var1 <= -1)
			{
				if (var1 <= -2)
				{
					dis = jr.Read<int32_t>("Data ID Specification");
					fw.Write(dis);
				}

				var1 = jr.Read<int32_t>("Field Count");
				fw.Write(var1);
			}

			std::vector<uint32_t> vars;

			if (var1 > 0)
			{
				vars = jr.ReadVec<uint32_t>("Type Configuration");
				fw.Write(vars);
			}

			uint32_t var4 = jr.Read<uint32_t>("TypeData Count");
			fw.Write(var4);

			for (uint32_t i = 0; i < var4; i++)
			{
				TypeData td(m_type, vars, i, dis);
				td.Json2Save(jr, fw);
			}
		}

	private:
		int32_t m_unknown        = 0;
		int32_t m_dis            = 0;
		int32_t m_fieldCount     = 0;
		uint32_t m_typeDataCount = 0;

		std::vector<uint32_t> m_typeConfig;
		std::vector<TypeData> m_typeData;
		wolfrpg::Type m_type;
	};

public:
	VariableDatabase() = default;

	void SetDB(const wolfrpg::Database &db)
	{
		m_db = db;
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
	uint8_t m_unknown    = 0;
	uint32_t m_typeCount = 0;
	std::vector<VariableType> m_variableTypes;

	wolfrpg::Database m_db;
};
