#include "Font.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "RendererManager.h"
#include "Texture.h"
#include "ConsoleManager.h"

Renderer::Font::Font(const std::string& filepath, int pointSize) : pointSize(pointSize), path(filepath)
{
	font = TTF_OpenFont(filepath.c_str(), pointSize);

	if(font == nullptr)
		Console::Output::PrintError("Invalid font", "Font couldnt be loaded <" + path + ">");
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
	if (font == nullptr) {

		Console::Output::PrintError("Font error", "Tried to create text from an invalid font <" + path + ">");
		return nullptr;
	}
	SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), { 255, 255, 255, 255 });

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}

Renderer::Texture* Renderer::Font::CreateWrappedText(std::string const& message, int maxSize)
{
	if (font == nullptr) {

		Console::Output::PrintError("Font error", "Tried to create text from an invalid font <" + path + ">");
		return nullptr;
	}


	SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font, message.c_str(), { 255, 255, 255, 255 }, maxSize);

	auto sdlText = SDL_CreateTextureFromSurface(RendererManager::instance()->getRenderer(), surface);

	SDL_FreeSurface(surface);

	Renderer::Texture* texture = new Renderer::Texture(sdlText);

	return texture;
}