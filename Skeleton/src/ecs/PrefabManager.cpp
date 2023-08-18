#include "PrefabManager.h"
#include "SceneLoader.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

#include "ECSUtilities/ClassReflection.h"
#include "Components/Transform.h"
#include "Components/Overlay.h"
#include "SceneLoader.h"
#include "ConsoleManager.h"

#include <fstream>
#include <unordered_map>

using namespace nlohmann;


namespace ECS {

	PrefabManager::PrefabManager() {}

	PrefabManager::PrefabManager(std::string const& prefabsPath) {

		prefabId = -1;

		LoadPrefabs(prefabsPath);

	}

	void PrefabManager::LoadPrefabs(std::string const& prefabsPath) {

		std::ifstream fileStream("Prefabs/" + prefabsPath + SceneLoader::extension);

		if (!fileStream.good()) {
			return;
		}

		if (!json::accept(fileStream)) {
			Console::Output::PrintError("Prefab Manager", "Trying to read an invalid prefabs file <" + prefabsPath + ">");
			return;
		}

		fileStream.clear();
		fileStream.seekg(0);

		json json = json::parse(fileStream);

		if (!json.contains("prefabs")) return; // TODO Manejar el error

		jsonarray prefabsArray = json["prefabs"].get<jsonarray>();

		for (auto& pref : prefabsArray) {

			// Si el JSON no contiene atributo "name"
			if (!pref.contains("name")) continue;

			// Si el JSON no contiene atributo "withOverlay"
			if (!pref.contains("withOverlay")) continue;

			bool wOverlay = pref["withOverlay"].get<bool>();

			if (wOverlay)
				prefabsWithOverlay.insert(std::make_pair(pref["name"].get<std::string>(), pref));
			else
				prefabsWithTransform.insert(std::make_pair(pref["name"].get<std::string>(), pref));

		}
	}

	void PrefabManager::InstantiatePrefabWithTransform(const std::string& prefabName, Scene* scene) {

		if (!prefabsWithTransform.contains(prefabName)) return; // TODO Manejar el error

		// Se accede al map para obtener el json correspondiente al nombre del prefab proporcionado
		json prefabData = prefabsWithTransform[prefabName].get<json>();

		// Si el JSON no contiene atributo "name"
		if (!prefabData.contains("name")) return; // TODO Manejar el error

		// Si el JSON no contiene atributo "renderOrder"
		if (!prefabData.contains("order")) return; // TODO: mover el orden al transform ya que es la posicion z


		// Se crea la entidad en la escena proporcionada
		ECS::Entity* entity = scene->createEntity(prefabData["name"].get<std::string>(), prefabData["order"].get<int>());



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

			// Components & Scripts
			AddComponentsAndScriptsToEntity(prefabData, entity);


		
		// Se inicializa la entidad en la escena
		entity->init();
		entity->start();

	}

	void PrefabManager::InstantiatePrefabWithOverlay(const std::string& prefabName, Scene* scene) {

		if (!prefabsWithOverlay.contains(prefabName)) return; // TODO Manejar el error

		// Se accede al map para obtener el json correspondiente al nombre del prefab proporcionado
		json prefabData = prefabsWithOverlay[prefabName].get<json>();

		// Si el JSON no contiene atributo "name"
		if (!prefabData.contains("name")) return; // TODO Manejar el error

		// Si el JSON no contiene atributo "renderOrder"
		if (!prefabData.contains("order")) return; // TODO Manejar el error y mover el orden al transform ya que es la posicion z


		// Se crea la entidad en la escena proporcionada
		ECS::Entity* entity = scene->createEntity(prefabData["name"].get<std::string>(), prefabData["order"].get<int>());



			// Overlay Component
			ECS::Overlay* overlayElement = entity->addComponent<ECS::Overlay>();

			std::unordered_map<std::string, std::string> map;

			const std::vector<std::string> overlayAttributes = { "placement", "anchor", "top", "left", "right", "bottom", "position", "size" , "color" , "interactable" };

			for (auto& attr : overlayAttributes) {

				if (prefabData.contains(attr)) {
					map[attr] = prefabData[attr].get<std::string>();
				}
			}

			ClassReflection::instance()->ReflectOverlay(overlayElement, map);
			overlayElement->SetParent(nullptr);

			// Components & Scripts
			AddComponentsAndScriptsToEntity(prefabData, entity);



		// Se inicializa la entidad en la escena
		entity->init();
		entity->start();

	}

	void PrefabManager::AddComponentsAndScriptsToEntity(nlohmann::json& prefabData, Entity* entity) {

		// Components
		if (prefabData.contains("components")) {

			jsonarray components = prefabData["components"].get<jsonarray>();

			for (auto& compInfo : components)
				SceneLoader::ProcessComponent(entity, compInfo);
		}


		// Scripts 
		if (prefabData.contains("scritps")) {
			SceneLoader::ProcessScripts(prefabData, entity);
		}

	}

}
