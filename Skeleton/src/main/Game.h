#pragma once

/*
* A class to test if ECS works
*/

#include <string>

namespace ECS {
	class SceneManager;
	class Scene;
}

class Game {

public:

	Game(ECS::SceneManager* sm);
	
	void initScenes();

private:

	ECS::SceneManager* sceneManager;

	ECS::Scene* scene;

	void firstScene();

	void flappyBird();

	void readScene(std::string const& sceneName);

};