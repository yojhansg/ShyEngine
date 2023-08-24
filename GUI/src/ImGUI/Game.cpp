#include "Game.h"

#include "Preferences.h"
#include "LogManager.h"
#include "Console.h"
#include "Editor.h"

#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <io.h>

#include "CheckML.h"

namespace ShyEditor {

	Game* Game::instance = nullptr;

	Game::Game(const std::string& path, const std::string& releasePath) : path(path), releasePath(releasePath)
	{
		isRunning = false;
		instance = this;
		game = NULL;
		gameEnded = false;
	}

	Game::~Game()
	{
		if (isRunning)
			Stop();
		instance = nullptr;
	}

	void Game::Init(const std::string& path, const std::string& releasePath)
	{
		Release();

		instance = new Game(path, releasePath);
	}

	void Game::Release()
	{
		if (instance != nullptr)
		{
			Stop();
			delete instance;
			instance = nullptr;
		}
	}

	void Game::Play(bool debug)
	{
		if (!IsRunning())
			instance->play(debug);
	}

	void Game::Stop()
	{
		if (IsRunning())
			instance->stop();
	}

	void Game::CheckEnd()
	{
		if (instance->gameEnded && instance->gameThread.joinable()) {

			instance->gameThread.join();
			instance->gameEnded = false;
			instance->isRunning = false;
		}
	}

	bool Game::IsRunning()
	{
		return instance->isRunning;
	}

	bool Game::PendingOutput()
	{
		return !instance->output.empty();
	}

	std::string Game::FlushOutput()
	{
		std::string str;

		if (instance->output.try_pop(str))
		{
			return str;
		}

		return "";
	}

	void Game::play(bool debug)
	{
		Editor::getInstance()->getConsole()->Show();

		output.clear();

		HANDLE hChildStdoutRead, hChildStdoutWrite;
		SECURITY_ATTRIBUTES saAttr;

		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		if (!CreatePipe(&hChildStdoutRead, &hChildStdoutWrite, &saAttr, 0)) {
			LogManager::LogError("Error creating pipe for the debug console.");
			return;
		}

		STARTUPINFOA si;

		ZeroMemory(&si, sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.hStdError = hChildStdoutWrite;
		si.hStdOutput = hChildStdoutWrite;
		si.dwFlags |= STARTF_USESTDHANDLES;

		std::string newProcessWorkingDirectory = "";
		if (debug) newProcessWorkingDirectory = "Engine";
		else newProcessWorkingDirectory = Preferences::GetData().buildPath.c_str();

		if (!CreateProcessA(NULL, (LPSTR)(debug ? path : releasePath).c_str(), NULL, NULL, TRUE, 0, NULL, newProcessWorkingDirectory.c_str(), &si, &pi)) {
			LogManager::LogError("Error creating the process to execute the engine.");
			return;
		}

		CloseHandle(hChildStdoutWrite);

		game = pi.hProcess;

		gameThread = std::thread(&Game::readOutput, this, hChildStdoutRead, hChildStdoutWrite);

		//gameThread.detach();

		isRunning = true;
	}

	void Game::stop()
	{
		// Forzar la terminación del proceso
		auto ret = TerminateProcess(game, 1);
		game = NULL;

		gameThread.join();

		isRunning = false;
	}

	void Game::readOutput(HANDLE hChildStdoutRead, HANDLE hChildStdoutWrite)
	{
		char buffer[4096];
		DWORD bytesRead;

		while (true) {

			if (!ReadFile(hChildStdoutRead, buffer, sizeof(buffer), &bytesRead, NULL) || bytesRead == 0) {
				DWORD errorCode = GetLastError();
				LogManager::LogError("ReadFile() failed with error code: " + errorCode);
				break;
			}

			std::string output(buffer, bytesRead);
			this->output.push(output);

		}

		CloseHandle(hChildStdoutRead);

		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		game = NULL;
		gameEnded = true;
	}

}
