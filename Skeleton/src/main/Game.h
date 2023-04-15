#pragma once

/*
* A class to test if ECS works
*/

#include <string>
#include <vector>

#include <json.hpp>


namespace ECS {
	class SceneManager;
	class Scene;
	class Overlay;
}

class Game {

public:

	Game(ECS::SceneManager* sm);
	
	void initScenes();

private:

	ECS::SceneManager* sceneManager;

	ECS::Scene* scene;

	void firstScene();

};