#include "Texture.h"
#include <SDL_image.h>

#include "LogManager.h"
#include "Editor.h"

#include "CheckML.h"

namespace ShyEditor {

	Texture::Texture(const std::string& filepath) {

		texture = nullptr;
		width = height = 0;

		// Surface and texture
		SDL_Surface* surface = IMG_Load(filepath.c_str());

		if (surface == nullptr) {
			LogManager::LogError("Could not create the surface from the image with path: " + filepath);
			return;
		}

		texture = SDL_CreateTextureFromSurface(Editor::getInstance()->getRenderer(), surface);

		if (texture == nullptr) {
			SDL_FreeSurface(surface);
			LogManager::LogError("Could not create the texture from the image with path: " + filepath);
			return;
		}

		width = surface->w; height = surface->h;

		SDL_FreeSurface(surface);
	}

	Texture::Texture(SDL_Texture* text) {
		texture = text;

		if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) < 0) {
			LogManager::LogError(SDL_GetError());
			return;
		}
	}

	Texture::~Texture() {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	SDL_Texture* Texture::getSDLTexture() {
		return texture;
	}

	int Texture::getWidth() {
		return width;
	}

	int Texture::getHeight() {
		return height;
	}

}
