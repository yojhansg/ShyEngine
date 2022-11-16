#include "Game.h"

#include <SceneManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Transform.h>
#include <Image.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	firstScene = secondScene = nullptr;
}

void Game::initScenes() {

	// Default scene
	firstScene = sceneManager->createScene("Default scene");

	ECS::Entity* player = firstScene->createEntity("Player");
	player->addComponent<ECS::Transform>();
	player->addComponent<ECS::Image>("link.png");

	sceneManager->changeScene(firstScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

	// First scene
	/*secondScene = sceneManager->createScene("First scene");

	ECS::Entity* rock = secondScene->createEntity("Rock");
	rock->addComponent<ECS::Image>();

	sceneManager->changeScene(secondScene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();*/
}
