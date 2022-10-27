#pragma once

#include <fstream>

namespace GameEngineSpace
{
	enum class FileMode
	{
		WRITE,
		READ,
		BOTH
	};

	class FileManager
	{
		static std::shared_ptr<FileManager> instance;

		std::fstream file;

	public:
		FileManager();
		~FileManager();

		bool OpenFile(const std::string& fileName, FileMode mode);
		bool CloseFile();

		bool SetFloat(float data);

		bool GetFloat(float& data);

		static std::shared_ptr<FileManager> GetInstance();
	};
}