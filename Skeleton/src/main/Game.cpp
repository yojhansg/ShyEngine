#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Transform.h>
#include <Image.h>
#include <Collider.h>
#include <Rigidbody.h>
#include <TestComponent.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();
}

void Game::firstScene() {
	RendererManager::RendererManager* renderer = RendererManager::RendererManager::instance();

	// Default scene
	scene = sceneManager->createScene("Default scene");

	// Player
	ECS::Entity* player = scene->createEntity("Player");

	auto im = player->addComponent<ECS::Image>("link.png");
	auto tr = player->addComponent<ECS::Transform>();
	auto collider = player->addComponent<ECS::Collider>();
	auto tComp = player->addComponent<ECS::TestComponent>();


	/*auto rb = player->addComponent<ECS::Rigidbody>();
	rb->setBodyType(ECS::Rigidbody::DINAMIC);*/

	// Ground
	/*ECS::Entity* ground = scene->createEntity("Ground");

	auto grTr = ground->addComponent<ECS::Transform>();
	auto grIm = ground->addComponent<ECS::Image>("ground.png");
	auto grCol = ground->addComponent<ECS::Collider>();*/

	// Init
	scene->init();

	// Player settings
	tr->setPosition(renderer->getWidth() / 2 - im->getWidth() / 2, renderer->getHeight() / 2 - im->getHeight() / 2);
	collider->setSize(200, 200);
	//tr->setRotation(45.0f);

	//// Ground settings
	/*grCol->setTrigger(true);
	grIm->setSize(50, 50);
	grTr->setPosition(renderer->getWidth() / 2 - grIm->getWidth() / 2, renderer->getHeight() - grIm->getHeight());*/

	// START
	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}