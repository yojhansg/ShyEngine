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
		auto im = player->addComponent<ECS::Image>("link.png");
		auto body = player->addComponent<ECS::BoxBody>();

	// Ground
	ECS::Entity* ground = scene->createEntity("Ground");

		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("ground.png");
		auto grBody = ground->addComponent<ECS::EdgeBody>();

	std::map<std::string, CallableFunction> funciones;
	FunctionManager::CreateFunctionMap(funciones);

	// Ball
	ECS::Entity* ball = scene->createEntity("Ball");

		auto trBall = ball->addComponent<ECS::Transform>();
		auto imBall = ball->addComponent<ECS::Image>("ball.png");
		auto ballBody = ball->addComponent<ECS::CircleBody>();
		auto tComp = ball->addComponent<ECS::TestComponent>();



	// 3.- Init
	scene->init();

	std::map<std::string, std::string> map;
	map["x"] = "3";
	map["miVector2d"] = "1, 89";
	ClassReflection::ReflectTransform(tr, map);

	std::cout << tr->x << std::endl;
	std::cout << tr->miVector2d << std::endl;

	// Scripting


	auto pm = Physics::PhysicsManager::instance();

	pm->addCollisionLayer("Player");
	pm->addCollisionLayer("Ground");
	pm->addCollisionLayer("Ball");

	pm->setCollisionBetweenLayers("Ball", "Player", false);

	// 4.- Components settings

		// Player
		tr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);
		tr->setScale(0.35f, 0.35f);

		body->setBodyType(ECS::PhysicsBody::DYNAMIC);
		body->setBounciness(0.5f);
		body->setGravityScale(0.6f);
		body->setCollisionLayer("Player");

		// Ground
		grTr->setScale(1, 1);
		grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 1.2f);
		grBody->addOffSet(0, -1);
		grBody->setCollisionLayer("Ground");

		//// Ball
		trBall->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 4);
		trBall->setScale(0.25f, 0.25f);

		ballBody->setBodyType(ECS::PhysicsBody::DYNAMIC);
		ballBody->setBounciness(0.5f);
		ballBody->setCollisionLayer("Ball");

	// 5.- Start


	//Yojhan - Scripts
	player->addComponent<ECS::Script>()->Initialise("printGameObjectName.json");
	player->addComponent<ECS::Script>()->Initialise("printWhenKeyDown.json");
	player->addComponent<ECS::Script>()->Initialise("moveUpWhenKeyDown.json");


	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}
