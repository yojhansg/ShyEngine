#include "SceneManager.h"
#include "Scene.h"

namespace ECS {

	SceneManager::SceneManager() {
		newScene = nullptr;
		mode = PUSH;
		change = false;
	}

	SceneManager::~SceneManager() {
		removeAllScenes();
	}

	Scene* SceneManager::getActualScene() {
		return scenes.top();
	}

	int SceneManager::getNumberOfScenes() {
		return scenes.size();
	}

	Scene* SceneManager::createScene(const std::string& sce_name) {
		return new Scene(sce_name);
	}

	void SceneManager::changeScene(Scene* sce, LOAD_MODE m) {
		newScene = sce;
		mode = m;
		change = true;
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

		scenes.push(newScene);
		scenes.top()->init();
	}

	void SceneManager::popScene() {
		if (scenes.size() > 0) {
			scenes.top()->onSceneDown();
			delete scenes.top();
			scenes.pop();

			if (scenes.size() > 0) scenes.top()->onSceneUp();
		}
	}

	void SceneManager::popAndPushScene() {
		if (scenes.size() > 0) {
			scenes.top()->onSceneDown();
			delete scenes.top();
			scenes.pop();

			scenes.push(newScene);
			scenes.top()->init();
		}
	}

	void SceneManager::removeAllScenes() {
		auto size = scenes.size();
		for (int i = 0; i < size; i++) popScene();
	}

}
