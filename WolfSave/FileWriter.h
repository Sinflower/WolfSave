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

		m_init = true;
	}

	~FileWriter()
	{
		if (m_file.is_open())
			m_file.close();
	}

	template<typename T>
	void Write(const T& data)
	{
		write<T>(data);
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
		if (!m_init)
			throw(FileWriterException("FileWriter not initialized"));

		m_size += size;
		m_file.write(static_cast<const char*>(pBuffer), size);
	}

private:
	template<typename T>
	void write(const T& data)
	{
		if (!m_init)
			throw(FileWriterException("FileWriter not initialized"));

		m_size += sizeof(T);
		m_file.write(reinterpret_cast<const char*>(&data), sizeof(T));
	}

private:
	bool m_init         = false;
	uint64_t m_size     = 0;
	std::fstream m_file = {};
};