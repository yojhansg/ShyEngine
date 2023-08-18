#pragma once

#include <Singleton.h>
#include <unordered_map>

namespace ECS {

	class Entity;

	class ReferencesManager : public Utilities::Singleton<ReferencesManager> {

		friend Singleton<ReferencesManager>;

	public:

		void AddEntityToMap(int id, Entity* e);
		void RemoveEntityFromMap(int id);

	private:

		std::unordered_map<int, Entity*> map;

	};

}

