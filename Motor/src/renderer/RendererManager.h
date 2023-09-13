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

using cString = std::string const&;

namespace Renderer {

	EditorManager RendererManager : public Utilities::Singleton<RendererManager> {

		friend Singleton<RendererManager>;

		friend Scripting::ScriptFunctionality;

	public:

		~RendererManager();

		bool Valid() override;

		SDL_Window* getWindow();
		SDL_Renderer* getRenderer();

		void clearRenderer();
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
		void renameWindow(cString name);
		void repositionWindow(int x, int y);
		void centerWindow();
		void setWindowBordered(bool border);
		bool SetWindowIcon(cString path);

	private:

		RendererManager();
		RendererManager(cString windowTitle, int width, int height, bool vsync, bool fullscreen, bool showcursor, cString bgColor, int bgAlpha);

		bool initSDL(bool vsync, bool fullscreen, bool showcursor);
		void closeSDL();

		int width;
		int height;

		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the renderer

		std::string windowTitle;

		Utilities::Color bgColor;
		int bgAlpha;

		SDL_Surface* icon;

		SDL_Texture* targetTexture;

		float cameraScale;
		Utilities::Vector2D cameraPosition;

		bool valid;
	};
}
