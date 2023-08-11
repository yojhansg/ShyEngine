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
#include <PrefabManager.h>
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

	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr;
	sceneManager = nullptr; engineTime = nullptr; renderManager = nullptr;
	overlayManager = nullptr;
}

bool Engine::init() {

	DataLoader data = DataLoader::Load("clock");

	data.windowSize.set(1920, 1080);

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

	sceneManager = ECS::SceneManager::init();
	rendererManager = Renderer::RendererManager::init(data.windowTitle, data.windowSize.getX(), data.windowSize.getY(), data.vsync);
	physicsManager = Physics::PhysicsManager::init(data.gravity);
	renderManager = ECS::RenderManager::init();
	inputManager = Input::InputManager::init(data.closeWithEscape);  
	engineTime = Utilities::Time::init(); 
	Resources::ResourcesManager::init();
	ECS::ContactListener::init(); 
	ECS::PrefabManager::init("prefabs");
	Sound::SoundManager::init();
	Scripting::ScriptManager::init();
	Scripting::ScriptFunctionality::init();
	ComponentFactory::init();

	overlayManager = ECS::OverlayManager::init(data.debugFrameRate, data.timeToDoubleClick, data.timeToHoldClick); //TODO: debug frame rate 

	// Maximun size = 64x64 pixels
	rendererManager->SetWindowIcon(data.windowIcon);

	physicsManager->enableDebugDraw(data.debugPhysics);

	Game(sceneManager).initScenes();

	/*sceneManager->ChangeScene(data.initialScene, (int)ECS::SceneManager::PUSH);
	sceneManager->manageScenes();*/

	if (data.useSplashScreen) {
		sceneManager->SplashScreen();
	}

	Scripting::ScriptFunctionality::instance()->Camera_SetPosition({ 0, 0 });
	Scripting::ScriptFunctionality::instance()->Camera_SetScale(1.f);

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
		rendererManager->clearRenderer(Utilities::Color(131, 92, 243));
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
	sceneManager->close();
}
