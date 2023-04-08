#pragma once

#include <Singleton.h>
#include <string>
#include <SDL.h>
#include <Vector2D.h>
#include <Color.h>

//TODO: no incluir aqui SDL

namespace Scripting {
	class ScriptFunctionality;
}

namespace Renderer {


	class RendererManager : public Utilities::Singleton<RendererManager> {

		friend Singleton<RendererManager>;

		friend Scripting::ScriptFunctionality;
	public:

		~RendererManager();

		SDL_Window* getWindow();
		SDL_Renderer* getRenderer();

		void clearRenderer(cColor color = Utilities::Color(255, 255, 255));
		void presentRenderer();

		int getWidth();
		int getHeight();

		void toggleFullScreen();
		void showCursor(bool show);
		void resizeWindow(int w, int h);
		void renameWindow(const std::string& name);
		void repositionWindow(int x, int y);

		void AdjustRectToCamera(int* x, int* y, int* w, int* h);

		Utilities::Vector2D CameraPosition();
		float CameraScale();

		void SetCameraPosition(cVector2D vec);
		void SetCameraScale(float scale);

	private:

		RendererManager();
		RendererManager(const std::string& windowTitle, int width, int height, bool vsync);

		void initSDL(bool vsync);
		void closeSDL();

		std::string windowTitle;
		int width; 
		int height;

		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the renderer

		float cameraScale;
		Utilities::Vector2D cameraPosition;
	};
}