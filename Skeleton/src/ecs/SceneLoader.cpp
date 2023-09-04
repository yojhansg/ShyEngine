#include "SceneLoader.h"

#include "ConsoleManager.h"
#include "SceneManager.h"
#include "Component.h"
#include "Script.h"
#include "Entity.h"
#include "Scene.h"

#include "ECSUtilities/ClassReflection.h"
#include "Components/Transform.h"
#include "Components/Overlay.h"
#include "ReferencesManager.h"
#include "ResourcesManager.h"
#include "ConsoleManager.h"

#include <unordered_map>
#include <fstream>

using namespace nlohmann;


#include <filesystem>
#include <iostream>
// ------------------------- LOAD SCENE ----------------------------

ECS::Scene* ECS::SceneLoader::LoadScene(std::string const& scenePath)
{
	std::ifstream fileStream(scenePath);

	if (!fileStream.good())
	{
		Console::Output::PrintError("Scene loading", "Cannot open scene <" + scenePath + ">");
		return nullptr; 
	}

	if (!json::accept(fileStream))
	{
		Console::Output::PrintError("Scene Manager", "Trying to read an invalid scene <" + scenePath + ">");
		return nullptr;
	}

	fileStream.clear();
	fileStream.seekg(0);

	json file = json::parse(fileStream);

	fileStream.close();


	if (!file.contains("name")) {
		Console::Output::PrintError("Load scene error", "The scene file doesn't have a valid format.");
		return nullptr;
	}

	Scene* scene = SceneManager::instance()->createScene(file["name"].get<std::string>());


	if (file.contains("position"))
		scene->cameraPosition = file["position"].get<std::string>();

	if (file.contains("scale"))
		scene->cameraScale = file["scale"].get<float>();


	if (!file.contains("objects")) {
		Console::Output::PrintError("Load scene error", "The scene file doesn't have a valid format.");
		return nullptr;
	}

	// Entities with transform processing
	jsonarray objects = file["objects"].get<jsonarray>();
	for (auto& obj : objects)
		ProcessEntityWithTransform(scene, obj, nullptr);



	if (!file.contains("overlays")) {
		Console::Output::PrintError("Load scene error", "The scene file doesn't have a valid format.");
		return nullptr;
	}

	// Entities with overlay processing
	jsonarray overlays = file["overlays"].get<jsonarray>();
	for (auto& overlay : overlays) {
		ProcessEntityWithOverlay(scene, overlay, nullptr);
	}

	return scene;
}






// ------------------------- PROCESS TRANSFORM ----------------------------

void ECS::SceneLoader::ProcessEntityWithTransform(ECS::Scene* scene, nlohmann::json& obj, ECS::Transform* parent) {

	int renderOrder = 0;
	std::string objectName = "New Entity";
	int id = 0;


	if (!obj.contains("name")) {
		Console::Output::PrintError("Load entity error", "The entity JSON doesn't have a valid format.");
		return;
	}

	objectName = obj["name"].get<std::string>();


	if (!obj.contains("order")) { // TODO: mover el orden al transform ya que es la posicion z
		Console::Output::PrintError("Load entity error", "The entity JSON doesn't have a valid format.");
		return;
	}

	renderOrder = obj["order"].get<int>();


	if (!obj.contains("id")) {
		Console::Output::PrintError("Load entity error", "The entity JSON doesn't have a valid format.");
		return;
	}

	id = obj["id"].get<int>();


	ECS::Entity* entity = scene->createEntity(objectName, renderOrder, id);

	ReferencesManager::instance()->AddEntityToMap(id, entity);

	// Transform component
	ECS::Transform* transform = entity->addComponent<ECS::Transform>();

	std::unordered_map<std::string, std::string> map;

	const std::vector<std::string> transformAttributes = { "localPosition", "localScale", "localRotation" };

	for (auto& attr : transformAttributes) {

		if (!obj.contains(attr)) {
			Console::Output::PrintError("Load transform error", "The transform component doesn't have a valid format.");
			return;
		}

		map[attr] = obj[attr].get<std::string>();
	}


	ClassReflection::instance()->ReflectTransform(transform, map);
	transform->SetParent(parent, false);


	if (!obj.contains("components")) {
		Console::Output::PrintError("Load components error", "The components JSON doesn't have a valid format.");
		return;
	}

	// All compomnents
	jsonarray components = obj["components"].get<jsonarray>();
	for (auto& compInfo : components) {
		ProcessComponent(entity, compInfo);
	}


	if (!obj.contains("scripts")) {
		Console::Output::PrintError("Load scripts error", "The scripts JSON doesn't have a valid format.");
		return;
	}

	// Scritps
	ProcessScripts(obj, entity);


	if (!obj.contains("childs")) {
		Console::Output::PrintError("Load childs error", "The childs JSON doesn't have a valid format.");
		return;
	}


	// Entity childs
	jsonarray childs = obj["childs"].get<jsonarray>();

	for (auto& child : childs) {
		ProcessEntityWithTransform(scene, child, transform);
	}
}




// ------------------------- PROCESS OVERLAY ----------------------------

void ECS::SceneLoader::ProcessEntityWithOverlay(ECS::Scene* scene, nlohmann::json& overlay, ECS::Overlay* parent)
{
	std::string name = "New Overlay";
	int id = 0;

	if (!overlay.contains("name")) {
		Console::Output::PrintError("Load entity error", "The entity JSON doesn't have a valid format.");
		return;
	}

	name = overlay["name"].get<std::string>();


	if (!overlay.contains("id")) {
		Console::Output::PrintError("Load entity error", "The entity JSON doesn't have a valid format.");
		return;
	}

	id = overlay["id"].get<int>();

	ECS::Entity* entity = scene->createEntity(name, 0, id);

	ReferencesManager::instance()->AddEntityToMap(id, entity);

	// Overlay Component
	ECS::Overlay* overlayElement = entity->addComponent<ECS::Overlay>();

	std::unordered_map<std::string, std::string> map;

	const std::vector<std::string> overlayAttributes = { "placement", "anchor", "top", "left", "right", "bottom", "position", "size" , "color" , "interactable" };

	for (auto& attr : overlayAttributes) {


		//No es necesario que los overlays contengan todas las propiedades por lo que no se necesita señalar un error
		if (overlay.contains(attr)) {

			map[attr] = overlay[attr].get<std::string>();
		}
	}

	ClassReflection::instance()->ReflectOverlay(overlayElement, map);
	overlayElement->SetParent(parent);


	if (!overlay.contains("components")) {
		Console::Output::PrintError("Load components error", "The components JSON doesn't have a valid format.");
		return;
	}

	// All components
	jsonarray components = overlay["components"].get<jsonarray>();

	for (auto& compInfo : components) {
		ProcessComponent(entity, compInfo);
	}


	if (!overlay.contains("scripts")) {
		Console::Output::PrintError("Load scripts error", "The scripts JSON doesn't have a valid format.");
		return;
	}

	// Scritps
	ProcessScripts(overlay, entity);



	if (!overlay.contains("childs")) {
		Console::Output::PrintError("Load childs error", "The childs JSON doesn't have a valid format.");
		return;
	}

	// Entity childs
	jsonarray childs = overlay["childs"].get<jsonarray>();

	for (auto& child : childs)
		ProcessEntityWithOverlay(scene, child, overlayElement);

}



// ------------------------- PROCESS COMPONENT ----------------------------

void ECS::SceneLoader::ProcessComponent(ECS::Entity* entity, nlohmann::json& compInfo) {

	std::string componentStr = compInfo["component"].get<std::string>();

	ECS::Component* component = entity->addComponent(componentStr);

	std::unordered_map<std::string, std::string> attributeMap;

	if (compInfo.contains("attributes")) {

		for (auto& attribute : compInfo["attributes"].items())
			attributeMap[attribute.key()] = attribute.value();

	}

	ClassReflection::instance()->ReflectComponent(componentStr, component, attributeMap);

}



// -------------------------- PROCESS SCRITPS ---------------------------

void ECS::SceneLoader::ProcessScripts(nlohmann::json& jsonData, ECS::Entity* entity) {

	json& scripts = jsonData["scripts"];

	for (auto& script : scripts.items())
	{
		json& scriptAttributes = script.value();
		ECS::Script* scriptCmp = entity->AddScript(script.key());

		for (auto& attribute : scriptAttributes.items()) {

			json& attr = attribute.value();

			Scripting::Variable value;

			std::string type = attr["type"].get<std::string>();

			if (type == "float") {

				float v = attr["value"].get<float>();
				value = v;
			}
			else if (type == "bool") {

				bool b = attr["value"].get<bool>();
				value = b;
			}
			else if (type == "char") {

				char c = attr["value"].get<char>();
				value = c;
			}
			else if (type == "Vector2D") {
				std::string vec = attr["value"].get<std::string>();
				value = (Utilities::Vector2D)vec;
			}
			else if (type == "string") {
				std::string vec = attr["value"].get<std::string>();
				value = vec;
			}
			else if (type == "color") {
				Utilities::Color col = Utilities::Color::CreateColor(attr["value"].get<std::string>());
				value = col;
			}
			else if (type == "Entity") {

				value = Scripting::Variable::Entity(nullptr);
				value.value.entityId = attr["value"].get<int>();
			}

			scriptCmp->SetSerialisedValue(attribute.key(), value);
		}
	}

}

