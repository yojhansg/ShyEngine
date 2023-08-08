#include "ComponentReader.h"
#include "nlohmann/json.hpp"

#include <fstream>

using nlohmann::json;

namespace Components {

	Component ComponentReader::ReadComponent(cstring name, json data)
	{
		Component cmp(name);


		if (data.contains("attributes"))
		{
			auto attributes = data["attributes"].get<std::vector<json>>();

			for (auto& attr : attributes) {

				Attribute attributeInfo(attr["name"].get<std::string>(), attr["type"].get<std::string>());

				cmp.addAttribute(attributeInfo);
			}
		}


		//TODO: hacer un fichero para leer los metodos de los managers

		if (data.contains("methods") && !data["methods"].is_null())
		{
			auto& methods = data["methods"];

			for (auto& method : methods) {

				Method methodInfo(method["name"].get<std::string>(), name);

				if (method.contains("input") && method["input"].is_array()) {


					for (const auto& in : method["input"]) {

						if (!in.is_null())
							methodInfo.AddInput(Variable(in["name"].get<std::string>(), in["type"].get<std::string>()));
					}

				}

				cmp.addMethod(methodInfo);
			}
		}

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