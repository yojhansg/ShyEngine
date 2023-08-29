#pragma once

#include <string>
#define WHITESPACE "\n\r\t\f\v "
/*
	Funciones para eliminar espacios de cadenas de texto
*/
namespace Utilities {


	inline std::string static tolower(const std::string& s) {

		std::string str = s;

		for (int i = 0; i < str.length(); i++) {
			str[i] = std::tolower(str[i]);
		}

		return str;
	}

	inline std::string static ltrim(const std::string& s, const std::string& to_trim = WHITESPACE)
	{
		size_t start = s.find_first_not_of(to_trim);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	inline std::string static rtrim(const std::string& s, const std::string& to_trim = WHITESPACE)
	{
		size_t end = s.find_last_not_of(to_trim);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	inline std::string static trim(const std::string& s, const std::string& to_trim = WHITESPACE) {
		return rtrim(ltrim(s, to_trim), to_trim);
	}
}
