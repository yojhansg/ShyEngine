#include "Engine.h"
#include "Game.h"
#include "DataLoader.h"

#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ComponentFactory.h>
#include <ECSUtilities/ClassReflection.h>
#include <Scripting/ScriptFunctionality.h>
#include <Scripting/ScriptManager.h>

#include <ResourcesManager.h>
#include <ContactListener.h>
#include <RendererManager.h>
#include <OverlayManager.h>
#include <PhysicsManager.h>
#include <ConsoleManager.h>
#include <RenderManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineTime.h>
#include <Component.h>
#include <SDLUtils.h>
#include <Scene.h>
#include <chrono>

using namespace std::chrono;

Engine::Engine() {

	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr; contactListener = nullptr;
	soundManager = nullptr; sceneManager = nullptr; engineTime = nullptr; resourcesManager = nullptr;
	scriptManager = nullptr;
	scriptFunctionality = nullptr;
	componentFactory = nullptr;
	renderManager = nullptr;
	overlayManager = nullptr;
}

bool Engine::init() {

	DataLoader data = DataLoader::Load("flappyBird");

	if (!data.valid) {

		Console::Output::PrintNoFormat("CRITICAL ERROR: The engine couldn't load the game configuration file <config.json>", Console::Color::LightRed);
		return false;
	}


	if (ECS_Version != ECSfunc_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the scripting version. This may cause unexpected behaviour");
	}
	
	if (ECS_Version != ECSreflection_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the editor reflection version. This may cause unexpected behaviour");
	}

	//TODO: Se guardan algunos punteros a managers que realmente no son necesarios

	sceneManager = ECS::SceneManager::init();
	rendererManager = Renderer::RendererManager::init(data.windowName, data.windowSize.getX(), data.windowSize.getY());
	physicsManager = Physics::PhysicsManager::init(data.gravity);
	contactListener = ECS::ContactListener::init();
	inputManager = Input::InputManager::init();
	soundManager = SoundManager::SoundManager::init();
	resourcesManager = Resources::ResourcesManager::init();
	engineTime = Utilities::Time::init();
	scriptManager = Scripting::ScriptManager::init();
	scriptFunctionality = Scripting::ScriptFunctionality::init();
	componentFactory = ComponentFactory::init();
	renderManager = ECS::RenderManager::init();
	overlayManager = ECS::OverlayManager::init();

	physicsManager->enableDebugDraw(data.debugPhysics);

	//Game(sceneManager).initScenes();

	sceneManager->ChangeScene(data.initialScene, (int)ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

	return true;
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

		overlayManager->Update();

		// Render
		rendererManager->clearRenderer(Utilities::createColor(0x835CF3FF));
		//scene->render();
		renderManager->Render();
		physicsManager->debugDraw();
		overlayManager->Render();

		rendererManager->presentRenderer();

		// OnDestroy on dead entities and components
		scene->preRemoveEntitiesAndComponents();

		// Handling physics bodies
		physicsManager->handleBodies();

		// Remove dead entities and components
		scene->postRemoveEntitiesAndComponents();

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
