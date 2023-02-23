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

	auto tr = player->addComponent<ECS::Transform>();
	auto im = player->addComponent<ECS::Image>("link.png");

	auto collider = player->addComponent<ECS::Collider>();
	collider->setBounciness(0.5f);
	auto rb = player->addComponent<ECS::Rigidbody>();
	rb->setBodyType(ECS::Rigidbody::DINAMIC);
	rb->setGravityScale(2.0f);

	im->setSize(im->getWidth() / 2, im->getHeight() / 2);

	tr->setPosition(renderer->getWidth() / 2 - im->getWidth() / 2, renderer->getHeight() / 2 - im->getHeight() / 2);


	// Ground
	ECS::Entity* ground = scene->createEntity("Ground");

	auto grTr = ground->addComponent<ECS::Transform>();
	auto grIm = ground->addComponent<ECS::Image>("ground.png");
	auto grCol = ground->addComponent<ECS::Collider>();

	grIm->setSize(1000, 100);
	grTr->setPosition(renderer->getWidth() / 2 - grIm->getWidth() / 2, renderer->getHeight() - grIm->getHeight());

	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();
}