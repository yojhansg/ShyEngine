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

		friend Scene;

	public:

		Entity(const std::string& ent_name);

		Entity(const std::string& ent_name, Scene* ent_scene);

		~Entity();

		Scene* getScene();

		std::string getEntityName();

		// Sets the active flag of the entity
		void setActive(bool ent_active);

		// Returns true if the entity is active on the scene, 0 otherwise
		bool isActive();

		// Remove the entity from the scene
		void removeEntity();

		// Returns true if the entity is removed, 0 otherwise
		bool isRemoved();

		// Remove a component from a pointer to that component
		void removeComponent(Component* c);

		// Adds a T component to the entity
		template <typename T>
		requires isComponent<T>
		T* addComponent() {

			T* c = new T();

			c->entity = this;
			components.push_back(c);

			return c;
		}

		// Returns true if the entity has the T component, false otherwise
		template <typename T>
		requires isComponent<T>
		bool hasComponent() {
			return getComponent<T>() != nullptr;
		}

		// Removed the T component from the entity
		template <typename T>
		requires isComponent<T>
		void removeComponent() {

			std::list<Component*>::iterator it = components.begin();
			while (it != components.end()) {

				if (dynamic_cast<T*>(*it) != nullptr)
				{
					componentsRemoved.push_back(it);
					return;
				}
				it++;
			}
		}

		// Returns a pointer to the T component if exists in the entity, nullptr otherwise
		template <typename T>
		requires isComponent<T>
		T* getComponent() {

			for (Component* comp : components) {
				T* v = dynamic_cast<T*>(comp);
				if (v != nullptr) {
					return v;
				}
			}

			return nullptr;
		}

		// Returns a vector with all the entity components
		template <typename T>
		requires isComponent<T>
		std::list<T*> getComponents() {

			std::list<T*> vec = components;

			return vec;
		}

	private:

		void init();

		void udpate(float deltaTime);

		void lateUpdate(float deltaTime);

		void render();

		void handleInput();

		// An update executed with the physics step rate
		void fixedUpdate(float fixedDeltaTime);

		// Called when the entity is set to active
		void onActive();

		// Called when the entity is set to inactive
		void onDeacitve();

		// Called when the scene is up
		void onSceneUp();

		// Called when the scene is changed or destroyed
		void onSceneDown();

		// Called when the entity starts colliding with other entity
		void onCollisionEnter();

		// Called when the entity is colliding with other entity
		void onCollisionStay();

		// Called when the entity exit the collider of the other entity
		void onCollisionExit();

		// Called when the entity starts trigging an entity marked as trigger
		void onTriggerEnter();

		// Called when the entity is trigging and entity marked as trigger
		void onTriggerStay();

		// Called when the entity exits the trigger of the other entity marked as trigger
		void onTriggerExit();

		// Name of the entity
		std::string name;

		// Flags to indicate if the entity is active or removed
		bool active, removed;

		// Pointer to the scene where this entity is located
		Scene* scene;

		// List of components associated with this entity
		std::list<Component*> components;

		// List of components to remove in the end of the update
		std::list<std::list<Component*>::iterator> componentsRemoved;
	};

}

