#include "ComponentManager.h"
#include "ComponentReader.h"

#include <iostream>

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

	void ComponentManager::ReadComponentInfo(cstring path)
	{
		EnsureInitialised();

		auto result = ComponentReader::ReadComponents(path);


		for (auto& cmp : result) {

			instance->components.emplace(cmp.getName(), cmp);
		}
	}

	void ComponentManager::ReadManagerInfo(cstring path)
	{
		EnsureInitialised();

		auto result = ComponentReader::ReadManagers(path);


		for (auto& cmp : result) {

			instance->managers.emplace(cmp.getName(), cmp);
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

}
