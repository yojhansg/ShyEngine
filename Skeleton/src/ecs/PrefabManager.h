#pragma once

#include "Singleton.h"
#include <string>
#include <unordered_map>

#undef new
#include "json.hpp"

namespace nlohmann {
	using jsonarray = std::vector<json>;
}

namespace ECS {

	class Entity;
	class Scene;

	class PrefabManager : public Utilities::Singleton<PrefabManager> {

	public:

		PrefabManager(std::string const& prefabsPath);

		// Instancia una entidad en la escena a partir un prefab
		void Instantiate(const std::string& prefabName, Scene* scene);

	private:

		// Parsea el JSON y guarda la informacion en la estructura prefabsData
		void LoadPrefabs(std::string const& prefabsPath);

		// Informacion del JSON parseado
		std::unordered_map<std::string, nlohmann::json> prefabsData;

	};

}

