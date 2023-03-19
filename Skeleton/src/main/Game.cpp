#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Components/Transform.h>
#include <Components/Image.h>
#include <Components/BoxCollider.h>
#include <Components/CircleCollilder.h>
#include <Components/Rigidbody.h>
#include <Components/TestComponent.h>
#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ClassReflection.h>
#include <map>
#include <iostream>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();
}

void Game::firstScene() {
	RendererManager::RendererManager* renderer = RendererManager::RendererManager::instance();

	// 1.- Scene
	scene = sceneManager->createScene("Default scene");

	// 2.- Entities

		// Player
		ECS::Entity* player = scene->createEntity("Player");

		auto tr = player->addComponent<ECS::Transform>();
		auto im = player->addComponent<ECS::Image>("kj.png");
		auto collider = player->addComponent<ECS::CircleCollilder>();

		//auto rb = player->addComponent<ECS::Rigidbody>();
		auto tComp = player->addComponent<ECS::TestComponent>();

		// Ground
		/*ECS::Entity* ground = scene->createEntity("Ground");

		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("ground.png");
		auto grCol = ground->addComponent<ECS::Collider>();*/

	// 3.- Init
	scene->init();


	// 4.- Components settings



	std::map<std::string, std::string> map;
	map["x"] = "3";
	map["miVector2d"] = "1, 89";
	ClassReflection::ReflectTransform(tr, map);

	std::cout << tr->miVector2d << std::endl;


		// Player
		tr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);
		tr->setScale(0.5f, 0.5f);

		collider->setRotationFreezed(true);
		//collider->setRadius(2);
		//collider->addOffSet(collider->getSize().getX() / 2, collider->getSize().getY() / 2);

		/*rb->setBodyType(ECS::Rigidbody::DINAMIC);
		rb->setGravityScale(0);*/

		// Ground
		//grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight());


	// Scripting
	//tr->CallMethod("HolaMamasita", { Scripting::Variable::Component(tr), Scripting::Variable::Float(2)});


	// 5.- Start
	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}
