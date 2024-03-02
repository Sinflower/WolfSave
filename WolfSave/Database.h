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
//  - Field can be empty, handle accordingly -- should auto trigger use of var_#x

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

	const bool& IsValid() const
	{
		return m_valid;
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
		DWORD typeCnt = fw.ReadUInt32();
		for (DWORD i = 0; i < typeCnt; i++)
			m_types.push_back(Type(fw));

		return true;
	}

private:
	Types m_types;

	bool m_valid = false;
};

} // namespace wolfrpg
