#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

#include "MemData.h"
#include "Types.h"

class JsonDumper
{
public:
	enum Flags
	{
		NON          = 0,
		DO_NOT_TOUCH = 1 << 0,
		COUNTER      = 1 << 1,
		SKIP         = 1 << 2,
		MAGIC_NUMBER = 1 << 3
	};

public:
	JsonDumper(const tString& filePath) :
		m_filePath(filePath)
	{
	}

	void EnterSection(const std::string& section)
	{
		m_sections.push_back(section);
	}

	void LeaveSection()
	{
		m_sections.pop_back();
	}

	template<typename T>
	void Dump(const T& data, const uint32_t& flags = Flags::NON)
	{
		std::string objName = buildObjName();
		nlohmann::json j    = buildObj(data);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		m_json[objName] = j;
	}

	template<typename T>
	void Dump(const std::vector<T>& vec, const uint32_t& flags = Flags::NON)
	{
		std::string objName = buildObjName();
		nlohmann::json j    = buildVecObj(vec);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		m_json[objName] = j;
	}

	template<typename T>
	void Dump(const MemData<T>& data, const uint32_t& flags = Flags::NON)
	{
		std::string objName = buildObjName();
		nlohmann::json j    = buildObj(data);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		m_json[objName] = j;
	}

	void Write2File()
	{
		// Check if the given file path is valid
		if (m_filePath.empty())
		{
			tcerr << _T("Invalid file path") << std::endl;
			return;
		}

		if (m_json.empty())
		{
			tcerr << _T("No data to write") << std::endl;
			return;
		}

		std::ofstream file(m_filePath);
		file << m_json.dump(4);
		file.close();
	}

private:
	std::string buildObjName()
	{
		std::string name;
		for (const std::string& sec : m_sections)
			name += sec + "-";

		name += std::to_string(m_varCnt++);
		return name;
	}

	template<typename T>
	nlohmann::json buildObj(const T& data)
	{
		nlohmann::json j;
		j["value"]    = data;
		j["byteSize"] = sizeof(T);

		return j;
	}

	template<typename T>
	nlohmann::json buildObj(const MemData<T>& data)
	{
		nlohmann::json j;
		j["value"]    = data.toString();
		j["byteSize"] = data.size;

		return j;
	}

	template<typename T>
	nlohmann::json buildVecObj(const std::vector<T>& vec)
	{
		if (vec.empty()) return nlohmann::json();

		nlohmann::json j;
		j["value"]    = vec;
		j["byteSize"] = sizeof(T);

		return j;
	}

	template<typename T>
	nlohmann::json buildVecObj(const std::vector<MemData<T>>& vec)
	{
		if (vec.empty()) return nlohmann::json();

		nlohmann::json j;

		nlohmann::json arr = nlohmann::json::array();

		for (const MemData<T>& data : vec)
			arr.push_back(data.toString());

		j["value"] = arr;
		j["size"]  = sizeof(T);

		return j;
	}

	std::vector<std::string> flags2Strings(const uint32_t& flags)
	{
		std::vector<std::string> strs;

		if (flags & Flags::DO_NOT_TOUCH)
			strs.push_back("DO_NOT_TOUCH");
		if (flags & Flags::COUNTER)
			strs.push_back("COUNTER");
		if (flags & Flags::SKIP)
			strs.push_back("SKIP");
		if (flags & Flags::MAGIC_NUMBER)
			strs.push_back("MAGIC_NUMBER");

		return strs;
	}

private:
	tString m_filePath = _T("");
	uint64_t m_varCnt  = 0;
	std::vector<std::string> m_sections;
	nlohmann::ordered_json m_json;
};
