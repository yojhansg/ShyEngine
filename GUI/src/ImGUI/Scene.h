#pragma once

#include <map>
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

		int viewMode;
		Camera* sceneCamera;

		std::map<int, GameObject*> gameObjects;
		std::vector<GameObject*> overlays;

		bool dragging;
		GameObject* selectedGameObject;


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
		void AddGameObjectChildsToScene(GameObject* go);

		GameObject* AddOverlay(std::string path);
		void AddOverlay(GameObject* overlay);
		void AddOverlayChildsToScene(GameObject* go);

		std::map<int, GameObject*>& GetGameObjects();
		std::vector<GameObject*>& GetOverlays();

		std::string GetSceneName();
		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* go);

		void SaveScene(const std::string& sceneName);
		void LoadScene(const std::string& sceneName);

		void RenderChildGameObjects(GameObject* go);
		void RenderGameObjects();
		void RenderFrame();
		void RenderUI();

		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;
		void ReceiveAssetDrop(Asset& asset) override;

		ImVec2 MousePositionInScene();
		bool IsMouseHoveringGameObject(GameObject* gameObject);

		std::string GetPath();
		std::string ToJson();

	};

}