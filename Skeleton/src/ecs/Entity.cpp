#include "Entity.h"
#include "Component.h"
#include "RenderManager.h"
#include "Scene.h"

#include "ECSUtilities/ComponentFactory.h"
#include "ConsoleManager.h"

#include "Scripting/Script.h"
#include "ReferencesManager.h"

namespace ECS {

	Entity::Entity(const std::string& ent_name, int renderOrder) {

		name = ent_name;
		active = true;
		removed = false;
		scene = nullptr;
		inRemovedEntityList = false;
		hasRemovedComponents = false;

		this->renderOrder = renderOrder;
		usesId = false;
		inRenderSet = false;
	}

	Entity::Entity(const std::string& ent_name, Scene* ent_scene, int renderOrder, int id) {

		name = ent_name;
		active = true;
		removed = false;
		scene = ent_scene;
		inRemovedEntityList = false;
		hasRemovedComponents = false;

		this->renderOrder = renderOrder;
		this->id = id;
		usesId = true;
		inRenderSet = false;
	}

	Entity::~Entity() {
		for (auto c : components) {
			delete c; c = nullptr;
		}

		components.clear();
		removedComponents.clear();

		if (inRenderSet)
			RenderManager::instance()->RemoveElement(renderIt);

		inRenderSet = false;

		if (usesId)
			getScene()->GetReferencesManager()->RemoveEntityFromMap(id);
	}

	Scene* Entity::getScene() {
		return scene;
	}

	std::string Entity::getEntityName() {
		return name;
	}

	ECS::Script* Entity::AddScript(std::string const& scriptName)
	{
		ECS::Script* script = addComponent<ECS::Script>();

		script->Initialise(scriptName);

		scripts.push_back(script);

		return script;
	}

	void Entity::AddToRenderSet() {

		if (inRenderSet)
			return;

		renderIt = RenderManager::instance()->AddElement(this);

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

	void Entity::SetAttribute(std::string const& name, Scripting::Variable const& val)
	{
		entityAttributes[name] = val;
	}

	Scripting::Variable Entity::GetAttribute(std::string const& name)
	{
		if (!entityAttributes.contains(name)) {

			Console::Output::PrintWarning("Entity attribute: " + getEntityName(), "Trying to access non existing attribute. Returning empty instead");
			return Scripting::Variable::Null();
		}

		return entityAttributes[name];
	}

	void Entity::Event(cstring name)
	{
		for (auto c : scripts) {
			c->Event(name);
		}
	}

	//TODO: comprobar si esta enabled antes
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
			if (c->isActive() && !c->isRemoved())
				c->update(deltaTime);
			else if (c->isRemoved() && !c->inRemovedComponentList)
				removeComponent(c);
		}
	}

	void Entity::lateUpdate(float deltaTime) {
		for (auto c : components) {
			if (c->isActive() && !c->isRemoved())
				c->lateUpdate(deltaTime);
			else if (c->isRemoved() && !c->inRemovedComponentList)
				removeComponent(c);
		}
	}

	void Entity::render() {
		for (auto c : components) {
			if (c->isActive()) c->render();
		}
	}

	void Entity::fixedUpdate(float fixedDeltaTime) {
		for (auto c : components) {
			if (c->isActive() && !c->isRemoved())
				c->fixedUpdate(fixedDeltaTime);
			else if (c->isRemoved() && !c->inRemovedComponentList) {
				removeComponent(c);
			}
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

	void Entity::onClickBegin()
	{
		for (auto c : components) {
			if (c->isActive()) c->onClickBegin();
		}
	}

	void Entity::onMouseHover()
	{
		for (auto c : components) {
			if (c->isActive()) c->onMouseHover();
		}
	}

	void Entity::onMouseEnter()
	{
		for (auto c : components) {
			if (c->isActive()) c->onMouseEnter();
		}
	}

	void Entity::onMouseExit()
	{
		for (auto c : components) {
			if (c->isActive()) c->onMouseExit();
		}
	}

	void Entity::onClick()
	{
		for (auto c : components) {
			if (c->isActive()) c->onClick();
		}
	}

	void Entity::onClickHold()
	{
		for (auto c : components) {
			if (c->isActive()) c->onClickHold();
		}
	}

	void Entity::onDoubleClick()
	{
		for (auto c : components) {
			if (c->isActive()) c->onDoubleClick();
		}
	}

	void Entity::onRightClick()
	{
		for (auto c : components) {
			if (c->isActive()) c->onRightClick();
		}
	}

	void Entity::onDestroy() {
		for (auto c : components) {
			c->onDestroy();
		}
	}

	void Entity::removeComponents() {
		for (auto it : removedComponents) {
			delete* it;
			components.erase(it);
		}
		removedComponents.clear();
	}

	void Entity::addComponentToList(const std::list<Component*>::iterator& it, Component* c) {

		c->remove();

		removedComponents.push_back(it);

		c->inRemovedComponentList = true;
	}

	void Entity::onDestroyRemovedComponents() {
		for (auto c : components) {
			if (c->isRemoved()) c->onDestroy();
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
				addComponentToList(it, c);
				return;
			}
			it++;
		}
	}

	Component* Entity::addComponent(std::string const& comp)
	{
		Component* c = ComponentFactory::instance()->CreateComponent(comp);
		//TODO: lanzar error si no existe
		c->entity = this;
		components.push_back(c);

		return c;
	}


}
