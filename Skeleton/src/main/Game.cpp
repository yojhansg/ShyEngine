#include "Game.h"

#include <SceneManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Transform.h>
#include <RendererManager.h>
#include <Image.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	firstScene = secondScene = nullptr;
}

void Game::initScenes() {

	// Default scene
	firstScene = sceneManager->createScene("Default scene");

	ECS::Entity* player = firstScene->createEntity("Player");
	auto tr = player->addComponent<ECS::Transform>();
	auto im = player->addComponent<ECS::Image>("link.png");

	RendererManager::RendererManager* renderer = RendererManager::RendererManager::instance();

	im->setRotaionPoint(im->getWidth() / 2, im->getHeight() / 2);
	tr->setPosition(renderer->getWidth() / 2 - im->getWidth() / 2, renderer->getHeight() / 2 - im->getHeight() / 2);
	tr->scale(0.5f);

	sceneManager->changeScene(firstScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

	// First scene
	/*secondScene = sceneManager->createScene("First scene");

	ECS::Entity* rock = secondScene->createEntity("Rock");
	rock->addComponent<ECS::Image>();

	sceneManager->changeScene(secondScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();*/
}
