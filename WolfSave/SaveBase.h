/*
 *  File: SaveBase.h
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

	virtual std::string name() const
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
