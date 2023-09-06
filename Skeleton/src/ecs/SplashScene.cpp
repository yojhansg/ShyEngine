#include "SplashScene.h"

#include "ResourcesManager.h"
#include "RendererManager.h"
#include "SceneManager.h"

#include "Components/Image.h"
#include "Components/Transform.h"
#include "Components/Overlay.h"
#include "Components/OverlayImage.h"
#include "Scene.h"
#include "Entity.h"
#include "SDL_video.h"

#define Background "ShyLogo.png"

namespace ECS {


	// ------------ SplashSceneManager --------------

	SplashSceneManager* SplashSceneManager::instance = nullptr;

	SplashSceneManager* SplashSceneManager::Instance() {

		if (instance == nullptr)
			instance = new SplashSceneManager();

		return instance;
	}

	void SplashSceneManager::Release() {
		delete instance;
		instance = nullptr;
	}

	void SplashSceneManager::LoadSplashScreen() {

		Scene* scene = new Scene("SplashScreen");

		Entity* manager = scene->createEntityNoId("Manager");
		manager->addComponent<ECS::SplashScene>();

		auto overlay = manager->addComponent<ECS::Overlay>();

		overlay->SetStreched(0, 0, 0, 0);
		auto img = manager->addComponent<ECS::OverlayImage>();
		img->path = Background;

		scene->init();
		scene->start();

		SceneManager::instance()->scenes.push(scene);
	}

	void SplashSceneManager::LoadConfigurationData(const std::string& initialScene) {
		instance->initialScene = initialScene;
	}


	// ------------ SplashScene --------------

	SplashScene::SplashScene() {
		timer = 0;
		maxTime = 3;
	}

	SplashScene::~SplashScene() {}

	void SplashScene::update(float dt) {

		timer += dt;

		if (timer > maxTime) {

			SceneManager::instance()->ChangeScene(SplashSceneManager::Instance()->initialScene, SceneManager::POP_AND_PUSH);
		}
	}

}
