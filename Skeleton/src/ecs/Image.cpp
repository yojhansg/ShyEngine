#include "Image.h"
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include <SDL_image.h>

namespace ECS {

	Image::Image() {
		renderer = nullptr; texture = nullptr; transform = nullptr;
		width = height = 0;

		fileName = "";
	}

	Image::Image(const std::string& fileName) {
		
		renderer = nullptr; texture = nullptr; transform = nullptr;
		width = height = 0;

		this->fileName = fileName;
	}

	Image::~Image() {
		if (texture != nullptr)
			SDL_DestroyTexture(texture);
	}
	
	void Image::init() {

		renderer = RendererManager::RendererManager::instance()->getRenderer();

		SDL_Surface* surface = IMG_Load(fileName.c_str());
		if (surface == nullptr)
			throw "Couldn't load image: " + fileName;

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr) {
			SDL_FreeSurface(surface);
			throw "Couldn't load image: " + fileName;
		}

		width = surface->w;
		height = surface->h;

		transform = getEntity()->getComponent<Transform>();
	}

	void Image::render() {
		SDL_Rect src = { 0, 0, width, height };
		SDL_Rect dest = { 0, 0, width, height };
		SDL_RenderCopy(renderer, texture, &src, &dest);
	}

	int Image::getWidth() {
		return width;
	}

	int Image::getHeight() {
		return height;
	}

}