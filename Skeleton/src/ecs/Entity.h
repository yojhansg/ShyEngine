#pragma once

#include <type_traits>
#include <string>
#include <list>

namespace ECS {

	class Component;
	class Scene;

	// This is a way to ensure that T is a Component
	template <typename T>
	concept isComponent = std::is_base_of<Component, T>::value;

	class Entity final {

	public:

		Entity(const std::string& ent_name);

	private:

		void init();

		void udpate();

		void fixedUpdate();

		void setActive(bool ent_active);

		bool isActive();

		void removeEntity();


		std::string name;

		bool active;
		bool removed;

		std::list<Component*> components;

	};

}

