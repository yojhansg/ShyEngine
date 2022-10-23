#pragma once

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

		virtual void update() {};

		virtual void lateUpdate() {};

		virtual void render() {};

		virtual void handleInput() {};

		virtual void fixedUpdate() {};

		virtual void onActive() {};

		virtual void onDeacitve() {};

		virtual void onSceneUp() {};

		virtual void onSceneDown() {};

		virtual void onCollisionEnter() {};

		virtual void onCollisionStay() {};

		virtual void onCollisionExit() {};

		virtual void onTriggerEnter() {};

		virtual void onTriggerStay() {};

		virtual void onTriggerExit() {};

		Entity* entity;

		bool active;
	};

}

