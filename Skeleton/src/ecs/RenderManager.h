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

		void AddElement(Entity* elem);
		void RemoveElement(Entity* elem);

	private:

		std::set<Entity*, EntityComparer> set;
	};

}

