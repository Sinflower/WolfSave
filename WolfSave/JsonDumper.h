#pragma once

#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "MemData.h"
#include "Types.h"

class JsonDumper
{
	struct Section
	{
		Section() = default;
		Section(const std::string& name) :
			name(name)
		{
		}

		std::string name;
		nlohmann::ordered_json json = nlohmann::ordered_json();
		uint64_t varCnt             = 0;
	};

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
		m_sectionCnt.try_emplace(section, 0);
		m_sectionCnt[section]++;

		m_sections.push_back(Section(section));
	}

	void LeaveSection()
	{
		const std::string secName            = buildSecName(curSection()->name);
		const nlohmann::ordered_json secJson = curSection()->json;

		m_sections.pop_back();

		// If the current section is a sub-section, add it to the parent section
		if (m_sections.empty())
			m_json[secName] = secJson;
		else
			m_sections.back().json[secName] = secJson;
	}

	template<typename T>
	void Dump(const T& data, const uint32_t& flags = Flags::NON)
	{
		nlohmann::ordered_json j = buildObj(data);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		addObj(j);
	}

	template<typename T>
	void Dump(const std::vector<T>& vec, const uint32_t& flags = Flags::NON)
	{
		nlohmann::ordered_json j = buildVecObj(vec);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		addObj(j);
	}

	template<typename T, size_t U>
	void Dump(const std::array<T, U>& vec, const uint32_t& flags = Flags::NON)
	{
		nlohmann::ordered_json j = buildArrObj(vec);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		addObj(j);
	}

	template<typename T>
	void Dump(const MemData<T>& data, const uint32_t& flags = Flags::NON)
	{
		nlohmann::ordered_json j = buildObj(data);

		if (flags != Flags::NON)
			j["flags"] = flags2Strings(flags);

		addObj(j);
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
	Section* curSection()
	{
		if (m_sections.empty()) return nullptr;

		return &m_sections.back();
	}

	void addObj(const nlohmann::ordered_json& j)
	{
		const std::string objName   = buildObjName();
		curSection()->json[objName] = j;
	}

	std::string buildObjName()
	{
		std::string name = "var_#" + std::to_string(curSection()->varCnt++);
		return name;
	}

	std::string buildSecName(const std::string& secName) const
	{
		std::string name = secName;

		name += "_#" + std::to_string(m_sectionCnt.at(secName));

		return name;
	}

	template<typename T>
	nlohmann::ordered_json buildObj(const T& data) const
	{
		nlohmann::ordered_json j;
		j["value"]    = data;
		j["typeSize"] = sizeof(T);

		return j;
	}

	template<typename T>
	nlohmann::ordered_json buildObj(const MemData<T>& data) const
	{
		nlohmann::ordered_json j;
		j["value"]    = data.toString();
		j["typeSize"] = sizeof(T);
		j["readSize"] = data.readSize;

		return j;
	}

	template<typename T>
	nlohmann::ordered_json buildVecObj(const std::vector<T>& vec) const
	{
		if (vec.empty()) return nlohmann::ordered_json();

		nlohmann::ordered_json j;
		j["value"]    = vec;
		j["typeSize"] = sizeof(T);

		return j;
	}

	template<typename T>
	nlohmann::ordered_json buildVecObj(const std::vector<MemData<T>>& vec) const
	{
		if (vec.empty()) return nlohmann::ordered_json();

		nlohmann::ordered_json j;

		nlohmann::ordered_json arr = nlohmann::ordered_json::array();

		for (const MemData<T>& data : vec)
			arr.push_back(data.toString());

		j["value"]    = arr;
		j["typeSize"] = sizeof(T);
		j["readSize"] = vec.front().readSize;

		return j;
	}

	template<typename T, std::size_t U>
	nlohmann::ordered_json buildArrObj(const std::array<T, U>& arr) const
	{
		if (U == 0) return nlohmann::ordered_json();

		nlohmann::ordered_json j;
		j["value"]    = arr;
		j["typeSize"] = sizeof(T);
		j["arrCount"] = U;

		return j;
	}

	std::vector<std::string> flags2Strings(const uint32_t& flags) const
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
	std::vector<Section> m_sections;
	std::map<std::string, uint32_t> m_sectionCnt;
	nlohmann::ordered_json m_json;
};
