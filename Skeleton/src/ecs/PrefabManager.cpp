#include "PrefabManager.h"
#include "SceneLoader.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

#include "ECSUtilities/ClassReflection.h"
#include "Components/Transform.h"
#include "SceneLoader.h"
#include "ConsoleManager.h"

#include <fstream>
#include <unordered_map>

using namespace nlohmann;


namespace ECS {

	PrefabManager::PrefabManager(std::string const& prefabsPath) {

		LoadPrefabs(prefabsPath);

	}

	void PrefabManager::LoadPrefabs(std::string const& prefabsPath) {

		std::ifstream fileStream("Prefabs/" + prefabsPath + SceneLoader::extension);

		if (!fileStream.good()) {
			Console::Output::PrintError("Prefabs loading", "Cannot open prefabs file <" + prefabsPath + ">");
		}

		if (!json::accept(fileStream)) {
			Console::Output::PrintError("Prefab Manager", "Trying to read an invalid prefabs file <" + prefabsPath + ">");
		}

		fileStream.clear();
		fileStream.seekg(0);

		json json = json::parse(fileStream);

		jsonarray prefabsArray = json["prefabs"].get<jsonarray>();

		for (auto& pref : prefabsArray) {

			// Si el JSON no contiene atributo "name"
			if (!pref.contains("name")) continue;

			prefabsData.insert(std::make_pair(pref["name"].get<std::string>(), pref));

		}
	}

	void PrefabManager::Instantiate(const std::string& prefabName, Scene* scene) {

		if (!prefabsData.contains(prefabName)) return; // TODO Manejar el error

		// Se accede al map para obtener el json correspondiente al nombre del prefab proporcionado
		json prefabData = prefabsData[prefabName].get<json>();

		// Si el JSON no contiene atributo "name"
		if (!prefabData.contains("name")) return; // TODO Manejar el error

		// Si el JSON no contiene atributo "renderOrder"
		if (!prefabData.contains("order")) return; // TODO: mover el orden al transform ya que es la posicion z


		// Se crea la entidad en la escena proporcionada
		ECS::Entity* entity = scene->createEntity(prefabData["name"].get<std::string>(), prefabData["order"].get<int>());

		// Se añaden los componentes a la entidad

			// Transform
			ECS::Transform* transform = entity->addComponent<Transform>();

			std::unordered_map<std::string, std::string> map;

			const std::vector<std::string> transformAttributes = { "localPosition", "localScale", "localRotation" };

			for (auto& attr : transformAttributes) {
				if (prefabData.contains(attr))
					map[attr] = prefabData[attr].get<std::string>();
			}

			ClassReflection::instance()->ReflectTransform(transform, map);
			transform->SetParent(nullptr);


			// Components
			if (prefabData.contains("components")) {

				jsonarray components = prefabData["components"].get<jsonarray>();

				for (auto& compInfo : components)
					SceneLoader::ProcessComponent(entity, compInfo);
			}


			// Scripts (TODO)
		

		// Se inicializa la entidad en la escena
		entity->init();
		entity->start();

	}

}
