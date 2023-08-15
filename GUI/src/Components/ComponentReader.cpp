#include "ComponentReader.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <filesystem>

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

				Attribute attributeInfo(attr["name"].get<std::string>(), attr["type"].get<std::string>());

				cmp.addAttribute(attributeInfo);
			}
		}

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

				methodInfo.SetReturn(Variable("", method["return"].get<std::string>()));

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

		fileStream.close();
		std::vector<Component> components;
		for (auto& item : file.items()) {

			components.push_back(ReadComponent(item.key(), item.value()));
		}


		return components;
	}





	std::vector<Component> ComponentReader::ReadManagers(cstring filePath)
	{
		std::ifstream fileStream(filePath);

		if (!fileStream.good() || !json::accept(fileStream))
		{
			return std::vector<Component>();
		}

		fileStream.clear();
		fileStream.seekg(0);

		json file = json::parse(fileStream);
		fileStream.close();

		std::vector<Component> components;
		for (auto& manager : file) {

			std::string name = manager["name"].get<std::string>();
			Component comp(name);

			for (auto& method : manager["methods"]) {

				Method methodInfo(method["name"].get<std::string>(), name);

				for (const auto& in : method["input"]) {

					if (!in.is_null())
						methodInfo.AddInput(Variable(in["name"].get<std::string>(), in["type"].get<std::string>()));
				}

				methodInfo.SetReturn(Variable("", method["return"].get<std::string>()));
				comp.addMethod(methodInfo);
			}

			components.push_back(comp);
		}

		return components;
	}

	std::vector<Script> ComponentReader::ReadScripts(cstring filePath)
	{

		if (!std::filesystem::is_directory(filePath)) {

			std::cout << "Ups" << std::endl;

			return std::vector<Script>();
		}


		auto scripts = std::vector<Script>();
		for (const auto& entry : std::filesystem::directory_iterator(filePath)) {

			if (entry.is_directory()) continue; //TODO: leer en carpetas anidadas

			if (entry.path().extension() != ".script") continue;

			Script script = Script(entry.path().filename().stem().string());


			std::ifstream fileStream(entry.path());

			if (fileStream.good() && json::accept(fileStream))
			{
				fileStream.clear();
				fileStream.seekg(0);

				json root = json::parse(fileStream);
				fileStream.close();


				if (root.contains("serializedValues"))
				{

					for (auto& sv : root["serializedValues"])
					{
						std::string name = sv["name"].get<std::string>();
						std::string typeString = sv["type"].get<std::string>();


						AttributesType type = AttributesType::FLOAT;
						AttributeValue value;


						if (typeString == "int" || typeString == "float") {
							type = AttributesType::FLOAT;
							value.value.valueFloat = sv["defaultValue"].get<float>();
						}
						else if (typeString == "Utilities::Vector2D") {
							type = AttributesType::VECTOR2;
							//value.value.valueFloat = sv["value"].get<float>();
						}
						else if (typeString == "std::string" || typeString == "string") {
							type = AttributesType::STRING;
							value.valueString = sv["defaultValue"].get<std::string>();
						}
						else if (typeString == "bool") {
							type = AttributesType::BOOL;
							value.value.valueBool = sv["defaultValue"].get<bool>();
						}
						else if (typeString == "Utilities::Color") {
							value.value.valueColor = { 0.0f, 0.0f, 0.0f };
							type = AttributesType::COLOR;
						}
						else {
							type = AttributesType::NONE;
						}



						Attribute attribute(name);

						attribute.SetType(typeString, type);
						attribute.SetValue(value);

						script.AddAttribute(name, attribute);
					}


				}

			}


			scripts.push_back(script);

		}

		return scripts;
	}

}