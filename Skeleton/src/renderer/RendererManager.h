#pragma once

#include <Singleton.h>
#include <string>
#include <Vector2D.h>
#include <Color.h>
#include <EditorExport.h>

class SDL_Window;
struct SDL_Renderer;
class SDL_Surface;
struct SDL_Texture;

namespace Scripting {
	class ScriptFunctionality;
}

namespace Renderer {

	EditorManager RendererManager : public Utilities::Singleton<RendererManager> {

		friend Singleton<RendererManager>;

		friend Scripting::ScriptFunctionality;

	public:

		~RendererManager();

		bool Valid() override;

		SDL_Window* getWindow();
		SDL_Renderer* getRenderer();

		void clearRenderer(cColor color = Utilities::Color(255, 255, 255));
		void presentRenderer();
		void CopyTargetTextureContent();

		// Camera methods
		void AdjustRectToCamera(int* x, int* y, int* w, int* h);

		Utilities::Vector2D CameraPosition();
		float CameraScale();

		void SetCameraPosition(cVector2D vec);
		void SetCameraScale(float scale);

		// Render target (In case we need to run the engine inside the editor)
		void SetRenderTarget(bool renderOnTarget);

	publish:

		int getWidth();
		int getHeight();

		void toggleFullScreen();
		void showCursor(bool show);
		void resizeWindow(int w, int h);
		void renameWindow(const std::string& name);
		void repositionWindow(int x, int y);
		bool SetWindowIcon(const std::string& path);

	private:

		RendererManager();
		RendererManager(const std::string& windowTitle, int width, int height, bool vsync);

		bool initSDL(bool vsync);
		void closeSDL();

		int width; 
		int height;

		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the renderer

		std::string windowTitle;

		SDL_Surface* icon;

		SDL_Texture* targetTexture;

		float cameraScale;
		Utilities::Vector2D cameraPosition;

		bool valid;
	};
}