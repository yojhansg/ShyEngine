#include "Texture.h"
#include "RendererManager.h"

#include <ConsoleManager.h>
#include <SDL_image.h>
#include <cassert>

Renderer::Texture::Texture(const std::string& filepath) {

	texture = nullptr;
	width = height = 0;

	// Surface and texture
	SDL_Surface* surface = IMG_Load((filepath).c_str());

	if (surface == nullptr) {
		Console::Output::PrintError("Image loading error", "Couldn't load image: " + filepath);
		return;
	}

	texture = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	if (texture == nullptr) {
		Console::Output::PrintError("Texture creating error", "The texture could not be created from the image: " + filepath);
		return;
	}

	width = surface->w; height = surface->h;

	SDL_FreeSurface(surface);
}

Renderer::Texture::Texture(SDL_Texture* text) {
	texture = text;

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}


Renderer::Texture::~Texture() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

SDL_Texture* Renderer::Texture::getSDLTexture() {
	return texture;
}

int Renderer::Texture::getWidth() {
	return width;
}

int Renderer::Texture::getHeight() {
	return height;
}
