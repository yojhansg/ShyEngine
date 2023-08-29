#pragma once

#include <string>
#include <Windows.h>
#include <thread>
#include <concurrent_queue.h>

namespace ShyEditor {

	class Game {

	private:

		static Game* instance;

		bool isRunning;
		bool gameEnded;

		HANDLE game;
		PROCESS_INFORMATION pi;

		std::string path;
		std::string releasePath;

		Concurrency::concurrent_queue<std::string> output;

		std::thread gameThread;

		void play(bool debug);
		void stop();

		void readOutput(HANDLE read, HANDLE write);

	public:

		Game(const std::string& path, const std::string& releasePath);
		~Game();

		static void Init(const std::string& path, const std::string& releasePath);
		static void Release();

		static void Play(bool debug);
		static void Stop();
		static void CheckEnd();
		static bool IsRunning();

		static bool PendingOutput();
		static std::string FlushOutput();

	};


}
