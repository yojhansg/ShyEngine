#include "Engine.h"

#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <Scene.h>

#include <Entity.h>
#include <Transform.h>

#include <iostream>
#include <memory>


Engine::Engine() {
	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr; soundManager = nullptr; sceneManager = nullptr;
}

void Engine::init() {

	sceneManager = ECS::SceneManager::init();
	physicsManager = PhysicsManager::PhysicsManager::init();
	rendererManager = RendererManager::RendererManager::init("PhosphorusEngine Window", WIN_WIDTH, WIN_HEIGHT);
	inputManager = InputManager::InputManager::init();
	soundManager = SoundManager::SoundManager::init();

	physicsManager->configureScene();

	ECS::Scene* s = sceneManager->createScene();

	ECS::Entity* e = s->createEntity("Jugador");
	e->addComponent<ECS::Transform>();

	s->init();
}

void Engine::update() {

	SDL_Event event;

	int frames = 0;
	Uint32 startTime = SDL_GetTicks();
	Uint32 timeSinceStart = 0;

	while (true) {

		Uint32 sT = SDL_GetTicks();

		auto scene = sceneManager->getActualScene();

		if (scene == nullptr) break;

		// Input
		if (!inputManager->handleInput(event)) break;
		scene->handleInput();

		// Update
		scene->update();

		// FixedUpdate
		physicsManager->fixedUpdate();
		scene->fixedUpdate();

		// LateUpdate
		scene->lateUpdate();

		// Render
		rendererManager->clearRenderer();
		scene->render();
		rendererManager->presentRenderer();

		scene->removeEntities();

		sceneManager->manageScenes();

		// Tiempo
		timeSinceStart = SDL_GetTicks() - startTime;

		if (timeSinceStart > 1000) {
			std::cout << "Frames por segundo: " << frames << std::endl;

			timeSinceStart = 0;
			startTime = SDL_GetTicks();
			frames = 0;
		}
		frames++;

		Uint32 fT = SDL_GetTicks() - sT;

		if (fT < 20)
			SDL_Delay(20 - fT);
	}
}

void Engine::close() {

}
