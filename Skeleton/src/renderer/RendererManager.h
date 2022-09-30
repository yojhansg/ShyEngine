#pragma once

#include <string>

class SDL_Surface;
class SDL_Window;

class RendererManager {

	public:
		RendererManager();
		void closeSDL();

	private:

		bool initSDL();

		SDL_Surface* loadImage(std::string path);

		SDL_Window* window;
		SDL_Surface* screenSurface;
};
