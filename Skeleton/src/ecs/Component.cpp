#include "Component.h"
#include "Entity.h"
#include <iostream>

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

	Scene* Component::getScene() {
		return entity->getScene();
	}

}
