#pragma once

#include "Singleton.h"
#include <set>

namespace ECS {

	class Entity;


	struct EntityComparer {
		bool operator()(Entity* a, Entity* b) const;
	};


	class RenderManager : public Utilities::Singleton<RenderManager> {

	public:

		friend class Engine;

		RenderManager();
		~RenderManager();

		void Render();

		std::multiset<Entity*>::iterator AddElement(Entity* elem);
		void RemoveElement(std::multiset<Entity*>::iterator elem);

	private:

		std::multiset<Entity*, EntityComparer> set;
	};

}

