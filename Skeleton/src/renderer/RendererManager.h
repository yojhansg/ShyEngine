#pragma once

#include <Singleton.h>
#include <Vector2D.h>

#include <string>
#include <SDL.h>

namespace RendererManager {

	class RendererManager : public Singleton<RendererManager> {

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

		SDL_Color createColor(unsigned long num);
		SDL_Color createColor(std::string strnum);

		SDL_Rect createRect(Vector2D& pos, float w, float h);
		SDL_Rect createRect(float x, float y, float w, float h);

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

	// A macro to simplify the way to access the instance

	inline RendererManager& rm() {
		return *RendererManager::instance();
	}

}