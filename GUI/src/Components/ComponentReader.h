#pragma once

#include <string>
#include <map>
#include <vector>


namespace Components {

	using cstring = std::string const&;

	class ComponentReader {

	public:

		static std::vector<std::string> ReadComponents(cstring path);
		static void ReadFunctions(cstring path);
		static void ReadAttributes(cstring path);
	};
}
