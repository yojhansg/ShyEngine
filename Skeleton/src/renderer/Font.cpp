#include "Font.h"
#include <SDL.h>
#include <SDL_ttf.h>


RendererManager::Font::Font(const std::string& filepath, int pointSize): pointSize(pointSize), path(filepath)
{
	font = TTF_OpenFont(filepath.c_str(), pointSize);
}

RendererManager::Font::~Font()
{
	TTF_CloseFont(font);
	font = nullptr;
}

TTF_Font* RendererManager::Font::getSDLFont()
{
	return font;
}

RendererManager::Texture* RendererManager::Font::CreateText(std::string const& message)
{
	//TODO: hacer clase para el color
	SDL_Surface* surf = TTF_RenderText_Solid(font, message.c_str(), {0, 0, 0, 0});

	return nullptr;
}
