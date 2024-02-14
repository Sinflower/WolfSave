#pragma once

#include <Windows.h>
#include <iostream>

#include "FileWalker.h"
#include "FileWriter.h"
#include "JsonDumper.h"
#include "JsonReader.h"
#include "Utils.h"

class SaveInterface
{
public:
	virtual void SetFileVersion(const WORD &fileVersion)
	{
		m_fileVersion = fileVersion;
	}

	virtual bool Parse(FileWalker &fw) = 0;
	virtual void Dump(JsonDumper &jd) const final
	{
		jd.EnterSection(name());

		dump(jd);

		jd.LeaveSection();
	}

	virtual void Json2Save(JsonReader &jr, FileWriter &fw) const final
	{
		jr.EnterSection(name());

		json2Save(jr, fw);

		jr.LeaveSection();
	}

protected:
	bool check() const
	{
		if (m_fileVersion == 0)
		{
			std::cerr << "Error: File Version is not set" << std::endl;
			return false;
		}

		return true;
	}

	virtual void dump(JsonDumper &jd) const = 0;

	virtual void json2Save(JsonReader &jr, FileWriter &fw) const
	{
		std::cout << "IMPLEMENT ME: " << name() << std::endl;
	}

private:
	std::string name() const
	{
		std::string name = typeid(*this).name();
		// Remove the class name prefix
		std::size_t p = name.find_first_of(" ");
		if (p != std::string::npos)
			name = name.substr(p + 1);

		// Remove potential namespace prefixes
		p = name.find_first_of("::");
		if (p != std::string::npos)
			name = name.substr(p + 2);

		return name;
	}

protected:
	WORD m_fileVersion = 0;
};
