#include "Font.h"

#include "LogManager.h"
#include "Texture.h"
#include "Editor.h"

#include <SDL_ttf.h>
#include <SDL.h>

#include "CheckML.h"
#include <iostream>

namespace ShyEditor {

	Font::Font(const std::string& filepath, int pointSize) : pointSize(pointSize), path(filepath)
	{
		font = TTF_OpenFont(filepath.c_str(), pointSize);


		std::cout << "nueva fuente" << std::endl;

		if (font == NULL) {

			LogManager::LogError("Could not create font with path: " + filepath);
		}
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
			LogManager::LogError("The surface could not be created from the current text font.");
			return nullptr;
		}

		auto sdlText = SDL_CreateTextureFromSurface(Editor::GetInstance()->GetRenderer(), surface);

		// Check for errors on texture creation
		if (sdlText == NULL) {
			LogManager::LogError("The texture could not be created from the current text font.");
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
			LogManager::LogError("The surface could not be created from the current text font.");
			return nullptr;
		}

		auto sdlText = SDL_CreateTextureFromSurface(Editor::GetInstance()->GetRenderer(), surface);

		// Check for errors on texture creation
		if (sdlText == NULL) {
			LogManager::LogError("The texture could not be created from the current text font.");
			SDL_FreeSurface(surface);
			return nullptr;
		}

		SDL_FreeSurface(surface);

		Texture* texture = new Texture(sdlText);

		return texture;
	}

}