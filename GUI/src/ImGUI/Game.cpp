#include "Game.h"
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <io.h>
#include "Editor.h"
#include "Console.h"

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
			std::cerr << "Error creating pipe." << std::endl;
			return;
		}

		STARTUPINFOA si;

		ZeroMemory(&si, sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.hStdError = hChildStdoutWrite;
		si.hStdOutput = hChildStdoutWrite;
		si.dwFlags |= STARTF_USESTDHANDLES;

		// Replace "your_command_here" with the actual command you want to execute
		if (!CreateProcessA(NULL, (LPSTR)(debug ? path : releasePath).c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
			std::cerr << "Error creating process." << std::endl;
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
				break;
			}

			std::string output(buffer, bytesRead);
			this->output.push(output);

			//std::cout << output << "jeje" << std::endl;
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
