#include "Entity.h"
#include "Component.h"
#include "Scene.h"

namespace ECS {

	Entity::Entity(const std::string& ent_name) {

		name = ent_name;
		active = true;
		removed = false;
	}

	void Entity::init() {
		for (auto c : components) {
			c->init();
		}
	}

	void Entity::udpate() {

	}

	void Entity::fixedUpdate() {

	}

	void Entity::setActive(bool ent_active) {
		active = ent_active;
	}

	bool Entity::isActive() {
		return active;
	}

	void Entity::removeEntity() {
		removed = true;
	}

}
