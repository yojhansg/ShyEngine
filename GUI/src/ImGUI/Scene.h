#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>

#include "Window.h"

#define SCENE_WIN_WIDTH_RATIO 0.6f
#define SCENE_WIN_HEIGHT_RATIO 0.7f

union SDL_Event;
struct ImVec2;

struct SDL_Renderer;

namespace ShyEditor {

	class GameObject;
	class Camera;

	class Scene : public Window {

	private:

		SDL_Renderer* renderer;

		Camera* camera;

		std::string scenePath;

		std::unordered_map<int, GameObject*> gameObjects;
		GameObject* selectedGameObject;

		static bool CompareGameObjectsRenderOrder(GameObject* a, GameObject* b);

	public:

		Scene();
		virtual ~Scene();

		GameObject* AddGameObject(std::string path);
		void AddGameObject(GameObject* go);

		std::unordered_map<int, GameObject*>& getGameObjects();

		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* go);

		void RenderChildGameObjects(GameObject* go);
		void RenderGameObjects();
		void RenderFrame();
		void RenderUI();

		void saveScene(std::string path);
		void loadScene(std::string path);
		
		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;

		std::string getPath();
		std::string toJson();

	};

}