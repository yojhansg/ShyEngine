#include "Font.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "RendererManager.h"
#include "Texture.h"


Renderer::Font::Font(const std::string& filepath, int pointSize): pointSize(pointSize), path(filepath)
{
	font = TTF_OpenFont(filepath.c_str(), pointSize);
}

Renderer::Font::~Font()
{
	TTF_CloseFont(font);
	font = nullptr;
}

TTF_Font* Renderer::Font::getSDLFont()
{
	return font;
}

Renderer::Texture* Renderer::Font::CreateText(std::string const& message)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), {0, 0, 0, 0});

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);
	assert(sdlText != nullptr, "Couldn't load image: " + filepath);

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}

Renderer::Texture* Renderer::Font::CreateWrappedText(std::string const& message, int maxSize)
{
	SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font, message.c_str(), { 0, 0, 0, 0 }, maxSize);

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);
	assert(sdlText != nullptr, "Couldn't load image: " + filepath);

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}