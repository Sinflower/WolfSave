/*
 *  File: Database.h
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

#include "FileWalker.h"
#include "MemData.h"

// TODO:
//  - Implement decryption

namespace wolfrpg
{

class Field
{
public:
	Field() = default;

	explicit Field(FileWalker& fw)
	{
		m_name = initMemData<DWORD>(fw);
	}

	explicit Field(const MemData<DWORD>& name)
		: m_name(name)
	{
	}

	void SetUnknown1(const MemData<DWORD>& unknown1)
	{
		m_unknown1 = unknown1;
	}

	const MemData<DWORD>& GetUnknown1() const
	{
		return m_unknown1;
	}

	void SetStringArgs(const MemDataVec<DWORD>& stringArgs)
	{
		m_stringArgs = stringArgs;
	}

	const MemDataVec<DWORD>& GetStringArgs() const
	{
		return m_stringArgs;
	}

	void SetArgs(const DWORDS& args)
	{
		m_args = args;
	}

	const DWORDS& GetArgs() const
	{
		return m_args;
	}

	const MemData<DWORD>& GetName() const
	{
		return m_name;
	}

	friend std::ostream& operator<<(std::ostream& os, const Field& field)
	{
		os << "Field: " << field.m_name;

		return os;
	}

private:
	MemData<DWORD> m_name          = MemData<DWORD>();
	MemData<DWORD> m_unknown1      = MemData<DWORD>();
	MemDataVec<DWORD> m_stringArgs = {};
	DWORDS m_args                  = {};
};

using Fields = std::vector<Field>;

class Data
{
public:
	Data() = default;

	explicit Data(FileWalker& fw)
	{
		m_name = initMemData<DWORD>(fw);
	}

	explicit Data(const MemData<DWORD>& name)
		: m_name(name)
	{
	}

	const MemData<DWORD>& GetName() const
	{
		return m_name;
	}

	friend std::ostream& operator<<(std::ostream& os, const Data& data)
	{
		os << "Data: " << data.m_name << std::endl;

		return os;
	}

private:
	MemData<DWORD> m_name;
};

using Datas = std::vector<Data>;

class Type
{
public:
	Type() = default;

	explicit Type(FileWalker& fw)
	{
		m_name         = initMemData<DWORD>(fw);
		DWORD fieldCnt = fw.ReadUInt32();
		for (DWORD i = 0; i < fieldCnt; i++)
			m_fields.push_back(Field(fw));

		DWORD dataCnt = fw.ReadUInt32();
		for (DWORD i = 0; i < dataCnt; i++)
			m_data.push_back(Data(fw));

		m_description = initMemData<DWORD>(fw);

		m_fieldTypeListSize = fw.ReadUInt32();
		DWORD index         = 0;

		for (index = 0; index < m_fields.size(); index++)
			fw.ReadUInt8();

		fw.Skip(m_fieldTypeListSize - index);

		index = fw.ReadUInt32();
		for (DWORD i = 0; i < index; i++)
			m_fields[i].SetUnknown1(initMemData<DWORD>(fw));

		index = fw.ReadUInt32();
		for (DWORD i = 0; i < index; i++)
		{
			MemDataVec<DWORD> stringArgs;
			DWORD argsCnt = fw.ReadUInt32();
			for (DWORD j = 0; j < argsCnt; j++)
				stringArgs.push_back(initMemData<DWORD>(fw));
			m_fields[i].SetStringArgs(stringArgs);
		}

		index = fw.ReadUInt32();
		for (DWORD i = 0; i < index; i++)
		{
			DWORDS args;
			DWORD argsCnt = fw.ReadUInt32();
			for (DWORD j = 0; j < argsCnt; j++)
				args.push_back(fw.ReadUInt32());
			m_fields[i].SetArgs(args);
		}

		index = fw.ReadUInt32();
		for (DWORD i = 0; i < index; i++)
			fw.ReadUInt32();
	}

	const Datas& GetData() const
	{
		return m_data;
	}

	const std::string GetDataStr(const uint32_t index) const
	{
		if (index < m_data.size())
		{
			std::string str = m_data[index].GetName().toString();
			if (str.empty()) str = "BLANK";
			return str;
		}

		return "";
	}

	const std::string GetName() const
	{
		return m_name.toString();
	}

	const Fields& GetFields() const
	{
		return m_fields;
	}

	const std::string GetFieldName(const DWORD& index) const
	{
		//return "";
		if (index < m_fields.size())
			return m_fields[index].GetName().toString();

		return "";
	}

	void ToJson(JsonDumper& jd) const
	{
		jd.EnterSection("Type");

		jd.Dump(m_name.toString(), "Name");
		jd.Dump(static_cast<DWORD>(m_fields.size()), "FieldCount");
		for (const auto& field : m_fields)
			jd.Dump(field.GetName().toString(), "Field");

		jd.Dump(static_cast<DWORD>(m_data.size()), "DataCount");
		for (const auto& data : m_data)
			jd.Dump(data.GetName().toString(), "Data");

		jd.LeaveSection();
	}

	void FromJson(JsonReader& jr)
	{
		jr.EnterSection("Type");

		m_name         = initFromData<DWORD>(jr.Read<std::string>("Name"));
		DWORD fieldCnt = jr.Read<DWORD>("FieldCount");
		for (DWORD i = 0; i < fieldCnt; i++)
			m_fields.push_back(Field(initFromData<DWORD>(jr.Read<std::string>("Field"))));

		DWORD dataCnt = jr.Read<DWORD>("DataCount");
		for (DWORD i = 0; i < dataCnt; i++)
			m_data.push_back(Data(initFromData<DWORD>(jr.Read<std::string>("Data"))));


		jr.LeaveSection();
	}

	friend std::ostream& operator<<(std::ostream& os, const Type& type)
	{
		os << "Type: " << type.m_name;
		for (const auto& field : type.m_fields)
			os << std::endl << field;

		return os;
	}

private:
	MemData<DWORD> m_name = MemData<DWORD>();
	MemData<DWORD> m_description = MemData<DWORD>();
	Fields m_fields = {};
	Datas m_data = {};
	DWORD m_fieldTypeListSize = 0;
};

using Types = std::vector<Type>;

class Database
{
public:
	Database() = default;

	Database(const tString& projectFileName)
	{
		m_valid = init(projectFileName);
	}

	const Types& GetTypes() const
	{
		return m_types;
	}

	const Type& GetType(const uint32_t& index) const
	{
		if (index < m_types.size())
			return m_types[index];

		return m_invalidType;
	}

	const bool& IsValid() const
	{
		return m_valid;
	}

	void ToJson(JsonDumper& jd) const
	{
		jd.EnterSection("Database");

		jd.Dump(static_cast<DWORD>(m_types.size()), "TypeCount");
		for (const auto& type : m_types)
			type.ToJson(jd);

		jd.LeaveSection();
	}

	void FromJson(JsonReader& jr)
	{
		jr.EnterSection("Database");

		DWORD typeCnt = jr.Read<DWORD>("TypeCount");
		for (DWORD i = 0; i < typeCnt; i++)
		{
			Type type;
			type.FromJson(jr);
			m_types.push_back(type);
		}

		jr.LeaveSection();

		m_valid = true;
	}

	friend std::ostream& operator<<(std::ostream& os, const Database& db)
	{
		for (const auto& type : db.m_types)
			os << type << std::endl;

		return os;
	}

private:
	bool init(const tString& projectFileName)
	{
		FileWalker fw(projectFileName);

		if(!decrypt(fw))
			return false;

		DWORD typeCnt = fw.ReadUInt32();
		for (DWORD i = 0; i < typeCnt; i++)
			m_types.push_back(Type(fw));

		return true;
	}

	bool isEncrypted(const DWORD& header)
	{
		return (header > 0xFF);
	}

	bool decrypt(FileWalker& fw)
	{
		fw.Seek(0);
		// Get the first 4 bytes of the file
		const uint32_t header = fw.ReadUInt32();

		if (!isEncrypted(header))
		{
			fw.Seek(0);
			return true;
		}

		std::cout << "Trying to find the decryption seed ... " << std::flush;
		// A rough estimation of the min size of the type data
		const uint32_t MIN_TYPE_SIZE = 74;

		uint32_t seed;
		bool found = false;

		for (seed = 0x0; seed <= 0xFF; seed++)
		{
			srand(static_cast<int8_t>(seed));
			uint32_t typeCnt = header;
			uint8_t* pPtr = reinterpret_cast<uint8_t*>(&typeCnt);
	
			for (uint32_t i = 0; i < 4; i++)
			{
				pPtr[i] ^= static_cast<uint8_t>(rand());
				//std::cout << " 0x" << std::hex << static_cast<uint32_t>(pPtr[i]);
			}

			if (typeCnt <= 0xFF)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			std::cout << "Failed" << std::endl;
			std::cerr << "Failed to detect the decryption seed" << std::endl;
			return false;
		}

		std::cout << "Successful" << std::endl;

		std::cout << "Decryption Seed: 0x" << std::hex << seed << std::dec << std::endl;

		srand(static_cast<int8_t>(seed));

		fw.Seek(0);
		PBYTE ptr = fw.Get();
		std::vector<BYTE> data = std::vector<BYTE>(fw.GetSize());
		std::memcpy(data.data(), ptr, fw.GetSize());

		fw.InitData(data);

		ptr = fw.Get();

		for (uint32_t i = 0; i < fw.GetSize(); i++)
			ptr[i] ^= static_cast<uint8_t>(rand());

		fw.Seek(0);

		return true;
	}

private:
	Types m_types;
	Type m_invalidType = Type();

	bool m_valid = false;
};

} // namespace wolfrpg
