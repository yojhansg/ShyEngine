#pragma once

/*
* A class to test if ECS works
*/

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

	ECS::Scene* firstScene;
	ECS::Scene* secondScene;
};

