#include "ComponentManager.h"
#include "ComponentReader.h"

#include <iostream>

#include <fstream>
#include "nlohmann/json.hpp"

#include "CheckML.h"


namespace Components {

	ComponentManager* ComponentManager::instance = nullptr;

	std::string ComponentManager::scriptPath = "";

	void ComponentManager::EnsureInitialised()
	{
		if (instance == nullptr)
			Initialise();
	}

	void ComponentManager::Initialise()
	{
		instance = new ComponentManager();
	}

	void ComponentManager::Release()
	{
		delete instance;
		instance = nullptr;
	}

	void ComponentManager::ReadComponents(cstring path)
	{
		EnsureInitialised();

		auto result = ComponentReader::ReadComponents(path);


		for (auto& cmp : result) {

			instance->components.emplace(cmp.GetName(), cmp);
		}
	}

	void ComponentManager::ReadManagers(cstring path)
	{
		EnsureInitialised();

		auto result = ComponentReader::ReadManagers(path);


		for (auto& cmp : result) {

			instance->managers.emplace(cmp.GetName(), cmp);
		}

	}


	void ComponentManager::ReadScripts(cstring path)
	{
		EnsureInitialised();

		scriptPath = path;

		instance->scripts = ComponentReader::ReadScripts(path);

	}

	void ComponentManager::ReloadScripts()
	{
		ReadScripts(scriptPath);
	}


	std::unordered_map<std::string, Component>& ComponentManager::GetAllComponents() {
		return instance->components;
	}

	std::unordered_map<std::string, Component>& ComponentManager::GetAllManagers()
	{
		return instance->managers;
	}

	std::vector<Script>& ComponentManager::GetAllScripts()
	{
		return instance->scripts;
	}


	void ComponentManager::ReadInfo(cstring path)
	{
		EnsureInitialised();

		std::ifstream fileStream(path);

		if (!fileStream.good() || !json::accept(fileStream))
		{
			return;
		}

		fileStream.clear();
		fileStream.seekg(0);

		json file = json::parse(fileStream);
		fileStream.close();

		for (auto& component : file.items()) {

			bool isEnum = component.key() == "enums";

			for (auto& attr : component.value().items()) {

				std::vector<std::string> enums;
				for (auto& info : attr.value().items()) {

					if (isEnum)
						enums.push_back(info.value());
					else
						instance->info[component.key() + "_" + attr.key() + "_" + info.key()] = info.value().get<std::string>();
				}

				if (isEnum)
					instance->enums[attr.key()] = enums;
			}
		}
	}

	bool ComponentManager::GetComponentInfo(std::string& value, cstring component)
	{
		std::string key = component + "_info_info";
		if (instance->info.contains(key)) {

			value = instance->info[key];
			return true;
		}

		return false;
	}

	bool ComponentManager::GetComponentElementInfo(std::string* value, cstring component, cstring element, cstring info)
	{
		std::string key = component + "_" + element + "_" + info;
		if (instance->info.contains(key)) {
			if (value != NULL)
				*value = instance->info[key];
			return true;
		}

		return false;
	}

	const std::vector<std::string>& ComponentManager::GetEnum(cstring name)
	{
		return instance->enums[name];
	}

	std::vector<std::string> ComponentManager::GetEnumNames()
	{
		auto v = std::vector<std::string>();

		for (auto& it : instance->enums) {

			v.push_back(it.first);
		}

		return v;
	}



}
