#include "Texture.h"
#include "RendererManager.h"
#include <SDL_image.h>
#include <cassert>

RendererManager::Texture::Texture(const std::string& filepath) {
	
	// Surface and texture
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	assert(surface != nullptr, "Couldn't load image: " + filepath);

	texture = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);
	assert(texture != nullptr, "Couldn't load image: " + filepath);

	width = surface->w; height = surface->h;
}


RendererManager::Texture::~Texture() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

SDL_Texture* RendererManager::Texture::getSDLTexture() {
	return texture;
}

int RendererManager::Texture::getWidth() {
	return width;
}

int RendererManager::Texture::getHeight() {
	return height;
}
