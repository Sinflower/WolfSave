#pragma once

#include <Windows.h>
#include <iostream>

#include "FileWalker.h"
#include "Utils.h"
#include "JsonDumper.h"

class SaveInterface
{
public:
	virtual void SetFileVersion(const WORD &fileVersion)
	{
		m_fileVersion = fileVersion;
	}

	std::string Name() const
	{
		std::string name = typeid(*this).name();
		// Remove the class name prefix
		name = name.substr(name.find_first_of(" ") + 1);

		return name;
	}

	virtual bool Parse(FileWalker &fw) = 0;
	virtual void Dump(JsonDumper& jd) const
	{
		std::cout << "IMPLEMENT ME: " << Name() << std::endl;
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

protected:
	WORD m_fileVersion = 0;
};
