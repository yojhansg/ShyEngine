#pragma once

#include <string>
#include "ComponentInfo.h"
#include <nlohmann/json_fwd.hpp>



namespace Components {

	using nlohmann::json;

	using cstring = std::string const&;

	class ComponentReader {


		static Component ReadComponent(cstring name, json data);

	public:


		static std::vector<Component> ReadComponents(cstring filePath);
		static std::vector<Component> ReadManagers(cstring filePath);
	};
	

}
