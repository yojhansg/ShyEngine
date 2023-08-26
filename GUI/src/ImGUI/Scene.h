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

	class Entity;
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

		std::map<int, Entity*> entities;
		std::vector<Entity*> overlays;

		bool dragging;
		Entity* selectedEntity;


		static bool CompareEntitiesRenderOrder(Entity* a, Entity* b);

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

		Entity* AddEntity(std::string path);
		void AddEntity(Entity* entity);
		void AddEntityChildsToScene(Entity* entity);

		Entity* AddOverlay(std::string path);
		void AddOverlay(Entity* overlay);
		void AddOverlayChildsToScene(Entity* entity);

		std::map<int, Entity*>& GetEntities();
		std::vector<Entity*>& GetOverlays();

		std::string GetSceneName();
		Entity* GetSelectedEntity();
		void SetSelectedEntity(Entity* entity);

		void SaveScene(const std::string& sceneName);
		void LoadScene(const std::string& sceneName);

		void RenderChildEntities(Entity* entity);
		void RenderEntities();
		void RenderFrame();
		void RenderUI();

		void HandleInput(SDL_Event* event) override;
		void Behaviour() override;
		void ReceiveAssetDrop(Asset& asset) override;

		ImVec2 MousePositionInScene();
		bool IsMouseHoveringEntity(Entity* entity);

		std::string GetPath();
		std::string ToJson();

	};

}