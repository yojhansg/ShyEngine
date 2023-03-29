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

	ECS::Entity* background = scene->createEntity("fondo");
	auto bgTr = background->addComponent<ECS::Transform>();
	auto bg = background->addComponent<ECS::Image>("fondo.png");
	background->addComponent<ECS::Script>()->Initialise("Parallax.json");




	ECS::Entity* player = scene->createEntity("Player");

	auto tr = player->addComponent<ECS::Transform>();
	auto im = player->addComponent<ECS::Image>("pollo.png");
	auto body = player->addComponent<ECS::BoxBody>();

	// Ground
	//ECS::Entity* ground = scene->createEntity("Ground");
	//
	//auto grTr = ground->addComponent<ECS::Transform>();
	//auto grIm = ground->addComponent<ECS::Image>("ground.png");
	//auto grBody = ground->addComponent<ECS::EdgeBody>();


	// Ball
	ECS::Entity* ball = scene->createEntity("Ball");

	auto trBall = ball->addComponent<ECS::Transform>();
	auto imBall = ball->addComponent<ECS::Image>("ball.png");
	auto ballBody = ball->addComponent<ECS::CircleBody>();
	auto tComp = ball->addComponent<ECS::TestComponent>();


	ECS::Entity* tuberia = scene->createEntity("Tuberia");
	auto tubTr = tuberia->addComponent<ECS::Transform>();
	auto tubIm = tuberia->addComponent<ECS::Image>("tuberiaFlappyBird.png");
	auto tubBody = tuberia->addComponent<ECS::BoxBody>();



	ECS::Entity* tuberiaInv = scene->createEntity("Tuberia");
	auto tubTrInv = tuberiaInv->addComponent<ECS::Transform>();
	auto tubImInv = tuberiaInv->addComponent<ECS::Image>("tuberiaFlappyBird.png");
	auto tubBodyInv = tuberiaInv->addComponent<ECS::BoxBody>();

	player->addComponent<ECS::Script>()->Initialise("FlappyBirdJump.json");
	player->addComponent<ECS::Script>()->Initialise("RestartOnCollision.json");

	tuberia->addComponent<ECS::Script>()->Initialise("MovimientoTuberia.json");
	tuberiaInv->addComponent<ECS::Script>()->Initialise("MovimientoTuberia.json");



	// 3.- Init
	scene->init();

	bgTr->setScale(1.6f, 1.6f);
	bgTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);


	auto pm = Physics::PhysicsManager::instance();

	pm->addCollisionLayer("Player");
	pm->setCollisionBetweenLayers("Default", "Player", true);

	// 4.- Components settings

		// Player
	tr->setPosition(200 / 2, renderer->getHeight() / 2);
	tr->setScale(0.35f, 0.35f);

	body->setBodyType(ECS::PhysicsBody::DYNAMIC);
	body->setBounciness(0.5f);
	body->setCollisionLayer("Player");
	body->setRotationFreezed(true);

	tubTrInv->setRotation(180);
	tubTrInv->setPosition(1400, 150);
	tubTr->setPosition(1400, renderer->getHeight() - 150);

	tubBody->setBodyType(ECS::PhysicsBody::KINEMATIC);
	tubBody->setGravityScale(0);

	tubBodyInv->setBodyType(ECS::PhysicsBody::KINEMATIC);
	tubBodyInv->setGravityScale(0);

	// Ground
	//grTr->setScale(1, 1);
	//grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 1.2f);
	//grBody->addOffSet(0, -1);

	//// Ball
	trBall->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 4);
	trBall->setScale(0.25f, 0.25f);

	ballBody->setBodyType(ECS::PhysicsBody::DYNAMIC);
	ballBody->setBounciness(0.5f);

	// 5.- Start


	//Yojhan - Scripts
	//player->addComponent<ECS::Script>()->Initialise("printGameObjectName.json");
	//player->addComponent<ECS::Script>()->Initialise("printWhenKeyDown.json");
	//player->addComponent<ECS::Script>()->Initialise("moveUpWhenKeyDown.json");
	

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}
