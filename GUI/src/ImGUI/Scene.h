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
	class Overlay;

	class Scene : public Window {

	private:



		enum Dir {

			DIR_TOP = 1,
			DIR_BOTTOM = 2,
			DIR_LEFT = 4,
			DIR_RIGHT = 8
		};

		struct {

			int dir = 0;
			Overlay* overlay = nullptr;

			int offset_x = 0;
			int offset_y = 0;

		} selectedOverlay;


		SDL_Renderer* renderer;


		int uiWidth, uiHeight;
		SDL_Texture* uiTexture;

		std::string scenePath;
		std::string sceneName;
		const char* name;

		Camera* sceneCamera;
		int viewMode;

		std::unordered_map<int, GameObject*> gameObjects;



		bool dragging;
		GameObject* selectedGameObject;




		std::vector<GameObject*> overlays;

		static bool CompareGameObjectsRenderOrder(GameObject* a, GameObject* b);


		void ResizeOverlayIfNeccesary();

		void CalculateFrameRect(int& x, int& y, int& w, int& h);




		bool PointInsideHorizontalSegment(int x, int y, int sx, int sy, int w, int thickness);
		bool PointInsideVerticalSegment(int x, int y, int sx, int sy, int h, int thickness);

		bool PointInsideRect(int x, int y, int rx, int ry, int rw, int rh, int thickness = 0);

		void RenderRectangle(int x, int y, int w, int h, int thickness = 1);

		void RenderHorizontalSegment(int x, int y, int w, int thickness = 1);
		void RenderVerticalSegment(int x, int y, int h, int thickness = 1);

	public:

		Scene();
		~Scene();

		GameObject* AddGameObject(std::string path);
		void AddGameObject(GameObject* go);
		void AddChildsToScene(GameObject* go);

		GameObject* AddOverlay(std::string path);


		std::unordered_map<int, GameObject*>& getGameObjects();
		std::vector<GameObject*>& getOverlays();

		std::string getSceneName();

		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* go);

		void saveScene(const std::string& sceneName);
		void loadScene(const std::string& sceneName);

		void RenderChildGameObjects(GameObject* go);
		void RenderGameObjects();
		void RenderFrame();
		void RenderUI();

		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;
		void ReceiveAssetDrop(Asset& asset) override;


		ImVec2 MousePositionInScene();
		bool IsMouseHoveringGameObject(GameObject* gameObject);

		std::string getPath();
		std::string toJson();

	};

}