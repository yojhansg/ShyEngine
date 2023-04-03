#pragma once

#include <type_traits>
#include <string>
#include <list>
#include <set>

//TODO: Separar scripts de componentes

namespace ECS {

	class Component;
	class Scene;
	class ContactListener;
	class RenderManager;
	class OverlayManager;
	class PhysicBody;

	// This is a way to ensure that T is a Component
	template <typename T>
	concept isComponent = std::is_base_of<Component, T>::value;

	class Entity final {

		friend Scene;
		friend Component;
		friend ContactListener;
		friend RenderManager;
		friend OverlayManager;
		friend PhysicBody;

	public:

		Entity(const std::string& ent_name, int renderOrder);

		Entity(const std::string& ent_name, Scene* ent_scene, int renderOrder);

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

		//Create a component given a string with its type
		Component* addComponent(std::string const& comp);

		// Adds a T component to the entity
		template <typename T, typename ...Ts>
		requires isComponent<T>
		T* addComponent(Ts &&... args) {

			T* c = new T(std::forward<Ts>(args)...);

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

				T* c = dynamic_cast<T*>(*it);

				if (c != nullptr)
				{
					addComponentToList(it, c);
					return;
				}
				it++;
			}
		}

		// Returns a pointer to the T component if exists in the entity, nullptr otherwise
		template <typename T>
		requires isComponent<T>
		T* getComponent() {

			//TODO: se puede mejorar el rendimiento asociando a cada componente una posicion fija del array
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

		void AddToRenderSet();

		int GetRenderOrder();
		void SetRenderOrder(int neworder);

	private:

		void init();

		void start();

		void udpate(float deltaTime);

		void lateUpdate(float deltaTime);

		void render();

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
		void onCollisionEnter(Entity* b);

		// Called when the entity is colliding with other entity
		void onCollisionStay(Entity* b);

		// Called when the entity exit the collider of the other entity
		void onCollisionExit(Entity* b);

		// Called when the entity starts trigging an entity marked as trigger
		void onTriggerEnter(Entity * b);

		// Called when the entity is trigging and entity marked as trigger
		void onTriggerStay(Entity* b);

		// Called when the entity exits the trigger of the other entity marked as trigger
		void onTriggerExit(Entity* b);


		//Overlay
		void onClickBegin();
		void onClick();
		void onClickHold();
		void onDoubleClick();
		void onRightClick();


		// Called when the entity is removed
		void onDestroy();

		// Called by the scene to clear the removed components at the end of the main loop
		void removeComponents();

		void addComponentToList(const std::list<Component*>::iterator& it, Component* c);

		void onDestroyRemovedComponents();

		// Name of the entity
		std::string name;

		// Flags to indicate if the entity is active or removed
		bool active, removed;

		bool inRemovedEntityList;

		bool hasRemovedComponents;

		// Pointer to the scene where this entity is located
		Scene* scene;

		// Order to render elements
		int renderOrder;

		//Iterator to render set
		std::multiset<Entity*>::iterator renderIt;

		// Has the object already been added to the set
		bool inRenderSet;

		// List of components associated with this entity
		std::list<Component*> components;

		// List of components to remove in the end of the update
		std::list<std::list<Component*>::iterator> removedComponents;
	};

}

