#include "ComponentManager.h"
#include "ComponentReader.h"

static Components::ComponentManager* instance = nullptr;

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
}

void Components::ComponentManager::ReadComponentInfo(cstring components, cstring functions, cstring attributes)
{



}
