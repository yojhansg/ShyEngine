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

		virtual void fixedUpdate() {};

		virtual void onActive() {};

		virtual void onDeacitve() {};

		virtual void onSceneUp() {};

		virtual void onSceneDown() {};

		virtual void onDestroy() {};

		Entity* entity;

		bool active;
	};

}

