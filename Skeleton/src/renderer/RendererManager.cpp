#include "RendererManager.h"
#include "ConsoleManager.h"
#include "Texture.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Renderer {

	RendererManager::RendererManager() : RendererManager("SDL Window", 600, 400, true, false, true) {}

	RendererManager::RendererManager(const std::string& title, int w, int h, bool vsync, bool fullscreen, bool showcursor) {
		windowTitle = title;
		width = w;  height = h;
		targetTexture = nullptr;
		icon = nullptr;

		valid = initSDL(vsync, fullscreen, showcursor);
	}

	bool RendererManager::Valid() {
		return valid;
	}

	RendererManager::~RendererManager() {

		SDL_FreeSurface(icon);
		icon = nullptr;

		SDL_DestroyTexture(targetTexture);
		targetTexture = nullptr;

		closeSDL();

	}

	bool RendererManager::initSDL(bool vsync, bool fullscreen, bool showcursor) {
		// Initialise SDL
		int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);

		if (sdlInit_ret < 0) {
			Console::Output::PrintError("SDL Initialisation", SDL_GetError());
			return false;
		}

		// Create window
		int fullscreenEnable = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN;
		window = SDL_CreateWindow(windowTitle.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, width, height, fullscreen | SDL_WINDOW_OPENGL);

		if (window == NULL) {
			SDL_Quit();

			Console::Output::PrintError("SDL Initialisation (Window Creation)", SDL_GetError());
			return false;
		}

		// Create the renderer
		int vsyncEnable = vsync ? SDL_RENDERER_PRESENTVSYNC : 0;
		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | vsyncEnable);

		if (window == NULL) {
			SDL_DestroyWindow(window); window = NULL;
			SDL_Quit();

			Console::Output::PrintError("SDL Initialisation (Renderer Creation)", SDL_GetError());
			return false;
		}

		// Initialize SDL_image
		int imgInit_ret = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

		if (imgInit_ret == 0) {
			SDL_DestroyWindow(window); window = NULL;
			SDL_DestroyRenderer(renderer); renderer = NULL;
			SDL_Quit();

			Console::Output::PrintError("SDL Initialisation (IMG Library)", "Error in the initialization of the SDL image library.");
			return false;
		}

		// Initialize SDL_TTF (True Type Font)
		int ttfInit_ret = TTF_Init();

		if (ttfInit_ret == -1) {
			IMG_Quit();
			SDL_DestroyWindow(window); window = NULL;
			SDL_DestroyRenderer(renderer); renderer = NULL;
			SDL_Quit();

			Console::Output::PrintError("SDL Initialisation (TTF Library)", "Error in the initialization of the SDL font library.");
			return false;
		}

		// Target texture creacion
		targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, width, height);

		if (targetTexture == NULL) {
			TTF_Quit();
			IMG_Quit();
			SDL_DestroyWindow(window); window = NULL;
			SDL_DestroyRenderer(renderer); renderer = NULL;
			SDL_Quit();

			Console::Output::PrintError("Target texture creation failed", "Error in the creation of the target texture.");
			return false;
		}

		showCursor(showcursor);

		return true;
	}

	void RendererManager::closeSDL() {
		SDL_DestroyWindow(window); window = NULL;
		SDL_DestroyRenderer(renderer); renderer = NULL;
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	SDL_Window* RendererManager::getWindow() {
		return window;
	}

	SDL_Renderer* RendererManager::getRenderer() {
		return renderer;
	}

	int RendererManager::getWidth() {
		return width;
	}

	int RendererManager::getHeight() {
		return height;
	}

	void RendererManager::toggleFullScreen() {
		auto flags = SDL_GetWindowFlags(window);

		if (flags & SDL_WINDOW_FULLSCREEN)
			if (SDL_SetWindowFullscreen(window, 0) < 0)
				Console::Output::PrintError("Window FullScreen Error", SDL_GetError());
		else
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
				Console::Output::PrintError("Window FullScreen Error", SDL_GetError());
	}

	void RendererManager::resizeWindow(int w, int h) {

		if (w <= 0 || h <= 0) {
			Console::Output::PrintError("Invalid size parameters", "The provided height and width must be greater than zero.");
			return;
		}

		SDL_SetWindowSize(window, w, h);
	}

	void RendererManager::renameWindow(cString name) {
		SDL_SetWindowTitle(window, name.c_str());
	}

	void RendererManager::repositionWindow(int x, int y) {
		SDL_SetWindowPosition(window, x, y);
	}


	void RendererManager::showCursor(bool show) {

		if (SDL_ShowCursor(show) < 0)
			Console::Output::PrintError("Show cursor error", SDL_GetError());
	}

	void RendererManager::clearRenderer(cColor bg) {
		SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
		SDL_RenderClear(renderer);
	}

	void RendererManager::presentRenderer() {
		SDL_RenderPresent(renderer);
	}

	void RendererManager::CopyTargetTextureContent() {
		SDL_RenderCopy(renderer, targetTexture, nullptr, nullptr);
	}

	void RendererManager::AdjustRectToCamera(int* x, int* y, int* w, int* h) {

		auto campos = CameraPosition();
		float camscale = CameraScale();

		int winwid = Renderer::RendererManager::instance()->getWidth() * 0.5f;
		int winhei = Renderer::RendererManager::instance()->getHeight() * 0.5f;

		//	Invert Y-position
		*y *= -1;

		//	Move the coordinate center to the center of the screen
		*x += winwid;
		*y += winhei;

		//	Calculate camera displacement
		*x += campos.x_;
		*y += campos.y_;

		//	Scale positions according to the camera's scale
		*x *= camscale;
		*y *= camscale;
		
		//	Center the final scaling at the center
		*x += winwid - (winwid * camscale);
		*y += winhei - (winhei * camscale);


		if (w != NULL)
			*w *= camscale;
		if (h != NULL)
			*h *= camscale;

	}

	Utilities::Vector2D RendererManager::CameraPosition() {
		return cameraPosition;
	}

	float RendererManager::CameraScale() {
		return cameraScale;
	}

	void RendererManager::SetCameraPosition(cVector2D vec) {
		cameraPosition = vec;
	}

	void RendererManager::SetCameraScale(float scale) {
		cameraScale = scale;
	}


	bool RendererManager::SetWindowIcon(cString path) {

		icon = IMG_Load(path.c_str());

		if (icon == NULL) {
			Console::Output::PrintError("Window icon", "Could not change window icon. Error loading file <" + path + ">");
			return false;
		}

		SDL_SetWindowIcon(window, icon);
	}

	void RendererManager::SetRenderTarget(bool renderOnTarget) {

		SDL_Texture* texture = nullptr;

		if (renderOnTarget)
			texture = targetTexture;

		if (SDL_SetRenderTarget(renderer, texture) < 0) 
			Console::Output::PrintError("Render target error", SDL_GetError());

	}

}