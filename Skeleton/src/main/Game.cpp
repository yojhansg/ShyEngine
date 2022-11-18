#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Transform.h>
#include <Image.h>
#include <Collider.h>
#include <Rigidbody.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	firstScene = secondScene = nullptr;
}

void Game::initScenes() {

	RendererManager::RendererManager* renderer = RendererManager::RendererManager::instance();

	// Default scene
	firstScene = sceneManager->createScene("Default scene");

	// Player
	/*ECS::Entity* player = firstScene->createEntity("Player");

	auto tr = player->addComponent<ECS::Transform>();
	auto im = player->addComponent<ECS::Image>("link.png");
	auto collider = player->addComponent<ECS::Collider>();

	auto rb = player->addComponent<ECS::Rigidbody>();
	rb->setBodyType(ECS::Rigidbody::DINAMIC);

	tr->setPosition(renderer->getWidth() / 2 - im->getWidth() / 2, renderer->getHeight() / 2 - im->getHeight() / 2);*/

	ECS::Entity* player = firstScene->createEntity("Player");

	auto playerTr = player->addComponent<ECS::Transform>();
	auto playerIm = player->addComponent<ECS::Image>("blue_pixel.png");
	auto playerCollider = player->addComponent<ECS::Collider>();

	auto playerRb = player->addComponent<ECS::Rigidbody>();
	playerRb->setBodyType(ECS::Rigidbody::DINAMIC);

	playerIm->setSize(100, 100);

	playerTr->setPosition(renderer->getWidth() / 2 - playerIm->getWidth() / 2, renderer->getHeight() / 2 - playerIm->getHeight() * 10);

	// Ground
	ECS::Entity* ground = firstScene->createEntity("Ground");

	auto groundTr = ground->addComponent<ECS::Transform>();
	auto groundIm = ground->addComponent<ECS::Image>("red_pixel.png");

	groundIm->setSize(1000, 100);

	groundTr->setPosition(renderer->getWidth() / 2 - groundIm->getWidth() / 2, renderer->getHeight() - groundIm->getHeight() * 2);



	sceneManager->changeScene(firstScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

	// First scene
	/*secondScene = sceneManager->createScene("First scene");

	ECS::Entity* rock = secondScene->createEntity("Rock");
	rock->addComponent<ECS::Image>();

	sceneManager->changeScene(secondScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();*/
}
