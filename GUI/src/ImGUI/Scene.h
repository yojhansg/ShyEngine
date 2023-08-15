#pragma once
#include <vector>
#include <iostream>
#include "Window.h"

union SDL_Event;

struct SDL_Renderer;
struct SDL_Texture;
struct ImVec2;
class Camera;
class string;
class ImGUIManager;

namespace PEditor {
	class GameObject;
};

#define SCENE_WIN_WIDTH_RATIO 0.6f
#define SCENE_WIN_HEIGHT_RATIO 0.7f

namespace PEditor {
	class Scene : public Window
	{
	private:

		SDL_Renderer* renderer = nullptr;
		SDL_Texture* targetTexture = nullptr;

		float gameSizeX;
		float gameSizeY;

		Camera* camera;

		std::vector<PEditor::GameObject*> gameObjects;
		GameObject* selectedGameObject;

		bool mouseInsideWindow(ImVec2 mousePos);
		bool mouseInsideGameObject(GameObject* go, ImVec2 mousePos);

		ImVec2 getMousePosInsideScene(ImVec2 mousePos);

		static bool compareGameObjectsRenderOrder(GameObject* a, GameObject* b);

	public:

		Scene();
		virtual ~Scene();

		void addGameObject(std::string path);
		std::vector<PEditor::GameObject*> getGameObjects();

		GameObject* getSelectedGameObject();
		void setSelectedGameObject(GameObject* go);

		void renderGameObjects();
		void renderFrame();

		void saveScene();
		void loadScene();

		virtual void update();
		virtual void handleInput(SDL_Event* event);
		virtual void render();

		std::string toJson();
	};

}