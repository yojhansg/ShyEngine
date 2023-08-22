#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>

#include "Window.h"

#define SCENE_WIN_WIDTH_RATIO 0.6f
#define SCENE_WIN_HEIGHT_RATIO 0.7f

struct SDL_Renderer;
union SDL_Event;
struct ImVec2;

namespace ShyEditor {

	class GameObject;
	class Camera;

	class Scene : public Window {

	private:

		SDL_Renderer* renderer;

		std::string scenePath;
		std::string sceneName;
		const char* name;

		Camera* camera;
		int viewMode;

		std::unordered_map<int, GameObject*> gameObjects;
		GameObject* selectedGameObject;

		std::vector<GameObject*> overlays;

		static bool CompareGameObjectsRenderOrder(GameObject* a, GameObject* b);

	public:

		Scene();
		~Scene();

		GameObject* AddGameObject(std::string path);
		void AddGameObject(GameObject* go);

		std::unordered_map<int, GameObject*>& getGameObjects();
		std::string getSceneName();

		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* go);

		void saveScene(std::string sceneName);
		void loadScene(std::string sceneFile);

		void RenderChildGameObjects(GameObject* go);
		void RenderGameObjects();
		void RenderFrame();
		void RenderUI();

		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;

		std::string getPath();
		std::string toJson();

		ImVec2 MousePositionInScene();
		bool IsMouseHoveringGameObject(GameObject* gameObject);
	};

}