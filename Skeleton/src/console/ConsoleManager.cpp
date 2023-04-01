#include "ConsoleManager.h"
#include <iostream>
#include <Windows.h>

//TODO, no inicializar todo el rato el hconsole sino guardarlo en la clase que sera un singleton en algun momento
//TODO: mandar todos los logs a un fichero

void Console::Output::Print(cstring file, cstring time, cstring message, cstring title)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)Color::White);

	PrintInfo(file, time, title);

	SetConsoleTextAttribute(hConsole, (int)Color::White);

	std::cout << message << std::endl;
}

void Console::Output::PrintError(cstring file, cstring time, cstring message)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)Color::LightRed);

	PrintInfo(file, time, "ERROR");

	SetConsoleTextAttribute(hConsole, (int)Color::White);

	std::cout << message << std::endl;
}

void Console::Output::PrintWarning(cstring file, cstring time, cstring message)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)Color::Yellow);

	PrintInfo(file, time, "WARNING");

	SetConsoleTextAttribute(hConsole, (int)Color::White);

	std::cout << message << std::endl;
}


void Console::Output::PrintColor(Output::Color infoColor, Output::Color messageColor, cstring file, cstring time, cstring message, cstring title)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)infoColor);

	PrintInfo(file, time, "WARNING");

	SetConsoleTextAttribute(hConsole, (int)messageColor);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(hConsole, (int)Color::White);
}

void Console::Output::PrintInfo(cstring file, cstring time, cstring title)
{
	std::cout << "[" << time << "] ";

	if (title.size() > 0)
		std::cout << title << ": ";

	std::cout << file.substr(file.find_last_of("\\") + 1) << ": ";
}
