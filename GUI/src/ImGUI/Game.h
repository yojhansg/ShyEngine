#pragma once

#include <string>
#include <Windows.h>
#include <thread>

namespace PEditor
{

	class Game {

	private:

		static Game* instance;

		bool isRunning;
		HANDLE game;
		PROCESS_INFORMATION pi;

		std::string path;
		std::string output;

		std::thread gameThread;

		void play();
		void stop();

		void readOutput(HANDLE read, HANDLE write);

	public:

		Game(const std::string& path);
		~Game();

		static void Init(const std::string& path);
		static void Release();

		static void Play();
		static void Stop();
		static bool IsRunning();

		static std::string GetOutput();

	};


}
