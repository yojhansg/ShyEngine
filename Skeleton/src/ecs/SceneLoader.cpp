#include "SceneLoader.h"

#include <fstream>

#include "ConsoleManager.h"
#include "SceneManager.h"

#include "Component.h"
#include "Entity.h"
#include "Scene.h"
#include "Script.h"
#include <unordered_map>

#include "ECSUtilities/ClassReflection.h"

#include "Components/Overlay.h"
#include "Components/Transform.h"

#include "ConsoleManager.h"


using namespace nlohmann;

const std::string ECS::SceneLoader::extension = ".json";

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

	jsonarray objects = file["objects"].get<jsonarray>();

	for (auto& obj : objects) {

		ProcessTransform(scene, obj, nullptr);

	}

	if (file.contains("overlays")) {

		jsonarray overlays = file["overlays"].get<jsonarray>();

		for (auto& overlay : overlays) {

			ProcessOverlay(scene, overlay, nullptr);
		}
	}

	fileStream.close();

	return scene;
}


void ECS::SceneLoader::ProcessTransform(ECS::Scene* scene, nlohmann::json& obj, ECS::Transform* parent)
{

	int renderOrder = 0;
	std::string objectName = "New Entity";

	if (obj.contains("name"))
		objectName = obj["name"].get<std::string>();

	if (obj.contains("order")) { //TODO: mover el orden al transform ya que es la posicion z
		renderOrder = obj["order"].get<int>();
	}

	ECS::Entity* entity = scene->createEntity(objectName, renderOrder);

	ECS::Transform* transform = entity->addComponent<ECS::Transform>();

	std::unordered_map<std::string, std::string> map;

	const std::vector<std::string> transformAttributes = { "localPosition", "localScale", "localRotation"};

	for (auto& attr : transformAttributes) {

		if (obj.contains(attr)) {
			map[attr] = obj[attr].get<std::string>();
		}
	}


	ClassReflection::instance()->ReflectTransform(transform, map);
	transform->SetParent(parent);


	if (obj.contains("components")) {

		jsonarray components = obj["components"].get<jsonarray>();
		for (auto& compInfo : components) {

			ProcessComponent(entity, compInfo);
		}
	}
	if (obj.contains("scripts")) {
		jsonarray scripts = obj["scripts"].get<jsonarray>();
		for (auto& script : scripts) {

			entity->AddScript(script.get<std::string>());
		}
	}

	if (obj.contains("childs")) {

		jsonarray childs = obj["childs"].get<jsonarray>();

		for (auto& child : childs) {

			ProcessTransform(scene, child, transform);
		}
	}
}

void ECS::SceneLoader::ProcessOverlay(ECS::Scene* scene, nlohmann::json& overlay, ECS::Overlay* parent)
{
	std::string name = "New Overlay";

	if (overlay.contains("name")) {

		name = overlay["name"].get<std::string>();
	}

	ECS::Entity* entity = scene->createEntity(name);

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

	if (overlay.contains("components")) {
		jsonarray components = overlay["components"].get<jsonarray>();

		for (auto& compInfo : components) {

			std::string componentStr = compInfo["component"].get<std::string>();

			ECS::Component* component = entity->addComponent(componentStr);

			std::unordered_map<std::string, std::string> attributeMap;


			if (compInfo.contains("attributes")) {

				for (auto& attribute : compInfo["attributes"].items()) {

					attributeMap[attribute.key()] = attribute.value();
				}
			}

			ClassReflection::instance()->ReflectComponent(componentStr, component, attributeMap);
		}

	}

	if (overlay.contains("scripts")) {

		jsonarray scripts = overlay["scripts"].get<jsonarray>();

		for (auto& script : scripts)
		{
			entity->AddScript(script.get<std::string>());
		}
	}


	if (overlay.contains("childs")) {

		jsonarray childs = overlay["childs"].get<jsonarray>();

		for (auto& child : childs) {

			ProcessOverlay(scene, child, overlayElement);
		}
	}

}

void ECS::SceneLoader::ProcessComponent(ECS::Entity* entity, nlohmann::json& compInfo)
{
	std::string componentStr = compInfo["component"].get<std::string>();

	ECS::Component* component = entity->addComponent(componentStr);


	std::unordered_map<std::string, std::string> attributeMap;


	if (compInfo.contains("attributes")) {


		for (auto& attribute : compInfo["attributes"].items()) {

			attributeMap[attribute.key()] = attribute.value();
		}
	}


	ClassReflection::instance()->ReflectComponent(componentStr, component, attributeMap);

}
