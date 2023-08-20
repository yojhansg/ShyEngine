#include "Font.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "Texture.h"
#include "Editor.h"

namespace ShyEditor {

	Font::Font(const std::string& filepath, int pointSize) : pointSize(pointSize), path(filepath)
	{
		font = TTF_OpenFont(filepath.c_str(), pointSize);

		//if (font == NULL)
			// TODO Aviso por el log
	}

	Font::~Font()
	{
		TTF_CloseFont(font);
		font = nullptr;
	}

	TTF_Font* Font::getSDLFont() {
		return font;
	}

	Texture* Font::CreateText(std::string const& message)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), { 255, 255, 255, 255 });

		// Check for errors on surface creation
		if (surface == NULL) {
			// TODO Aviso por el log
			return nullptr;
		}

		auto sdlText = SDL_CreateTextureFromSurface(Editor::getInstance()->getRenderer(), surface);

		// Check for errors on texture creation
		if (sdlText == NULL) {
			// TODO Aviso por el log
			SDL_FreeSurface(surface);
			return nullptr;
		}

		SDL_FreeSurface(surface);

		Texture* texture = new Texture(sdlText);

		return texture;
	}

	Texture* Font::CreateWrappedText(std::string const& message, int maxSize)
	{
		SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font, message.c_str(), { 255, 255, 255, 255 }, maxSize);

		// Check for errors on surface creation
		if (surface == NULL) {
			// TODO Aviso por el log
			return nullptr;
		}

		auto sdlText = SDL_CreateTextureFromSurface(Editor::getInstance()->getRenderer(), surface);

		// Check for errors on texture creation
		if (sdlText == NULL) {
			// TODO Aviso por el log
			SDL_FreeSurface(surface);
			return nullptr;
		}

		SDL_FreeSurface(surface);

		Texture* texture = new Texture(sdlText);

		return texture;
	}

}