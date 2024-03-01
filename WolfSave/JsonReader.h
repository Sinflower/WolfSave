/*
 *  File: JsonReader.h
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

#include <exception>
#include <filesystem>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "MemData.h"
#include "Types.h"

namespace fs = std::filesystem;

// Use a variadic macro here to allow for std::array to be used
// See: https://stackoverflow.com/a/38030284
#define VALIDATE_JSON_OBJ(JSON, TYPE, ...)                                                                                                                           \
	do                                                                                                                                                               \
	{                                                                                                                                                                \
		/* Check if the json object contains the value field */                                                                                                      \
		if (!JSON.contains("value")) return __VA_ARGS__();                                                                                                           \
		/* Check if the typeSize field exists and matches the size of T */                                                                                           \
		if (JSON.contains("typeSize") && JSON["typeSize"].get<uint32_t>() != sizeof(TYPE))                                                                           \
		{                                                                                                                                                            \
			std::string msg = " [" __FUNCTION__ "] Type size mismatch: " + std::to_string(JSON["typeSize"].get<uint32_t>()) + " != " + std::to_string(sizeof(TYPE)); \
			std::cerr << msg << std::endl;                                                                                                                           \
			throw std::runtime_error(msg);                                                                                                                           \
		}                                                                                                                                                            \
	} while (0)

class JsonReader
{
	struct Section
	{
		Section() = default;
		Section(const nlohmann::ordered_json& json) :
			json(json)
		{
		}

		nlohmann::ordered_json json;
		uint64_t varCnt = 0;

		std::map<std::string, uint32_t> sectionCnt = std::map<std::string, uint32_t>();
	};

public:
	JsonReader(const tString& filePath)
	{
		// Check if the config file exists
		if (!fs::exists(filePath))
			throw std::runtime_error(" [" __FUNCTION__ "] File does not exist");

		std::ifstream file(filePath);
		file >> m_json;
	}

	void EnterSection(const std::string& section)
	{
		if (curSection())
		{
			curSection()->sectionCnt.try_emplace(section, 0);
			curSection()->sectionCnt[section]++;
		}

		const std::string secName = buildSecName(section);
		nlohmann::ordered_json secJson;

		if (m_sections.empty())
			secJson = m_json[secName];
		else
			secJson = m_sections.back().json[secName];

		m_sections.push_back(Section(secJson));
	}

	void LeaveSection()
	{
		m_sections.pop_back();
	}

	template<typename T>
	T Read(const std::string& name = "")
	{
		const nlohmann::ordered_json json = getJson(name);
		return getObj<T>(json);
	}

	template<typename T>
	std::vector<T> ReadVec()
	{
		const nlohmann::ordered_json json = getJson();
		return getVecObj<T>(json);
	}

	template<typename T>
	std::vector<MemData<T>> ReadMemDataVec()
	{
		const nlohmann::ordered_json json = getJson();
		return getMemDataVecObj<T>(json);
	}

	template<typename T, size_t U>
	std::array<T, U> ReadArr()
	{
		const nlohmann::ordered_json json = getJson();
		return getArrObj<T, U>(json);
	}

	template<typename T>
	MemData<T> ReadMemData()
	{
		const nlohmann::ordered_json json = getJson();
		return getMemDataObj<T>(json);
	}

private:
	Section* curSection()
	{
		if (m_sections.empty()) return nullptr;

		return &m_sections.back();
	}

	const Section* curSection() const
	{
		if (m_sections.empty()) return nullptr;

		return &m_sections.back();
	}

	nlohmann::ordered_json getJson(const std::string& name = "")
	{
		return curSection()->json[(name.empty() ? buildObjName() : name)];
	}

	std::string buildObjName()
	{
		std::string name = "var_#" + std::to_string(curSection()->varCnt++);
		return name;
	}

	std::string buildSecName(const std::string& secName) const
	{
		std::string name = secName;

		if (curSection() != nullptr)
			name += "_#" + std::to_string(curSection()->sectionCnt.at(secName));

		return name;
	}

	template<typename T>
	T getObj(const nlohmann::ordered_json& json) const
	{
		VALIDATE_JSON_OBJ(json, T, T);

		return json["value"].get<T>();
	}

	template<typename T>
	MemData<T> getMemDataObj(const nlohmann::ordered_json& json) const
	{
		VALIDATE_JSON_OBJ(json, T, MemData<T>);

		return initFromData<T>(json["value"].get<std::string>(), json["readSize"].get<bool>());
	}

	template<typename T>
	std::vector<T> getVecObj(const nlohmann::ordered_json& json) const
	{
		VALIDATE_JSON_OBJ(json, T, std::vector<T>);

		return json["value"].get<std::vector<T>>();
	}

	template<typename T>
	std::vector<MemData<T>> getMemDataVecObj(const nlohmann::ordered_json& json) const
	{
		VALIDATE_JSON_OBJ(json, T, std::vector<MemData<T>>);

		std::vector<MemData<T>> vec;

		for (const std::string& data : json["value"])
			vec.push_back(initFromData<T>(data, json["readSize"].get<bool>()));

		return vec;
	}

	template<typename T, std::size_t U>
	std::array<T, U> getArrObj(const nlohmann::ordered_json& json) const
	{
		VALIDATE_JSON_OBJ(json, T, std::array<T, U>);

		return json["value"].get<std::array<T, U>>();
	}

private:
	std::vector<Section> m_sections;
	nlohmann::ordered_json m_json;
};
