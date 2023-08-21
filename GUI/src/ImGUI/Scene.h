#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>

#include "Window.h"

#define SCENE_WIN_WIDTH_RATIO 0.6f
#define SCENE_WIN_HEIGHT_RATIO 0.7f

struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;
struct ImVec2;


namespace ShyEditor {

	class GameObject;
	class Camera;

	class Scene : public Window {

	private:

		SDL_Renderer* renderer = nullptr;
		SDL_Texture* targetTexture = nullptr;

		Camera* camera;

		std::string path;

		std::unordered_map<int, GameObject*> gameObjects;
		GameObject* selectedGameObject;

		bool mouseInsideWindow(ImVec2 mousePos);
		bool mouseInsideGameObject(GameObject* go, ImVec2 mousePos);

		ImVec2 getMousePosInsideScene(ImVec2 mousePos);

		static bool compareGameObjectsRenderOrder(GameObject* a, GameObject* b);

	public:

		Scene();
		virtual ~Scene();

		GameObject* addGameObject(std::string path);
		void addGameObject(GameObject* go);

		std::unordered_map<int, GameObject*> getGameObjects();

		GameObject* getSelectedGameObject();
		void setSelectedGameObject(GameObject* go);

		void renderChildGameObjects(GameObject* go);
		void renderGameObjects();
		void renderFrame();

		void saveScene(std::string path);
		void loadScene(std::string path);

		
		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;

		std::string getPath();
		std::string toJson();

	};

}