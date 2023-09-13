#pragma once

#include <string>

using cstring = std::string const&;

namespace Console {

	enum class Color {

		Black, DeepBlue, DarkGreen, LightBlue, Red, Purple, Yellow, LightGrey, Grey, Blue, Green, Aqua, LightRed, LightPurple, LightYellow, White
	};

	class Output {

	public:


		static void Print(cstring info, cstring message);

		static void PrintError(cstring info, cstring message);

		static void PrintWarning(cstring info, cstring message);

		static void PrintColor(Color infoColor, Color messageColor, cstring info, cstring message);


		static void PrintNoFormat(cstring message, Color color = Color::White);

	private:

		static void PrintInfo(cstring info);


	};

}

