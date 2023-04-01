#pragma once

#include <string>

using cstring = std::string const&;

namespace Console {

	class Output {

	public:

		enum class Color {

			Black, DeepBlue, DarkGreen, LightBlue, Red, Purple, Yellow, LightGrey, Grey, Blue, Green, Aqua, LightRed, LightPurple, LightYellow, White
		};


		static void Print(cstring file, cstring time, cstring message, cstring title = "");

		static void PrintError(cstring file, cstring time, cstring message);

		static void PrintWarning(cstring file, cstring time, cstring message);

		static void PrintColor(Output::Color infoColor, Output::Color messageColor, cstring file, cstring time, cstring message, cstring title = "");


	private:

		static void PrintInfo(cstring file, cstring time, cstring title);


	};

}

