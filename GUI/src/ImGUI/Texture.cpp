#include "Texture.h"
#include <SDL_image.h>

#include "Editor.h"

#include "CheckML.h"

namespace ShyEditor {

	Texture::Texture(const std::string& filepath) {

		texture = nullptr;
		width = height = 0;

		// Surface and texture
		SDL_Surface* surface = IMG_Load(filepath.c_str());

		if (surface == nullptr)
		{
			// TODO Aviso por el log
			return;
		}

		texture = SDL_CreateTextureFromSurface(Editor::getInstance()->getRenderer(), surface);

		if (texture == nullptr)
		{
			SDL_FreeSurface(surface);
			// TODO Aviso por el log
			return;
		}

		width = surface->w; height = surface->h;

		SDL_FreeSurface(surface);
	}

	Texture::Texture(SDL_Texture* text)
	{
		texture = text;

		if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) < 0) {
			// TODO Aviso por el log
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
