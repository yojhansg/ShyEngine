#pragma once

#include <string>
#include <Windows.h>
#include <thread>
#include <concurrent_queue.h>

namespace PEditor
{

	class Game {

	private:

		static Game* instance;

		bool isRunning;
		bool gameEnded;

		HANDLE game;
		PROCESS_INFORMATION pi;

		std::string path;
		Concurrency::concurrent_queue<std::string> output;

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
		static void CheckEnd();
		static bool IsRunning();

		static bool PendingOutput();
		static std::string FlushOutput();

	};


}
