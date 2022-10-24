#include "SceneManager.h"
#include "Scene.h"

namespace ECS {

	SceneManager::SceneManager() {
		newScene = nullptr;
		mode = PUSH;
		change = false;

		auto s = new Scene("Bottom Scene");
		scenes.push(s); 

		n_scenes = 1;
	}

	SceneManager::~SceneManager() {
		removeAllScenes();

		delete scenes.top();
		scenes.pop();
	}

	Scene* SceneManager::getActualScene() {
		return scenes.top();
	}

	int SceneManager::getNumberOfScenes() {
		return n_scenes - 1;
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

		if (n_scenes > 1) scenes.top()->onSceneDown();

		scenes.push(newScene);
		scenes.top()->init();

		n_scenes++;
	}

	void SceneManager::popScene() {
		if (n_scenes > 1) {
			scenes.top()->onSceneDown();
			delete scenes.top();
			scenes.pop();

			n_scenes--;

			if (n_scenes > 1) scenes.top()->onSceneUp();
		}
	}

	void SceneManager::popAndPushScene() {
		if (n_scenes > 1) {
			scenes.top()->onSceneDown();
			delete scenes.top();
			scenes.pop();

			scenes.push(newScene);
			scenes.top()->init();
		}
	}

	void SceneManager::removeAllScenes() {
		for (int i = 0; i < n_scenes - 1; i++) popScene();
	}

}
