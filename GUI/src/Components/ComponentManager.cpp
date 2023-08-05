#include "ComponentManager.h"
#include "ComponentReader.h"

#include <iostream>

Components::ComponentManager* Components::ComponentManager::instance = nullptr;

void Components::ComponentManager::EnsureInitialised()
{
	if (instance == nullptr)
		Initialise();

}

void Components::ComponentManager::Initialise()
{
	instance = new ComponentManager();
}

void Components::ComponentManager::Release()
{
	delete instance;
	instance = nullptr;
}

void Components::ComponentManager::ReadComponentInfo(cstring path)
{
	EnsureInitialised();

	auto result = ComponentReader::ReadComponents(path);


	for (auto& cmp : result) {


		instance->components.emplace(cmp.getName(), cmp);
	}
}


std::unordered_map<std::string, Components::Component> const& Components::ComponentManager::GetAllComponents() {
	return instance->components;
}
