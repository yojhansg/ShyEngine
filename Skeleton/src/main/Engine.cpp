#include "Engine.h"

#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <ResourcesManager.h>
#include <ContactListener.h>
#include <Scene.h>
#include <SDLUtils.h>
#include <EngineTime.h>
#include <RenderManager.h>
#include <OverlayManager.h>

#include <Component.h>

#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ComponentFactory.h>
#include <Scripting/ScriptManager.h>
#include <Scripting/ScriptFunctionality.h>

#include "Game.h"
#include <iostream>

#include <chrono>

using namespace std::chrono;

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

Engine::Engine() {

	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr; contactListener = nullptr;
	soundManager = nullptr; sceneManager = nullptr; engineTime = nullptr; resourcesManager = nullptr;
	scriptManager = nullptr;
	scriptFunctionality = nullptr;
	componentFactory = nullptr;
	renderManager = nullptr;
	overlayManager = nullptr;
}

void Engine::init() {

	if (ECS_Version != ECSfunc_Version) {
		std::cout << "Warning: La version del motor no coincide con la version de las funciones de los scripts" << std::endl;
	}

	sceneManager = ECS::SceneManager::init();
	rendererManager = Renderer::RendererManager::init("MyEngine Window", WIN_WIDTH, WIN_HEIGHT);
	physicsManager = Physics::PhysicsManager::init(Utilities::Vector2D(0, 9.81f));
	contactListener = ECS::ContactListener::init();
	inputManager = Input::InputManager::init();
	soundManager = SoundManager::SoundManager::init();
	resourcesManager = Resources::ResourcesManager::init();
	engineTime = Utilities::EngineTime::init();
	scriptManager = Scripting::ScriptManager::init();
	scriptFunctionality = Scripting::ScriptFunctionality::init();
	componentFactory = ComponentFactory::init();
	renderManager = ECS::RenderManager::init();
	overlayManager = ECS::OverlayManager::init();

	//TODO: Se guardan algunos punteros a managers que realmente no son necesarios

	physicsManager->enableDebugDraw(true);

	Game g(sceneManager);
	g.initScenes();
}

void Engine::update() {

	SDL_Event event;

	steady_clock::time_point beginTime = high_resolution_clock::now();
	steady_clock::time_point physicsTime = beginTime;
	steady_clock::time_point startTime = beginTime;

	while (true) {

		// Scene
		auto scene = sceneManager->getActualScene();
		if (scene == nullptr) break;

		// Input
		if (!inputManager->handleInput(event)) break;

		// FixedUpdate
		while (physicsTime < beginTime) {

			scene->fixedUpdate(engineTime->fixedDeltaTime);
			physicsManager->fixedUpdate(engineTime->fixedDeltaTime);

			physicsTime += std::chrono::milliseconds((int) (engineTime->fixedDeltaTime * 1000));
		}

		// Update
		scene->update(engineTime->deltaTime);

		// LateUpdate
		scene->lateUpdate(engineTime->deltaTime);

		// Render
		rendererManager->clearRenderer(Utilities::createColor(0x835CF3FF));
		//scene->render();
		renderManager->Render();
		physicsManager->debugDraw();
		overlayManager->Render();


		rendererManager->presentRenderer();

		// Remove dead entities
		scene->removeEntities();

		// Handling physics bodies
		physicsManager->handleBodies();

		// Change scene if necessary
		sceneManager->manageScenes();

		// Time
		steady_clock::time_point endTime = high_resolution_clock::now();

		duration<float, std::milli> timeSinceStart = endTime - startTime;
		duration<double, std::milli> frameTime = endTime - beginTime;

		engineTime->timeSinceStart = timeSinceStart.count();
		engineTime->deltaTime = frameTime.count() * 0.001f;
		engineTime->frames++;

		beginTime = endTime;

	}
}

void Engine::close() {

	sceneManager->close();

}
