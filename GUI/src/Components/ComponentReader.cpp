#include "ComponentReader.h"
#include "nlohmann/json.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "CheckML.h"

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

				cmp.AddAttribute(attributeInfo);
			}
		}



		if (data.contains("methods") && !data["methods"].is_null())
		{
			auto& functions = data["methods"];

			for (auto& function : functions) {

				Function functionInfo(function["name"].get<std::string>(), name);

				if (function.contains("input") && function["input"].is_array()) {

					for (const auto& in : function["input"]) {

						if (!in.is_null())
							functionInfo.AddInput(Variable(in["name"].get<std::string>(), in["type"].get<std::string>()));
					}

				}

				functionInfo.SetReturn(Variable("", function["return"].get<std::string>()));

				cmp.AddFunction(functionInfo);
			}
		}

		return cmp;
	}


	std::vector<Component> ComponentReader::ReadComponents(cstring filePath)
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

			for (auto& function : manager["methods"]) {

				Function functionInfo(function["name"].get<std::string>(), name);

				for (const auto& in : function["input"]) {

					if (!in.is_null())
						functionInfo.AddInput(Variable(in["name"].get<std::string>(), in["type"].get<std::string>()));
				}

				functionInfo.SetReturn(Variable("", function["return"].get<std::string>()));
				comp.AddFunction(functionInfo);
			}

			components.push_back(comp);
		}

		return components;
	}


	std::vector<Script> ComponentReader::ReadScripts(cstring filePath)
	{

		if (!std::filesystem::is_directory(filePath)) {
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


						if (typeString == "Number") {
							type = AttributesType::FLOAT;
							value.value.valueFloat = sv["defaultValue"].get<float>();
						}
						else if (typeString == "Pair") {
							type = AttributesType::VECTOR2;
							sscanf_s(sv["defaultValue"].get<std::string>().c_str(), "%f, %f", &value.value.valueVector2.x, &value.value.valueVector2.y);
						}
						else if (typeString == "Text") {
							type = AttributesType::STRING;
							value.valueString = sv["defaultValue"].get<std::string>();
						}
						else if (typeString == "Toggle") {
							type = AttributesType::BOOL;
							value.value.valueBool = sv["defaultValue"].get<bool>();
						}
						else if (typeString == "Color") {
							type = AttributesType::COLOR;
							sscanf_s(sv["defaultValue"].get<std::string>().c_str(), "%f, %f, %f", &value.value.valueColor.r, &value.value.valueColor.g, &value.value.valueColor.b);
						}
						else if (typeString == "Entity") {
							type = AttributesType::ENTITY;
							value.value.entityIdx = sv["defaultValue"].get<int>();
						}
						else if (typeString == "Letter") {
							type = AttributesType::CHAR;
							value.value.valueChar = sv["defaultValue"].get<char>();;
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