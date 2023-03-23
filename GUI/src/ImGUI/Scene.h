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

namespace PEditor {
	class Scene : public Window
	{
	private:

		SDL_Renderer* renderer = nullptr;
		SDL_Texture* targetTexture = nullptr;

		Camera* camera;

		std::vector<PEditor::GameObject*> gameObjects;
		GameObject* selectedGameObject;

	public:

		Scene();

		void addGameObject(std::string path);
		std::vector<PEditor::GameObject*> getGameObjects();
		GameObject* getSelectedGameObject();

		bool entityOutsideCamera(ImVec2 pos, float width, float height);

		virtual void handleInput(SDL_Event* event);
		virtual void render();
	};

}