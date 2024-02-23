/*
 *  File: FileWriter.h
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

#include <fstream>
#include <string>
#include <vector>

#include "Utils.h"

using QWORD = uint64_t;

class FileWriterException : public std::exception
{
public:
	explicit FileWriterException(const std::string& what) :
		m_what(what) {}

	explicit FileWriterException(const std::wstring& what) :
		m_what(ws2s(what)) {}

	virtual ~FileWriterException() noexcept {}

	virtual const char* what() const throw()
	{
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class FileWriter
{
public:
	FileWriter() {}
	FileWriter(const std::string& filename) :
		FileWriter(s2ws(filename)) {}

	FileWriter(const std::wstring& filename)
	{
		Open(filename);
	}

	// Disable copy constructor and copy assignment operator
	FileWriter(const FileWriter&)            = delete;
	FileWriter& operator=(const FileWriter&) = delete;

	void Open(const std::wstring& filename)
	{
		m_file = std::fstream(filename, std::ios::out | std::ios::binary);
		if (!m_file.is_open())
			throw(FileWriterException("Failed to open file"));
		m_bufferMode = false;
	}

	~FileWriter()
	{
		if (m_file.is_open())
			m_file.close();
	}

	PBYTE Get()
	{
		return m_buffer.data();
	}

	void SetAt(const uint64_t& offset, const BYTE& value)
	{
		if (offset < m_buffer.size())
			m_buffer[offset] = value;
		else
			throw(FileWriterException("SetAt: offset is larger than buffer size"));
	}

	const uint64_t& GetSize() const
	{
		return m_size;
	}

	void WriteToFile(const std::string& filename)
	{
		WriteToFile(s2ws(filename));
	}

	void WriteToFile(const std::wstring& filename)
	{
		if (m_bufferMode)
		{
			std::ofstream file(filename, std::ios::out | std::ios::binary);
			file.write(reinterpret_cast<const char*>(m_buffer.data()), m_buffer.size());
		}
	}

	template<typename T>
	void Write(const T& data)
	{
		write<T>(data);
	}

	template<typename T>
	void Write(const std::vector<T>& data)
	{
		for (const T& d : data)
			write<T>(d);
	}

	template<std::size_t S>
	void WriteBytesArr(const std::array<BYTE, S>& buffer, const DWORD& size = -1)
	{
		if (size == -1)
			WriteBytes(buffer.data(), buffer.size());
		else if (size <= buffer.size())
			WriteBytes(buffer.data(), size);
		else
			throw(FileWriterException("WriteBytesArr: size is larger than buffer size"));
	}

	void WriteBytesVec(const std::vector<BYTE>& buffer, const DWORD& size = -1)
	{
		if (size == -1)
			WriteBytes(buffer.data(), buffer.size());
		else if (size <= buffer.size())
			WriteBytes(buffer.data(), size);
		else
			throw(FileWriterException("WriteBytesVec: size is larger than buffer size"));
	}

	void WriteBytes(LPCVOID pBuffer, const int& size)
	{
		WriteBytes(pBuffer, static_cast<DWORD>(size));
	}

	void WriteBytes(LPCVOID pBuffer, const std::size_t& size)
	{
		WriteBytes(pBuffer, static_cast<DWORD>(size));
	}

	void WriteBytes(LPCVOID pBuffer, const DWORD& size)
	{
		m_size += size;
		if (m_bufferMode)
			m_buffer.insert(m_buffer.end(), static_cast<const BYTE*>(pBuffer), static_cast<const BYTE*>(pBuffer) + size);
		else if (m_file.is_open())
			m_file.write(static_cast<const char*>(pBuffer), size);
		else
			throw(FileWriterException("FileWriter not initialized"));
	}

private:
	template<typename T>
	void write(const T& data)
	{
		m_size += sizeof(T);
		if (m_bufferMode)
			m_buffer.insert(m_buffer.end(), reinterpret_cast<const BYTE*>(&data), reinterpret_cast<const BYTE*>(&data) + sizeof(T));
		else if (m_file.is_open())
			m_file.write(reinterpret_cast<const char*>(&data), sizeof(T));
		else
			throw(FileWriterException("FileWriter not initialized"));
	}

private:
	bool m_bufferMode          = true;
	uint64_t m_size            = 0;
	std::fstream m_file        = {};
	std::vector<BYTE> m_buffer = {};
};