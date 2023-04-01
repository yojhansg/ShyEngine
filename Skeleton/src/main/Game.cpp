#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <PhysicsManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Components/Transform.h>
#include <Components/Image.h>
#include <Components/BoxBody.h>
#include <Components/CircleBody.h>
#include <Components/EdgeBody.h>
#include <Scripting/Script.h>
#include <Components/ChainBody.h>
#include <Components/TestComponent.h>
#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ClassReflection.h>

#include <map>
#include <fstream>

#include <Components/OverlayElement.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>

#include <ConsoleManager.h>

using namespace nlohmann;

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();
	//flappyBird();

	//readScene("DefaultScene.json");
}

void Game::firstScene() {

	Renderer::RendererManager* renderer = Renderer::RendererManager::instance();

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Link
		ECS::Entity* player = scene->createEntity("Player");

		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("link.png");
		auto body = player->addComponent<ECS::BoxBody>();
		auto tComp = player->addComponent<ECS::TestComponent>();

		// Ground
		ECS::Entity* ground = scene->createEntity("Ground");
	
		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("ground.png");
		auto grBody = ground->addComponent<ECS::EdgeBody>();

		// Ball
		ECS::Entity* ball = scene->createEntity("Ball");

		auto trBall = ball->addComponent<ECS::Transform>();
		auto imBall = ball->addComponent<ECS::Image>("ball.png");
		auto ballBody = ball->addComponent<ECS::CircleBody>();

	
	// 3.- Init
	scene->init();

	// 4.- Components settings

		// Player
		tr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);
		tr->setScale(0.35f, 0.35f);
		body->setBodyType((int)ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		body->setBounciness(0.5f);
		//body->setTrigger(true);

		// Ground
		grTr->setScale(1, 1);
		grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 1.2f);
		grBody->addOffSet(0, -1);

		// Ball
		trBall->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 4);
		trBall->setScale(0.25f, 0.25f);
		ballBody->setBodyType((int)ECS::PhysicBody::BODY_TYPE::DYNAMIC);
		ballBody->setBounciness(0.5f); 

	// 5.- Start

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}

void Game::flappyBird() {

	Renderer::RendererManager* renderer = Renderer::RendererManager::instance();

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Fondo
		ECS::Entity* background = scene->createEntity("fondo");
		auto bgTr = background->addComponent<ECS::Transform>();
		auto bg = background->addComponent<ECS::Image>("fondo.png");
		background->addComponent<ECS::Script>()->Initialise("Parallax");

		// Pollo
		ECS::Entity* player = scene->createEntity("Player");
		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("pollo.png");
		auto body = player->addComponent<ECS::BoxBody>();

		// Tuberias
		ECS::Entity* tuberia = scene->createEntity("Tuberia");
		auto tubTr = tuberia->addComponent<ECS::Transform>();
		auto tubIm = tuberia->addComponent<ECS::Image>("tuberiaFlappyBird.png");
		auto tubBody = tuberia->addComponent<ECS::BoxBody>();

		ECS::Entity* tuberiaInv = scene->createEntity("Tuberia");
		auto tubTrInv = tuberiaInv->addComponent<ECS::Transform>();
		auto tubImInv = tuberiaInv->addComponent<ECS::Image>("tuberiaFlappyBird.png");
		auto tubBodyInv = tuberiaInv->addComponent<ECS::BoxBody>();

		player->addComponent<ECS::Script>()->Initialise("FlappyBirdJump");
		player->addComponent<ECS::Script>()->Initialise("RestartOnCollision");

		tuberia->addComponent<ECS::Script>()->Initialise("MovimientoTuberia");
		tuberiaInv->addComponent<ECS::Script>()->Initialise("MovimientoTuberia");

	// 3.- Init
	scene->init();

	// 4.- Components settings

		// Fondo
	std::map<std::string, std::string> trMap;
	trMap["position_"] = "(" + std::to_string(renderer->getWidth() / 2) + "," + std::to_string(renderer->getHeight() / 2) + ")";
	trMap["scale_"] = "(1.6f, 1.6f)";

	ClassReflection::instance()->ReflectComponent("Transform", bgTr, trMap);

		//bgTr->setScale(1.6f, 1.6f);
		//bgTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);

		// Pollo
		tr->setPosition(200 / 2, renderer->getHeight() / 2);
		tr->setScale(0.35f, 0.35f);

		body->setBounciness(0.5f);
		body->setRotationFreezed(true);

		// Tuberias
		tubTrInv->setRotation(180);
		tubTrInv->setPosition(1400, 150);
		tubTr->setPosition(1400, renderer->getHeight() - 150);

		tubBody->setBodyType((int)ECS::PhysicBody::BODY_TYPE::KINEMATIC);
		tubBody->setGravityScale(0);

		tubBodyInv->setBodyType((int)ECS::PhysicBody::BODY_TYPE::KINEMATIC);
		tubBodyInv->setGravityScale(0);

	// 5.- Start

	//Yojhan - Scripts
	//player->addComponent<ECS::Script>()->Initialise("printGameObjectName.json");
	//player->addComponent<ECS::Script>()->Initialise("printWhenKeyDown.json");
	//player->addComponent<ECS::Script>()->Initialise("moveUpWhenKeyDown.json");

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();
}

void Game::readScene(std::string const& sceneName)
{
	std::ifstream fileStream("Scenes/" + sceneName);

	if (!fileStream.good())
	{
		Console::Output::PrintError("Scene loading", "Cannot open scene <" + sceneName + ">");

		return;
	}
	json file = json::parse(fileStream);


	scene = sceneManager->createScene(file["name"].get<std::string>());


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

		jsonarray scripts = obj["scripts"].get<jsonarray>();
		for (auto& script : scripts) {

			entity->addComponent<ECS::Script>()->Initialise(script.get<std::string>());
		}
	}


	jsonarray overlays = file["overlays"].get<jsonarray>();

	for (auto& overlay : overlays) {

		processOverlay(overlay, nullptr);
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



	scene->init();

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();
}



void Game::processOverlay(nlohmann::json& overlay, ECS::OverlayElement* parent)
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

		processOverlay(child, overlayElement);
	}
}
