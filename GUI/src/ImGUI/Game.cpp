#include "Game.h"
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <io.h>

PEditor::Game* PEditor::Game::instance = nullptr;


PEditor::Game::Game(const std::string& path) : path(path)
{
	isRunning = false;
	instance = this;
	game = NULL;
	gameEnded = false;
}

PEditor::Game::~Game()
{
	if (isRunning)
		Stop();
	instance = nullptr;
}

void PEditor::Game::Init(const std::string& path)
{
	Release();
	instance = new Game(path);
}

void PEditor::Game::Release()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void PEditor::Game::Play()
{
	if (!IsRunning())
		instance->play();
}

void PEditor::Game::Stop()
{
	if (IsRunning())
		instance->stop();
}

void PEditor::Game::CheckEnd()
{
	if (instance->gameEnded && instance->gameThread.joinable()) {

		instance->gameThread.join();
		instance->gameEnded = false;
		instance->isRunning = false;
	}
}

bool PEditor::Game::IsRunning()
{
	return instance->isRunning;
}

bool PEditor::Game::PendingOutput()
{
	return !instance->output.empty();
}

std::string PEditor::Game::FlushOutput()
{
	std::string str;

	if (instance->output.try_pop(str))
	{
		return str;
	}

	return "";
}



void PEditor::Game::play()
{
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
	if (!CreateProcessA(NULL, (LPSTR)path.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		std::cerr << "Error creating process." << std::endl;
		return;
	}

	CloseHandle(hChildStdoutWrite);

	game = pi.hProcess;

	gameThread = std::thread(&Game::readOutput, this, hChildStdoutRead, hChildStdoutWrite);

	//gameThread.detach();

	isRunning = true;
}

void PEditor::Game::stop()
{
	// Forzar la terminación del proceso
	auto ret = TerminateProcess(game, 1);
	game = NULL;

	gameThread.join();

	isRunning = false;
}

void PEditor::Game::readOutput(HANDLE hChildStdoutRead, HANDLE hChildStdoutWrite)
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
