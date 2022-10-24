#include "Game.h"

#include <SceneManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Transform.h>
#include <Image.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	defaultScene = firstScene = secondScene = nullptr;
}

void Game::initScenes() {

	// Default scene
	defaultScene = sceneManager->createScene("Default scene");

	ECS::Entity* player = defaultScene->createEntity("Player");
	player->addComponent<ECS::Transform>();

	sceneManager->changeScene(defaultScene, ECS::SceneManager::PUSH);

	//// First scene
	//firstScene = sceneManager->createScene("First scene");

	//ECS::Entity* rock = firstScene->createEntity("Rock");
	//rock->addComponent<ECS::Image>();

	//// Second scene 
	//secondScene = sceneManager->createScene("Second scene");

	//ECS::Entity* water = secondScene->createEntity("Water");
	//water->addComponent<ECS::Image>();
}
