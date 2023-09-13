#include "Font.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "RendererManager.h"
#include "Texture.h"
#include "ConsoleManager.h"

#include <filesystem>
#include <iostream>
#include <fstream>

Renderer::Font::Font(const std::string& filepath, int pointSize) : pointSize(pointSize), path(filepath)
{
	font = TTF_OpenFont(filepath.c_str(), pointSize);

	if (font == NULL)
		Console::Output::PrintError("Invalid font", "Font couldnt be loaded from file <" + path + ">");
}

Renderer::Font::~Font()
{
	TTF_CloseFont(font);
	font = nullptr;
}

TTF_Font* Renderer::Font::getSDLFont() {
	return font;
}

Renderer::Texture* Renderer::Font::CreateText(std::string const& message)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), { 255, 255, 255, 255 });

	// Check for errors on surface creation
	if (surface == NULL) {
		Console::Output::PrintError("Font loading error", "Couldnt create the surface of the font from file  <" + path + ">");
		return nullptr;
	}

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	// Check for errors on texture creation
	if (sdlText == NULL) {
		Console::Output::PrintError("Font loading error", SDL_GetError());
		SDL_FreeSurface(surface);
		return nullptr;
	}

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}

Renderer::Texture* Renderer::Font::CreateWrappedText(std::string const& message, int maxSize)
{
	SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font, message.c_str(), { 255, 255, 255, 255 }, maxSize);

	// Check for errors on surface creation
	if (surface == NULL) {
		Console::Output::PrintError("Font loading error", "Couldnt create the surface of the font from file  <" + path + ">");
		return nullptr;
	}

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	// Check for errors on texture creation
	if (sdlText == NULL) {
		Console::Output::PrintError("Font loading error", SDL_GetError());
		SDL_FreeSurface(surface);
		return nullptr;
	}

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}