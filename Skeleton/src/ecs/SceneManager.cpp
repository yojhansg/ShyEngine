#include "SceneManager.h"
#include "Scene.h"

#include "SceneLoader.h"
#include "RendererManager.h"
#include "Entity.h"
#include "SplashScene.h"
#include "Components/Overlay.h"
#include "Components/OverlayImage.h"
#include "ResourcesManager.h"

namespace ECS {

	SceneManager::SceneManager() {
		mode = PUSH;
		change = false;
	}

	SceneManager::~SceneManager() {
		removeAllScenes();
	}

	Scene* SceneManager::getCurrentScene() {

		if (scenes.empty()) return nullptr;

		return scenes.top();
	}

	int SceneManager::getNumberOfScenes() {
		return scenes.size();
	}

	std::string SceneManager::GetCurrentScenePath() {
		return currentScenePath;
	}

	Scene* SceneManager::createScene(const std::string& sce_name) {
		return new Scene(sce_name);
	}


	void SceneManager::ChangeScene(std::string scene, int m) {

		currentScenePath = Resources::ResourcesManager::instance()->GetResourcesPath() + scene;
		mode = (LOAD_MODE) m;
		change = true;
	}

	//TODO: llevar esto al fichero splash screen que sino queda feote
	void SceneManager::SplashScreen() {

		Scene* scene = new Scene("SplashScreen");
	
		SplashScene::CreateSplashScreen(scene);

		scene->init();
		scene->start();

		scenes.push(scene);
	}


	void SceneManager::ResetScene() {
		
		ChangeScene(currentScenePath, LOAD_MODE::POP_AND_PUSH);
	}

	void SceneManager::manageScenes() {
		if (change) {
			switch (mode) {

			case PUSH:
				pushScene();
				break;

			case POP:
				popScene();
				break;

			case POP_AND_PUSH:
				popAndPushScene();
				break;

			case CLEAR_AND_PUSH:
				removeAllScenes();
				pushScene();
				break;

			case CLEAR:
				removeAllScenes();
				break;

			default:
				break;
			}
		}

		change = false;
	}

	void SceneManager::pushScene() {

		if (scenes.size() > 0) scenes.top()->onSceneDown();

		auto scene = LoadScene(currentScenePath);

		if (scene != nullptr) {

			scenes.push(scene);
			scenes.top()->start();
		}
	}

	void SceneManager::popScene() {
		if (scenes.size() > 0) {
			scenes.top()->onDestroy();
			delete scenes.top();
			scenes.pop();

			if (scenes.size() > 0) scenes.top()->onSceneUp();
		}
	}

	void SceneManager::popAndPushScene() {
		if (scenes.size() > 0) {
			scenes.top()->onDestroy();
			delete scenes.top();
			scenes.pop();

			scenes.push(LoadScene(currentScenePath));
			scenes.top()->start();
		}
	}

	void SceneManager::removeAllScenes() {
		auto size = scenes.size();
		for (int i = 0; i < size; i++) popScene();
	}


	Scene* SceneManager::LoadScene(std::string const& scenePath)
	{
		Scene* scene = SceneLoader::LoadScene(scenePath);

		if (scene == nullptr)
			return nullptr;

		scene->init();
		return scene;
	}

	void SceneManager::SetScene(ECS::Scene* scene) {
		scenes.push(scene);
		scene->start();
	}

	void SceneManager::EndGame() {
		ChangeScene("", LOAD_MODE::CLEAR);
	}

	void SceneManager::SetCameraScale(float newScale) {
		scenes.top()->cameraScale = newScale;
	}

	void SceneManager::SetCameraPosition(Utilities::Vector2D const& newPosition) {
		scenes.top()->cameraPosition = newPosition;
	}

}
