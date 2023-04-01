#pragma once

/*
* A class to test if ECS works
*/

#include <string>
#include <vector>

#include <json.hpp>

namespace nlohmann {
	using jsonarray = std::vector<json>;

}

namespace ECS {
	class SceneManager;
	class Scene;


	class OverlayElement;
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


	void processOverlay(nlohmann::json& overlay, ECS::OverlayElement* parent);
};