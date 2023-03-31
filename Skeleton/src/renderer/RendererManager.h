#pragma once

#include <Singleton.h>
#include <string>
#include <SDL.h>

//TODO: no incluir aqui SDL

namespace Renderer {

	class RendererManager : public Utilities::Singleton<RendererManager> {

		friend Singleton<RendererManager>;

	public:

		~RendererManager();

		SDL_Window* getWindow();
		SDL_Renderer* getRenderer();

		void clearRenderer(SDL_Color bg = SDL_Color());
		void presentRenderer();

		int getWidth();
		int getHeight();

		void toggleFullScreen();
		void showCursor(bool show);
		void resizeWindow(int w, int h);
		void renameWindow(const std::string& name);
		void repositionWindow(int x, int y);

	private:

		RendererManager();
		RendererManager(const std::string& windowTitle, int width, int height);

		void initSDL();
		void closeSDL();

		std::string windowTitle;
		int width; 
		int height;

		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the renderer
	};
}