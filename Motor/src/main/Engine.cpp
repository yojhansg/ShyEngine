#include "Engine.h"
#include "DataLoader.h"
#include "SplashScene.h"

#include <Scripting/ScriptFunctionality.h>
#include <ECSUtilities/ComponentFactory.h>
#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ClassReflection.h>
#include <Scripting/ScriptManager.h>

#include <ResourcesManager.h>
#include <ContactListener.h>
#include <RendererManager.h>
#include <OverlayManager.h>
#include <PhysicsManager.h>
#include <ConsoleManager.h>
#include <PrefabManager.h>
#include <RenderManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineTime.h>
#include <Component.h>
#include <filesystem>
#include <Scene.h>
#include <chrono>

using namespace std::chrono;

Engine::Engine() {

	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr;
	sceneManager = nullptr; engineTime = nullptr; renderManager = nullptr;
	overlayManager = nullptr; resourcesManager = nullptr;
}

bool Engine::init() {

	DataLoader data = DataLoader::Load("config");

	if (!data.valid) {
		Console::Output::PrintNoFormat("CRITICAL ERROR: The engine couldn't load the game configuration file <config.json>", Console::Color::LightRed);
		return false;
	}

	if (ECS_Version != ECSfunc_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the scripting version. This may cause unexpected behaviour.");
	}

	if (ECS_Version != ECSreflection_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the editor reflection version. This may cause unexpected behaviour.");
	}


	// --------------------------------- MANAGERS -----------------------------------

	sceneManager = ECS::SceneManager::init();
	if (!sceneManager->Valid()) return false;

	rendererManager = Renderer::RendererManager::init(data.windowTitle, data.windowSize.getX(), data.windowSize.getY(), data.vsync, data.fullscreen, data.showCursor, data.bgColor, data.bgAlpha);
	if (!rendererManager->Valid()) return false;

	physicsManager = Physics::PhysicsManager::init(data.gravity, data.layers, data.collisionMatrix);
	if (!physicsManager->Valid()) return false;

	inputManager = Input::InputManager::init(data.closeWithEscape, data.KB_Jump, data.KB_Action, data.KB_movements, data.CT_Jump, data.CT_Action);
	if (!inputManager->Valid()) return false;

	if (!Sound::SoundManager::init(data.frequency, data.channels + 1, data.chunksize)->Valid()) return false;

	renderManager = ECS::RenderManager::init();	

	engineTime = Utilities::Time::init();

	resourcesManager = Resources::ResourcesManager::init();

	// Engine Assets Path
	resourcesManager->SetResourcesPath("Assets\\");

	ECS::SplashSceneManager::Instance()->LoadConfigurationData(data.initialScene);

	ECS::ContactListener::init();

	ECS::PrefabManager::init(data.projectFilePath);

	Scripting::ScriptManager::init();

	Scripting::ScriptFunctionality::init();

	ComponentFactory::init();

	overlayManager = ECS::OverlayManager::init(data.debugFramerate, data.timeToDoubleClick, data.timeToHoldClick);



	// ------- Data configuration ---------

	if (data.windowIcon != "")
		rendererManager->SetWindowIcon(data.resourcesPath + data.windowIcon);
	else
		rendererManager->SetWindowIcon("Assets\\icon.png");

	rendererManager->SetRenderTarget(false);

	physicsManager->enableDebugDraw(data.debugPhysics);

	if (data.useSplashScreen) {
		ECS::SplashSceneManager::LoadSplashScreen();

		// Project Assets Path
		resourcesManager->SetResourcesPath(data.resourcesPath);
	}
	else {
		// Project Assets Path
		resourcesManager->SetResourcesPath(data.resourcesPath);

		sceneManager->ChangeScene(data.initialScene, (int)ECS::SceneManager::PUSH);
		sceneManager->manageScenes();
	}


	return true;
}

void Engine::update() {

	SDL_Event event;

	steady_clock::time_point beginTime = high_resolution_clock::now();
	steady_clock::time_point physicsTime = beginTime;
	steady_clock::time_point startTime = beginTime;

	while (true) {

		// Scene
		auto scene = sceneManager->getCurrentScene();
		if (scene == nullptr) break;

		// Input
		if (!inputManager->handleInput(event)) break;

		// FixedUpdate
		while (physicsTime < beginTime) {

			scene->fixedUpdate(engineTime->fixedDeltaTime);
			physicsManager->fixedUpdate(engineTime->fixedDeltaTime);

			physicsTime += std::chrono::milliseconds((int)(engineTime->fixedDeltaTime * 1000));
		}

		// Update
		scene->update(engineTime->deltaTime);

		// LateUpdate
		scene->lateUpdate(engineTime->deltaTime);

		// Render
		overlayManager->Update();
		rendererManager->SetRenderTarget(false);
		rendererManager->clearRenderer();
		renderManager->Render();
		physicsManager->debugDraw();
		overlayManager->Render();
		rendererManager->presentRenderer();

		// OnDestroy
		scene->onDestroyOnRemovedEntities();

		// Handling physics bodies
		physicsManager->handleBodies();

		// Remove dead entities and components
		scene->handleRemovedEntities();

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

	ECS::SplashSceneManager::Release();
	resourcesManager->close();
	inputManager->close();
	rendererManager->close();
	sceneManager->close();
	physicsManager->close();
}
