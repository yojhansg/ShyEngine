#pragma once

#include <string>
#include <fstream>

namespace ShyEditor {

	class LogManager {

	public:

		static LogManager* GetInstance();

		static bool Init();
		static void Release();

		static void LogError(const std::string& msg);
		static void LogWarning(const std::string& msg);

	private:

		static LogManager* instance;

		std::ofstream logFile;

		LogManager();
		~LogManager();

	};

}
