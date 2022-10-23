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
		if (scenes.empty()) return nullptr;

		return scenes.top();
	}

	int SceneManager::getNumberOfScenes() {
		return scenes.size();
	}

	Scene* SceneManager::createScene(const std::string& sce_name) {
		Scene* s = new Scene(sce_name);

		scenes.push(s);

		return s;
	}

	void SceneManager::removeActualScene() {
		if (!scenes.empty()) {
			scenes.top()->onSceneDown();
			delete scenes.top();
			scenes.pop();
		}
	}

	void SceneManager::changeScene(Scene* sce, LOAD_MODE m) {
		newScene = sce;
		mode = m;
		change = true;
	}

	bool SceneManager::manageScenes() {
		if (change) {
			switch (mode) {

			case PUSH:
				pushScene();
				break;

			case POP:
				popScene();
				break;

			case POP_AND_PUSH:
				popScene();
				scenes.push(newScene);
				scenes.top()->onSceneUp();
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

		return scenes.empty();
	}

	void SceneManager::pushScene() {

		if (!scenes.empty()) scenes.top()->onSceneDown();

		scenes.push(newScene);
		scenes.top()->onSceneUp();
	}

	void SceneManager::popScene() {
		removeActualScene();
	}

	void SceneManager::removeAllScenes() {
		for (int i = 0; i < scenes.size(); i++) popScene();
	}

}
