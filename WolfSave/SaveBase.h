#pragma once

#include <Windows.h>
#include <iostream>

#include "FileWalker.h"
#include "Utils.h"

class SaveInterface
{
public:
	virtual void SetFileVersion(const WORD &fileVersion)
	{
		m_fileVersion = fileVersion;
	}

	virtual bool Parse(FileWalker &fw) = 0;

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
