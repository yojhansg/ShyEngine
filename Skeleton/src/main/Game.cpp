#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Components/Transform.h>
#include <Components/Image.h>
#include <Components/Collider.h>
#include <Components/BoxCollider.h>
#include <Components/CircleCollilder.h>
#include <Components/Rigidbody.h>
#include <Components/TestComponent.h>
#include <FunctionManager/FunctionManager.h>
#include <map>

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
		auto collider = player->addComponent<ECS::Collider>();
		//auto rb = player->addComponent<ECS::Rigidbody>();
		auto tComp = player->addComponent<ECS::TestComponent>();

		// Ground
		ECS::Entity* ground = scene->createEntity("Ground");

		auto grTr = ground->addComponent<ECS::Transform>();
		auto grIm = ground->addComponent<ECS::Image>("ground.png");
		auto grCol = ground->addComponent<ECS::Collider>();

	// 3.- Init
	scene->init();


	// 4.- Components settings

		// Player
		tr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 3);
		tr->setScale(0.35f, 0.35f);
		
		collider->setBodyType(ECS::Collider::DYNAMIC);
		
		// Ground
		grTr->setScale(0.75f, 0.75f);
		grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight());


	// Scripting
	//tr->CallMethod("HolaMamasita", { Scripting::Variable::Component(tr), Scripting::Variable::Float(2)});


	// 5.- Start
	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}
