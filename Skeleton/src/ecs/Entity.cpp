#include "Entity.h"
#include "Component.h"
#include "Scene.h"

namespace ECS {

	Entity::Entity(const std::string& ent_name) {

		name = ent_name;
		active = true;
		removed = false;
		scene = nullptr;
	}

	Entity::Entity(const std::string& ent_name, Scene* ent_scene) {

		name = ent_name;
		active = true;
		removed = false;
		scene = ent_scene;
	}

	Entity::~Entity() {
		for (auto c : components) {
			delete c; c = nullptr;
		}
		components.clear();
		componentsRemoved.clear();
	}

	Scene* Entity::getScene() {
		return scene;
	}

	std::string Entity::getEntityName() {
		return name;
	}

	void Entity::init() {
		for (auto c : components) {
			c->init();
		}
	}

	void Entity::udpate() {
		for (auto c : components) {
			if (c->isActive()) c->update();
		}

		for (auto it : componentsRemoved) {
			delete *it;
			components.erase(it);
		}
		componentsRemoved.clear();
	}

	void Entity::fixedUpdate() {
		for (auto c : components) {
			if (c->isActive()) c->fixedUpdate();
		}
	}

	void Entity::onActive() {
		for (auto c : components) {
			 c->onActive();
		}
	}

	void Entity::onDeacitve() {
		for (auto c : components) {
			c->onDeacitve();
		}
	}

	void Entity::onSceneUp() {
		for (auto c : components) {
			c->onSceneUp();
		}
	}

	void Entity::onSceneDown() {
		for (auto c : components) {
			c->onSceneDown();
		}
	}

	void Entity::onDestroy() {
		for (auto c : components) {
			c->onDestroy();
		}
	}

	void Entity::setActive(bool ent_active) {
		active = ent_active;

		if (active) onActive();
		else onDeacitve();
	}

	bool Entity::isActive() {
		return active;
	}

	void Entity::removeEntity() {
		removed = true;
	}

	bool Entity::isRemoved() {
		return removed;
	}

	void Entity::removeComponent(Component* c) {
		std::list<Component*>::iterator it = components.begin();
		while (it != components.end()) {

			if (*it == c)
			{
				componentsRemoved.push_back(it);
				return;
			}
			it++;
		}
	}
}
