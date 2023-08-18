#include "SceneLoader.h"

#include "ConsoleManager.h"
#include "SceneManager.h"
#include "Component.h"
#include "Entity.h"
#include "Scene.h"
#include "Script.h"

#include "Components/Overlay.h"
#include "Components/Transform.h"

#include "ECSUtilities/ClassReflection.h"
#include "ConsoleManager.h"

#include <fstream>
#include <unordered_map>

using namespace nlohmann;

const std::string ECS::SceneLoader::extension = ".scene";



// ------------------------- LOAD SCENE ----------------------------

ECS::Scene* ECS::SceneLoader::LoadScene(std::string const& scenePath)
{
	std::ifstream fileStream("Scenes/" + scenePath + extension);

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

	Scene* scene = SceneManager::instance()->createScene(file["name"].get<std::string>());

	// Entities with transform processing
	if (file.contains("objects")) {

		jsonarray objects = file["objects"].get<jsonarray>();

		for (auto& obj : objects) {
			ProcessEntityWithTransform(scene, obj, nullptr);
		}

	}

	// Entities with overlay processing
	if (file.contains("overlays")) {

		jsonarray overlays = file["overlays"].get<jsonarray>();

		for (auto& overlay : overlays) {
			ProcessEntityWithOverlay(scene, overlay, nullptr);
		}
	}

	fileStream.close();

	return scene;
}






// ------------------------- PROCESS TRANSFORM ----------------------------

void ECS::SceneLoader::ProcessEntityWithTransform(ECS::Scene* scene, nlohmann::json& obj, ECS::Transform* parent) {

	int renderOrder = 0;
	std::string objectName = "New Entity";

	if (obj.contains("name"))
		objectName = obj["name"].get<std::string>();

	if (obj.contains("order")) // TODO: mover el orden al transform ya que es la posicion z
		renderOrder = obj["order"].get<int>();

	ECS::Entity* entity = scene->createEntity(objectName, renderOrder);

	// Transform component
	ECS::Transform* transform = entity->addComponent<ECS::Transform>();

	std::unordered_map<std::string, std::string> map;

	const std::vector<std::string> transformAttributes = { "localPosition", "localScale", "localRotation" };

	for (auto& attr : transformAttributes) {

		if (obj.contains(attr)) {
			map[attr] = obj[attr].get<std::string>();
		}
	}


	ClassReflection::instance()->ReflectTransform(transform, map);
	transform->SetParent(parent);


	// All compomnents
	if (obj.contains("components")) {

		jsonarray components = obj["components"].get<jsonarray>();
		for (auto& compInfo : components) {
			ProcessComponent(entity, compInfo);
		}
	}

	// Scritps
	if (obj.contains("scripts")) {
		ProcessScripts(obj, entity);
	}


	// Entity childs
	if (obj.contains("childs")) {

		jsonarray childs = obj["childs"].get<jsonarray>();

		for (auto& child : childs) {
			ProcessEntityWithTransform(scene, child, transform);
		}
	}
}




// ------------------------- PROCESS OVERLAY ----------------------------

void ECS::SceneLoader::ProcessEntityWithOverlay(ECS::Scene* scene, nlohmann::json& overlay, ECS::Overlay* parent)
{
	std::string name = "New Overlay";

	if (overlay.contains("name"))
		name = overlay["name"].get<std::string>();

	ECS::Entity* entity = scene->createEntity(name);


	// Overlay Component
	ECS::Overlay* overlayElement = entity->addComponent<ECS::Overlay>();

	std::unordered_map<std::string, std::string> map;

	const std::vector<std::string> overlayAttributes = { "placement", "anchor", "top", "left", "right", "bottom", "position", "size" , "color" , "interactable" };

	for (auto& attr : overlayAttributes) {

		if (overlay.contains(attr)) {
			map[attr] = overlay[attr].get<std::string>();
		}
	}

	ClassReflection::instance()->ReflectOverlay(overlayElement, map);
	overlayElement->SetParent(parent);


	// All components
	if (overlay.contains("components")) {
		jsonarray components = overlay["components"].get<jsonarray>();

		for (auto& compInfo : components) {
			ProcessComponent(entity, compInfo);
		}

	}

	// Scripts
	if (overlay.contains("scripts")) {
		ProcessScripts(overlay, entity);
	}


	// Entity childs
	if (overlay.contains("childs")) {

		jsonarray childs = overlay["childs"].get<jsonarray>();

		for (auto& child : childs)
			ProcessEntityWithOverlay(scene, child, overlayElement);

	}

}



// ------------------------- PROCESS COMPONENT ----------------------------

void ECS::SceneLoader::ProcessComponent(ECS::Entity* entity, nlohmann::json& compInfo)
{
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
			else if (type == "string" ) {
				std::string vec = attr["value"].get<std::string>();
				value = vec;
			}
			else if (type == "color") {
				Utilities::Color col = Utilities::Color::CreateColor(attr["value"].get<std::string>());
				value = col;
			}

			scriptCmp->SetSerialisedValue(attribute.key(), value);
		}
	}

}

