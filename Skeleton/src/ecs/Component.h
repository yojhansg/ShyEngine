#pragma once

#include "Scripting/Variable.h"
#include <map>
#include <vector>
#include <string>

#include <EditorExport.h>

#define ECS_Version 1.0


namespace ECS {

	class Entity;
	class Scene;

	class Component {
		friend Entity;

	public:

		Component();

		virtual ~Component() {};

		Entity* getEntity();

		Scene* getScene();

		bool isActive();

		void setActive(bool ent_active);

		void remove();

		void remove(Component* c);

	private:

		virtual void init() {};

		virtual void start() {};

		virtual void update(float deltaTime) {};

		virtual void lateUpdate(float deltaTime) {};

		virtual void render() {};

		virtual void fixedUpdate(float fixedDeltaTime) {};

		virtual void onActive() {};

		virtual void onDeacitve() {};

		virtual void onSceneUp() {};

		virtual void onSceneDown() {};

		virtual void onCollisionEnter(Entity* b) {};

		virtual void onCollisionStay() {};

		virtual void onCollisionExit(Entity* b) {};

		virtual void onTriggerEnter() {};

		virtual void onTriggerStay() {};

		virtual void onTriggerExit() {};

		
	protected:

		Entity* entity;
		bool active;
	};
}

