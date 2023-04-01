#include "Component.h"
#include "Entity.h"
#include <iostream>

#include "ConsoleManager.h"
#include "EngineTime.h"

namespace ECS {

	Component::Component() {

		entity = nullptr;
		active = true;
	}

	bool Component::isActive() {
		return active;
	}

	void Component::setActive(bool ent_active) {
		active = ent_active;

		if (active)
			onActive();
		else
			onDeacitve();
	}

	Entity* Component::getEntity() {
		return entity;
	}

	void Component::remove() {
		entity->removeComponent(this);
	}

	void Component::remove(Component* c) {
		entity->removeComponent(c);
	}

	void Component::print(std::string const& message, std::string const& className)
	{
		std::string time = Utilities::EngineTime::Time2String(Utilities::EngineTime::instance()->timeSinceStart);

		Console::Output::Print(entity->name, time, message, className);
	}

	void Component::printError(std::string const& message, std::string const& className)
	{
		std::string time = Utilities::EngineTime::Time2String(Utilities::EngineTime::instance()->timeSinceStart);

		Console::Output::PrintError(className + ": " + entity->name, time, message);
	}

	void Component::printWarning(std::string const& message, std::string const& className)
	{
		std::string time = Utilities::EngineTime::Time2String(Utilities::EngineTime::instance()->timeSinceStart);

		Console::Output::PrintWarning(className + ": " + entity->name, time, message);
	}

	Scene* Component::getScene() {
		return entity->getScene();
	}

}
