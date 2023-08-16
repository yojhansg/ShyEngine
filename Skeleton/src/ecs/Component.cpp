#include "Component.h"
#include "Entity.h"

#include "ConsoleManager.h"

namespace ECS {

	Component::Component() {

		entity = nullptr;
		active = true;
		removed = false;
		inRemovedComponentList = false;
		valid = true;
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
		removed = true;

		entity->hasRemovedComponents = true;
	}

	void Component::remove(Component* c) {
		c->removed = true;

		c->entity->hasRemovedComponents = true;
	}

	bool Component::isRemoved() {
		return removed;
	}

	void Component::print(std::string const& message, std::string const& className)
	{
		Console::Output::Print(className + ": " + entity->name, message);
	}

	void Component::printError(std::string const& message, std::string const& className)
	{
		Console::Output::PrintError(className + ": " + entity->name, message);
	}

	void Component::printWarning(std::string const& message, std::string const& className)
	{
		Console::Output::PrintWarning(className + ": " + entity->name, message);
	}

	Scene* Component::getScene() {
		return entity->getScene();
	}

}
