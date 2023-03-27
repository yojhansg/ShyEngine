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

	Scripting::Variable Component::CallMethod(std::string methodName, std::vector<Scripting::Variable> const& input)
	{

		auto it = registeredMethods.find(methodName);

		if (it == registeredMethods.end()) {

			//TODO:
			//Lanzar error si no se encuentra el mensaje que se quiere lanzar

			std::cout << "Error al encontrar el metodo seleccionado" << std::endl;
			return Scripting::Variable::Null();
		}

		CallableMethod method = (*it).second;
		return method(input);
	}

	Scripting::Variable Component::cacadevaca(std::vector<Scripting::Variable> const&)
	{
		std::cout << "la puta madre c++ es un coñazo" << std::endl;
		return Scripting::Variable();
	}

	Scene* Component::getScene() {
		return entity->getScene();
	}

}
