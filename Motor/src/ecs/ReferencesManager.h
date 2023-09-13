#pragma once

#include <Singleton.h>
#include <unordered_map>

namespace ECS {

	class Entity;

	class ReferencesManager {

	public:

		void AddEntityToMap(int id, Entity* e);
		void RemoveEntityFromMap(int id);

		bool IsEntityValid(int id);
		Entity* GetEntity(int id);

	private:

		std::unordered_map<int, Entity*> map;

	};

}

