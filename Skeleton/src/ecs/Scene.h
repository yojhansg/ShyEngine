#pragma once

#include <string>
#include <list>

namespace ECS {

	class Entity;

	class Scene {

	public:

		Scene(const std::string& sce_name = "New Scene");

		~Scene();

		void start();

		void update(float deltaTime);

		void lateUpdate(float deltaTime);

		void render();

		void handleInput();

		void fixedUpdate(float fixedDeltaTime);

		void removeEntities();

		// Called when the scene is up
		void onSceneUp();

		// Called when the scene is changed or destroyed
		void onSceneDown();

		// Returns the name of the scene
		std::string getName();

		// Sets the name of the scene
		void setName(const std::string& sce_name);

		// Creates an entity on the scene and returns the pointer
		Entity* createEntity(const std::string& name = "New Entity");

		// Destroys an entity with a given pointer
		void destroyEntity(Entity* ent);

		// Returns the pointer to an entity that matches with the name, nullptr otherwise (Not an efficient way to find an entity)
		Entity* findEntityByName(const std::string& ent_name);

	private:

		std::string name;

		std::list<std::list<Entity*>::iterator> entitiesRemoved;
		std::list<Entity*> entities;
	};
}

