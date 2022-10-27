#include "pch.h"
#include "FileManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<FileManager> FileManager::instance = nullptr;

	FileManager::FileManager()
	{

	}

	FileManager::~FileManager()
	{
		if (file.is_open() == true)
			file.close();
	}

	bool FileManager::OpenFile(const std::string& fileName, FileMode mode)
	{
		switch (mode)
		{

		case FileMode::WRITE:
		{
			file.open(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

			break;
		}
		
		case FileMode::READ:
		{
			file.open(fileName.c_str(), std::ios::in | std::ios::binary);

			break;
		}
		
		case FileMode::BOTH:
		{
			file.open(fileName.c_str(), std::ios::in | std::ios::out | std::ios::binary);

			break;
		}

		}

		return file.is_open();
	}

	bool FileManager::CloseFile()
	{
		if (file.is_open() == true)
		{
			file.close();

			return true;
		}

		return false;
	}

	bool FileManager::SetFloat(float data)
	{
		if (file.is_open() != true)
			return false;

		file << data;

		return true;
	}

	bool FileManager::GetFloat(float& data)
	{
		if (file.is_open() != true)
			return false;

		if (file.eof() == true)
			return false;

		file >> data;

		return true;
	}

	std::shared_ptr<FileManager> FileManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<FileManager>();

		return instance;
	}
}