#include "Save.h"

#include "ConsoleManager.h"
#include <json.hpp>
#include <fstream>
#include "Entity.h"
#include <vector>
#include <filesystem>
#include <iostream>
using namespace nlohmann;

using jsonarray = std::vector<json>;

ECS::SaveManager::SaveManager()
{
	std::filesystem::create_directory(std::filesystem::path("Saves"));


	Load(0);
	currentLoaded = -1;
}

ECS::SaveManager::~SaveManager()
{
	Save(0);
}


void ECS::SaveManager::SaveAll()
{
	Save(0);

	if (currentLoaded > 0)
		Save(currentLoaded);
}

void ECS::SaveManager::Save(int slot)
{
	if (slot < 0) {

		Console::Output::PrintError("Save", "Cannot load a slot smaller than 0");
		return;
	}

	std::map<std::string, Scripting::Variable>& map = slot == 0 ? implicit : loaded;

	json root;

	map["currentEngineDay"] = Scripting::Variable::Int(std::time(NULL) / (1440) - 1166934);


	int idx = 0;
	for (auto& elem : map) {

		root[idx]["name"] = elem.first;
		root[idx]["type"] = elem.second.Type2String();

		switch (elem.second.type) {

		case Scripting::Variable::Type::Bool:
			root[idx]["value"] = elem.second.value.Bool;
			break;
		case Scripting::Variable::Type::Float:
			root[idx]["value"] = elem.second.value.Float;
			break;
		case Scripting::Variable::Type::Char:
			root[idx]["value"] = elem.second.value.Char;
			break;
		case Scripting::Variable::Type::String:
			root[idx]["value"] = elem.second.str;
			break;
		case Scripting::Variable::Type::Entity:
			root[idx]["value"] = elem.second.value.entity->getEntityName();
			break;
		case Scripting::Variable::Type::Null:
			break;
		case Scripting::Variable::Type::Vector2D:
			root[idx]["value"] = (std::string)elem.second.vector;
			break;
		default:
			break;
		}
		idx++;
	}


	std::ofstream fileStream(GetSlotPath(slot));

	if (!fileStream.good())
	{
		Console::Output::PrintError("Save", std::format("Cannot create save slot <{}>", slot));
		fileStream.close();
		return;
	}

	fileStream << root.dump(4) << std::endl;

	currentLoaded = slot;

	fileStream.close();
}

void ECS::SaveManager::Load(int slot)
{
	if (slot < 0) {

		Console::Output::PrintError("Save", "Cannot load a slot smaller than 0");
		return;
	}

	currentLoaded = slot;

	std::map<std::string, Scripting::Variable>& map = slot == 0 ? implicit : loaded;
	map.clear();

	std::ifstream fileStream(GetSlotPath());

	if (!fileStream.good())
	{
		if (slot != 0)
			Console::Output::PrintError("Save", std::format("Missing save slot <{}>", slot));
		fileStream.close();
		return;
	}

	jsonarray file = json::parse(fileStream).get<jsonarray>();

	for (auto& element : file) {

		Scripting::Variable value;

		std::string type = element["type"];

		//TODO: get entity
		if (type == "float") {
			value = element["value"].get<float>();
		}
		else if (type == "string") {
			value = element["value"].get<std::string>();
		}
		else if (type == "bool") {
			value = element["value"].get<bool>();
		}
		else if (type == "char") {
			value = element["value"].get<char>();
		}
		else if (type == "Vector2D") {
			value = (Utilities::Vector2D) element["value"].get<std::string>();
		}

		map[element["name"].get<std::string>()] = value;
	}


	fileStream.close();
}

void ECS::SaveManager::SetSlot(std::string name, Scripting::Variable value)
{
	if (currentLoaded == 0) {
		Set(name, value);
		return;
	}

	loaded[name] = value;
}

Scripting::Variable ECS::SaveManager::GetSlot(std::string name)
{
	if (currentLoaded == 0)
		return Get(name);

	if (loaded.contains(name))
		return loaded[name];

	return Scripting::Variable::Null();
}

void ECS::SaveManager::Set(std::string name, Scripting::Variable value)
{
	implicit[name] = value;
}


Scripting::Variable ECS::SaveManager::Get(std::string name)
{
	if (implicit.contains(name))
		return implicit[name];

	return Scripting::Variable::Null();
}

std::string ECS::SaveManager::GetSlotPath()
{
	return path + std::to_string(currentLoaded) + extension;
}

std::string ECS::SaveManager::GetSlotPath(int slot)
{
	return path + std::to_string(slot) + extension;
}

void ECS::SaveManager::ClearSlot()
{
	currentLoaded = 0;
	loaded.clear();
}

void ECS::SaveManager::DeleteSave(int slot)
{
	std::remove(GetSlotPath(slot).c_str());
}

bool ECS::SaveManager::Exists(int slot)
{
	std::ifstream stream(GetSlotPath(slot));

	bool good = stream.good();

	stream.close();

	return good;
}