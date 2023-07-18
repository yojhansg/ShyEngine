#include "Texture.h"
#include "RendererManager.h"
#include <SDL_image.h>
#include <cassert>

Renderer::Texture::Texture(const std::string& filepath) {

	//TODO control de errores al crear textura (esta con asserts)
	
	// Surface and texture
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	assert(surface != nullptr, "Couldn't load image: " + filepath);

	texture = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);
	assert(texture != nullptr, "Couldn't load image: " + filepath);

	width = surface->w; height = surface->h;

	SDL_FreeSurface(surface);
}

Renderer::Texture::Texture(SDL_Texture* text)
{
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
