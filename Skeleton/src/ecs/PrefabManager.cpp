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

		if (!json.contains("prefabs")) {
			Console::Output::PrintError("Load prefabs error", "The prefabs file doesn't have a valid format.");
			return;
		}

		jsonarray prefabsArray = json["prefabs"].get<jsonarray>();

		for (auto& pref : prefabsArray) {

			// If the JSON doesn't contain the "name" attribute
			if (!pref.contains("name")) {
				Console::Output::PrintError("Load prefabs error", "The prefabs file doesn't have a valid format.");
				return;
			}

			// If the JSON doesn't contain the "withOverlay" attribute
			if (!pref.contains("withOverlay")) {
				Console::Output::PrintError("Load prefabs error", "The prefabs file doesn't have a valid format.");
				return;
			}

			bool wOverlay = pref["withOverlay"].get<bool>();

			if (wOverlay)
				prefabsWithOverlay.insert(std::make_pair(pref["name"].get<std::string>(), pref));
			else
				prefabsWithTransform.insert(std::make_pair(pref["name"].get<std::string>(), pref));

		}
	}

	void PrefabManager::InstantiatePrefabWithTransform(const std::string& prefabName, Scene* scene) {

		if (!prefabsWithTransform.contains(prefabName)) {
			Console::Output::PrintError("Instantiate prefab", "There is no prefab with name: " + prefabName);
			return;
		}

		// Se accede al map para obtener el json correspondiente al nombre del prefab proporcionado
		json prefabData = prefabsWithTransform[prefabName].get<json>();

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

		if (!prefabsWithOverlay.contains(prefabName)) {
			Console::Output::PrintError("Instantiate prefab", "There is no prefab with name: " + prefabName);
			return;
		}

		// Se accede al map para obtener el json correspondiente al nombre del prefab proporcionado
		json prefabData = prefabsWithOverlay[prefabName].get<json>();

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

		if (!prefabData.contains("components")) {
			Console::Output::PrintError("Instantiate prefab", "The prefab file doesnt have a valid format");
			return;
		}

		// Components
		jsonarray components = prefabData["components"].get<jsonarray>();
		for (auto& compInfo : components)
			SceneLoader::ProcessComponent(entity, compInfo);


		if (!prefabData.contains("scritps")) {
			Console::Output::PrintError("Instantiate prefab", "The prefab file doesnt have a valid format");
			return;
		}

		// Scripts 
		SceneLoader::ProcessScripts(prefabData, entity);

	}

}
