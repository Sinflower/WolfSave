#pragma once
#include <windows.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Utils.h"

using QWORD = uint64_t;

class FileWalkerException : public std::exception
{
public:
	explicit FileWalkerException(const std::string& what) :
		m_what(what) {}

	explicit FileWalkerException(const std::wstring& what) :
		m_what(ws2s(what)) {}

	virtual ~FileWalkerException() noexcept {}

	virtual const char* what() const throw()
	{
		return m_what.c_str();
	}

private:
	std::string m_what;
};

class FileWalker
{
public:
	FileWalker() {}
	FileWalker(const std::string& filename, const DWORD& startOffset = -1) :
		FileWalker(s2ws(filename), startOffset) { }

	FileWalker(const std::wstring& filename, const DWORD& startOffset = -1)
	{
		Open(filename, startOffset);
	}

	FileWalker(const std::vector<BYTE>& dataVec)
	{
		InitData(dataVec);
	}

	// Disable copy constructor and copy assignment operator
	FileWalker(const FileWalker&) = delete;
	FileWalker& operator=(const FileWalker&) = delete;

	void InitData(const std::vector<BYTE>& dataVec)
	{
		m_dataVec = dataVec;
		m_pData = m_dataVec.data();
		m_size = static_cast<DWORD>(m_dataVec.size());
		m_init = true;
	}

	void Open(const std::wstring& filename, const DWORD& startOffset = -1)
	{
		m_pFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_pFile == nullptr)
			throw(FileWalkerException(L"Failed to open file: " + filename));

		m_pFileMap = CreateFileMappingW(m_pFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (m_pFileMap == nullptr)
		{
			CloseHandle(m_pFile);
			throw(FileWalkerException(L"Failed to create file mapping for: " + filename));
		}

		m_pMapView = MapViewOfFile(m_pFileMap, FILE_MAP_READ, 0, 0, 0);
		if (m_pMapView == nullptr)
		{
			CloseHandle(m_pFileMap);
			CloseHandle(m_pFile);
			throw(FileWalkerException(L"Failed to create map view of file: " + filename));
		}

		m_pData = reinterpret_cast<PBYTE>(m_pMapView);

		m_size = GetFileSize(m_pFile, 0);

		if (startOffset != -1)
			m_offset = startOffset;

		m_init = true;
	}

	~FileWalker()
	{
		if (m_pMapView != nullptr)
			UnmapViewOfFile(m_pMapView);

		if (m_pFileMap != nullptr)
			CloseHandle(m_pFileMap);

		if (m_pFile != nullptr)
			CloseHandle(m_pFile);
	}

	QWORD ReadQWord()
	{
		return read<QWORD>();
	}

	DWORD ReadDWord()
	{
		return read<DWORD>();
	}

	WORD ReadWord()
	{
		return read<WORD>();
	}

	BYTE ReadByte()
	{
		return read<BYTE>();
	}

	template<std::size_t S>
	void ReadBytesArr(std::array<BYTE, S>& buffer, const DWORD& size = -1)
	{
		if (size == -1)
			ReadBytes(buffer.data(), static_cast<DWORD>(buffer.size()));
		else if (size <= buffer.size())
			ReadBytes(buffer.data(), size);
		else
			throw(FileWalkerException("ReadBytesArr: size is larger than buffer size"));
	}

	void ReadBytesVec(std::vector<BYTE>& buffer, const DWORD& size = -1)
	{
		if (size == -1)
			ReadBytes(buffer.data(), static_cast<DWORD>(buffer.size()));
		else if (size <= buffer.size())
			ReadBytes(buffer.data(), size);
		else
			throw(FileWalkerException("ReadBytesVec: size is larger than buffer size"));
	}

	void ReadBytes(LPVOID pBuffer, std::size_t& size)
	{
		ReadBytes(pBuffer, static_cast<DWORD>(size));
	}

	void ReadBytes(LPVOID pBuffer, const DWORD& size)
	{
		if (!m_init)
			throw(FileWalkerException("FileWalker not initialized"));

		if (m_offset + size > m_size)
			throw(FileWalkerException("ReadBytes: Attempted to read past end of file"));

		std::memcpy(pBuffer, m_pData + m_offset, size);
		m_offset += size;
	}

	template<typename T>
	void ReadVec(std::vector<T>& buffer, const DWORD& dwordCnt = -1)
	{
		if (dwordCnt == -1)
			ReadBytes(buffer.data(), buffer.size() * sizeof(T));
		else if (dwordCnt <= buffer.size())
			ReadBytes(buffer.data(), dwordCnt * sizeof(T));
		else
			throw(FileWalkerException("ReadVec: size is larger than buffer size"));

	}

	void Seek(const DWORD& offset)
	{
		if (!m_init)
			throw(FileWalkerException("FileWalker not initialized"));

		if (offset > m_size)
			throw(FileWalkerException("Seek: Attempted to seek past end of file"));

		m_offset = offset;
	}

	const PBYTE Get() const
	{
		return m_pData + m_offset;
	}

	DWORD GetOffset()
	{
		return m_offset;
	}

	DWORD GetSize()
	{
		return m_size;
	}

	bool IsEndOfFile()
	{
		return m_offset >= m_size;
	}

	BYTE At(const DWORD& offset)
	{
		if (!m_init)
			throw(FileWalkerException("FileWalker not initialized"));

		if (offset >= m_size)
			throw(FileWalkerException("At: Attempted to read past end of file"));

		return *(m_pData + offset);
	}

private:
	template<typename T>
	T read()
	{
		if (!m_init)
			throw(FileWalkerException("FileWalker not initialized"));

		if (m_offset + sizeof(T) > m_size)
			throw(FileWalkerException("read: End of file reached"));

		T value = *(reinterpret_cast<T*>(m_pData + m_offset));
		m_offset += sizeof(T);
		return value;
	}

private:
	bool m_init = false;
	HANDLE m_pFile = nullptr;
	LPVOID m_pMapView = nullptr;
	HANDLE m_pFileMap = nullptr;

	PBYTE m_pData = nullptr;

	DWORD m_offset = 0;
	DWORD m_size = 0;

	std::vector<BYTE> m_dataVec = {};
};
