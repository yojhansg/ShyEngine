#include "ComponentReader.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

using nlohmann::json;

namespace Components {

	Component ComponentReader::ReadComponent(cstring name, json data)
	{
		Component cmp(name);


		if (data.contains("attributes"))
		{
			auto attributes = data["attributes"].get<std::vector<json>>();

			for (auto& attr : attributes) {

				Attribute attributeInfo(attr["name"].get<std::string>(), attr["type"].get<std::string>()) ;

				cmp.addAttribute(attributeInfo);
			}
		}

		//TODO: Methods

		return cmp;
	}

	std::vector<Component> Components::ComponentReader::ReadComponents(cstring filePath)
	{
		std::ifstream fileStream(filePath);

		if (!fileStream.good() || !json::accept(fileStream))
		{
			return std::vector<Component>();
		}

		fileStream.clear();
		fileStream.seekg(0);

		json file = json::parse(fileStream);

		std::vector<Component> components;
		for (auto& item : file.items()) {
			
			components.push_back(ReadComponent(item.key(), item.value()));
		}

		return components;
	}

}