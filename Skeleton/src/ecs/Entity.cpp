#include "Entity.h"
#include "Component.h"
#include "Scene.h"
#include "ECSUtilities/ComponentFactory.h"

#include "RenderManager.h"
namespace ECS {

	Entity::Entity(const std::string& ent_name, int renderOrder) {

		name = ent_name;
		active = true;
		removed = false;
		scene = nullptr;

		this->renderOrder = renderOrder;
	}

	Entity::Entity(const std::string& ent_name, Scene* ent_scene, int renderOrder) {

		name = ent_name;
		active = true;
		removed = false;
		scene = ent_scene;

		this->renderOrder = renderOrder;
	}

	Entity::~Entity() {
		for (auto c : components) {
			delete c; c = nullptr;
		}
		components.clear();
		componentsRemoved.clear();

		if (inRenderSet)
			RenderManager::instance()->RemoveElement(this);

		inRenderSet = false;
	}

	Scene* Entity::getScene() {
		return scene;
	}

	std::string Entity::getEntityName() {
		return name;
	}

	void Entity::AddToRenderSet() {

		if (inRenderSet)
			return;

		RenderManager::instance()->AddElement(this);

		inRenderSet = true;
	}

	int Entity::GetRenderOrder()
	{
		return renderOrder;
	}

	void Entity::SetRenderOrder(int neworder)
	{
		renderOrder = neworder;
	}

	void Entity::init() {
		for (auto c : components) {
			c->init();
		}
	}

	void Entity::start() {
		for (auto c : components) {
			c->start();
		}
	}

	void Entity::udpate(float deltaTime) {
		for (auto c : components) {
			if (c->isActive()) c->update(deltaTime);
		}

		for (auto it : componentsRemoved) {
			delete *it;
			components.erase(it);
		}
		componentsRemoved.clear();
	}

	void Entity::lateUpdate(float deltaTime) {
		for (auto c : components) {
			if (c->isActive()) c->lateUpdate(deltaTime);
		}
	}

	void Entity::render() {
		for (auto c : components) {
			if (c->isActive()) c->render();
		}
	}

	void Entity::fixedUpdate(float fixedDeltaTime) {
		for (auto c : components) {
			if (c->isActive()) c->fixedUpdate(fixedDeltaTime);
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
			if (c->isActive()) c->onSceneUp();
		}
	}

	void Entity::onSceneDown() {
		for (auto c : components) {
			if (c->isActive()) c->onSceneDown();
		}
	}

	void Entity::onCollisionEnter(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onCollisionEnter(b);
		}
	}

	void Entity::onCollisionStay(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onCollisionStay(b);
		}
	}

	void Entity::onCollisionExit(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onCollisionExit(b);
		}
	}

	void Entity::onTriggerEnter(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onTriggerEnter(b);
		}
	}

	void Entity::onTriggerStay(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onTriggerStay(b);
		}
	}

	void Entity::onTriggerExit(Entity* b) {
		for (auto c : components) {
			if (c->isActive()) c->onTriggerExit(b);
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

		for (auto comp : components) {

			comp->onDestroy();
		}

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

	Component* Entity::addComponent(std::string const& comp)
	{
		Component* c = ComponentFactory::instance()->CreateComponent(comp);

		c->entity = this;
		components.push_back(c);

		return c;
	}


}
