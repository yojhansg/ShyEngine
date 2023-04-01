#include "ConsoleManager.h"
#include <iostream>
#include <Windows.h>
#include "EngineTime.h"


//TODO, no inicializar todo el rato el hconsole sino guardarlo en la clase que sera un singleton en algun momento
//TODO: mandar todos los logs a un fichero

void Console::Output::Print(cstring info, cstring message)
{
	PrintColor(Color::LightBlue, Color::White, info, message);
}

void Console::Output::PrintError(cstring info, cstring message)
{
	PrintColor(Color::White, Color::White, "ERROR: " + info, message);
}

void Console::Output::PrintWarning(cstring info, cstring message)
{
	PrintColor(Color::White, Color::White, "WARNING: " + info, message);
}


void Console::Output::PrintColor(Color infoColor, Color messageColor, cstring info, cstring message)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)infoColor);

	PrintInfo(info);

	SetConsoleTextAttribute(hConsole, (int)messageColor);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(hConsole, (int)Color::White);
}


void Console::Output::PrintInfo(cstring info)
{
	std::string time = Utilities::EngineTime::Time2String(Utilities::EngineTime::instance()->timeSinceStart);

	std::cout << "[" << time << "] ";

	if (info.size() > 0)
		std::cout << info << ": ";
}


void Console::Output::PrintNoFormat(cstring message, Color color)
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, (int)color);

	std::cout << message << std::endl;

	SetConsoleTextAttribute(hConsole, (int)Color::White);

}