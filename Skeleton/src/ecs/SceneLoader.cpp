#include "SceneLoader.h"

#include <fstream>

#include "ConsoleManager.h"
#include "SceneManager.h"

#include "Component.h"
#include "Entity.h"
#include "Scene.h"
#include "Script.h"

#include "ECSUtilities/ClassReflection.h"

#include "Components/OverlayElement.h"

using namespace nlohmann;

const std::string ECS::SceneLoader::extension = ".json";

ECS::Scene* ECS::SceneLoader::LoadScene(std::string const& scenePath)
{
	std::ifstream fileStream("Scenes/" + scenePath);

	if (!fileStream.good())
	{
		Console::Output::PrintError("Scene loading", "Cannot open scene <" + scenePath + ">");

		return nullptr;
	}
	json file = json::parse(fileStream);


	Scene* scene = SceneManager::instance()->createScene(file["name"].get<std::string>());

	jsonarray objects = file["objects"].get<jsonarray>();

	for (auto& obj : objects) {

		int renderOrder = 0;
		std::string objectName = "New Entity";

		if (obj.contains("name"))
			objectName = obj["name"].get<std::string>();

		if (obj.contains("order")) {
			renderOrder = obj["order"].get<int>();
		}

		ECS::Entity* entity = scene->createEntity(objectName, renderOrder);


		jsonarray components = obj["components"].get<jsonarray>();
		for (auto& compInfo : components) {

			ProcessComponent(entity, compInfo);
		}

		jsonarray scripts = obj["scripts"].get<jsonarray>();
		for (auto& script : scripts) {

			entity->addComponent<ECS::Script>()->Initialise(script.get<std::string>());
		}
	}


	jsonarray overlays = file["overlays"].get<jsonarray>();

	for (auto& overlay : overlays) {

		ProcessOverlay(scene, overlay, nullptr);
	}


	//// OverlayElement
	//ECS::Entity *overlay, *text;

	//ECS::OverlayElement* elem;
	//ECS::OverlayText* textText;

	//overlay = scene->createEntity("Overlay");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();

	//elem->SetPositioned({ 10, 10 }, { 200, 100 });
	//elem->SetAnchorTopLeft();

	////====

	//overlay = scene->createEntity("Overlay 2");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();

	//elem->SetPositioned({ 10, 120 }, { 200, 100 });
	//elem->SetAnchorTopLeft();

	////====

	//overlay = scene->createEntity("Overlay 2");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();

	//elem->SetPositioned({ 10, 230 }, { 200, 100 });
	//elem->SetAnchorTopLeft();


	////====

	//overlay = scene->createEntity("Overlay 3");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();

	//elem->SetPositioned({ 10, 340 }, { 400, 100 });
	//elem->SetAnchorTopLeft();



	//overlay = scene->createEntity("Overlay 3");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();

	//elem->SetPositioned({ 500, 340 }, { 400, 100 });
	//elem->SetAnchorTopLeft();



	//// OverlayElement
	//text = scene->createEntity("Clamp");
	//elem = text->addComponent<ECS::OverlayElement>();
	//textText = text->addComponent<ECS::OverlayText>();

	//elem->SetPositioned({ 10, 10 }, { 200, 100 });
	//elem->SetAnchorTopLeft();
	//textText->SetText("Clamp y se corta el texto");


	//// OverlayElement
	//text = scene->createEntity("Overlay");
	//elem = text->addComponent<ECS::OverlayElement>();
	//textText = text->addComponent<ECS::OverlayText>();

	//elem->SetPositioned({ 10, 120 }, { 200, 100 });
	//elem->SetAnchorTopLeft();
	//textText->SetFit(1);
	//textText->SetText("Este es un ejemplo de Overflow");


	//text = scene->createEntity("Overlay");
	//elem = text->addComponent<ECS::OverlayElement>();
	//textText = text->addComponent<ECS::OverlayText>();

	//elem->SetPositioned({ 10, 230 }, { 200, 100 });
	//elem->SetAnchorTopLeft();
	//textText->SetFit(4);
	//textText->SetPointSize(100);
	//textText->SetText("Me expando");


	//text = scene->createEntity("Wrapped");
	//elem = text->addComponent<ECS::OverlayElement>();
	//textText = text->addComponent<ECS::OverlayText>();

	//elem->SetPositioned({ 10, 340 }, { 400, 100 });
	//elem->SetAnchorTopLeft();
	//textText->SetFit((int)OverlayText::Fit::WrapClamp);
	//textText->SetText("HOLA ME CAMBIO DE LINEA");


	//text = scene->createEntity("Wrapped overflow");
	//elem = text->addComponent<ECS::OverlayElement>();
	//textText = text->addComponent<ECS::OverlayText>();

	//elem->SetPositioned({ 500, 340 }, { 400, 100 });
	//elem->SetAnchorTopLeft();
	//textText->SetFit((int)OverlayText::Fit::WrapOverflow);
	//textText->SetText("Hola soy un texto que se expande entre varias lineas y encima me salgo del total mama mia soy un loco");

	//Pruebas de jerarquia


	//auto overlay = scene->createEntity("Overlay papa");
	//auto elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();
	//elem->SetAnchorCenter();
	//elem->SetStreched(600, 50, 50, 50);

	//auto papa = elem;

	//overlay = scene->createEntity("Overlay hijo");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>()->path = "pollo.png";
	//elem->SetPositioned({0, 0}, {300, 300});
	//elem->SetAnchorCenter();
	//elem->SetParent(papa);

	//papa = elem;

	//overlay = scene->createEntity("Overlay nieto");
	//elem = overlay->addComponent<ECS::OverlayElement>();
	//overlay->addComponent<ECS::OverlayImage>();
	//elem->SetStreched(100, 100, 100, 0);
	//elem->SetAnchorCenter();
	//elem->SetParent(papa);

	return scene;
}


void ECS::SceneLoader::ProcessOverlay(ECS::Scene* scene, nlohmann::json& overlay, ECS::OverlayElement* parent)
{
	std::string name = "New Overlay";

	if (overlay.contains("name")) {

		name = overlay["name"].get<std::string>();
	}

	ECS::Entity* entity = scene->createEntity(name);

	ECS::OverlayElement* overlayElement = entity->addComponent<ECS::OverlayElement>();

	std::map<std::string, std::string> map;

	const std::vector<std::string> overlayAttributes = { "placement", "anchor", "top", "left", "right", "bottom", "position", "size" };

	for (auto& attr : overlayAttributes) {

		if (overlay.contains(attr)) {
			map[attr] = overlay[attr].get<std::string>();
		}
	}

	ClassReflection::instance()->ReflectOverlayElement(overlayElement, map);
	overlayElement->SetParent(parent);

	jsonarray components = overlay["components"].get<jsonarray>();

	for (auto& compInfo : components) {

		std::string componentStr = compInfo["component"].get<std::string>();

		ECS::Component* component = entity->addComponent(componentStr);

		std::map<std::string, std::string> attributeMap;


		jsonarray attributes = compInfo["attributes"].get<jsonarray>();

		for (auto& attribute : attributes) {

			std::string attributeName = attribute["name"].get<std::string>();
			std::string attributeValue = attribute["value"].get<std::string>();

			attributeMap[attributeName] = attributeValue;

		}

		ClassReflection::instance()->ReflectComponent(componentStr, component, attributeMap);

	}

	jsonarray childs = overlay["childs"].get<jsonarray>();

	for (auto& child : childs) {

		ProcessOverlay(scene, child, overlayElement);
	}

}

void ECS::SceneLoader::ProcessComponent(ECS::Entity* entity, nlohmann::json& compInfo)
{
	std::string componentStr = compInfo["component"].get<std::string>();

	ECS::Component* component = entity->addComponent(componentStr);


	std::map<std::string, std::string> attributeMap;


	jsonarray attributes = compInfo["attributes"].get<jsonarray>();

	for (auto& attribute : attributes) {

		std::string attributeName = attribute["name"].get<std::string>();
		std::string attributeValue = attribute["value"].get<std::string>();

		attributeMap[attributeName] = attributeValue;

	}

	ClassReflection::instance()->ReflectComponent(componentStr, component, attributeMap);

}
