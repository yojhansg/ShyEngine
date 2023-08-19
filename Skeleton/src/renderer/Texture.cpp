#include "Texture.h"
#include "RendererManager.h"
#include <SDL_image.h>
#include "ConsoleManager.h"

Renderer::Texture::Texture(const std::string& filepath) {

	//TODO control de errores al crear textura (esta con asserts)
	// Surface and texture
	SDL_Surface* surface = IMG_Load(filepath.c_str());

	if (surface == nullptr)
	{
		Console::Output::PrintError("Texture error", "Couldnt load texture <" + filepath + ">");
		return;
	}

	texture = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	if (texture == nullptr)
	{
		Console::Output::PrintError("Texture error", "Couldnt load texture <" + filepath + ">");
		return;
	}
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
