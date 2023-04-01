#pragma once

#include <string>

using cstring = std::string const&;

namespace Console {

	class Output {

	public:

		enum class Color {

			Black, DeepBlue, DarkGreen, LightBlue, Red, Purple, Yellow, LightGrey, Grey, Blue, Green, Aqua, LightRed, LightPurple, LightYellow, White
		};


		static void Print(cstring info, cstring message);

		static void PrintError(cstring info, cstring message);

		static void PrintWarning(cstring info,  cstring message);

		static void PrintColor(Output::Color infoColor, Output::Color messageColor, cstring info, cstring message);


	private:

		static void PrintInfo(cstring info);


	};

}

