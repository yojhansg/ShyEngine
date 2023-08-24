#pragma once

#include <string>

struct SDL_Texture;

namespace ShyEditor {
	
	class Texture {

	public:

		Texture(const std::string& filepath);
		Texture(SDL_Texture* text);

		~Texture();

		SDL_Texture* getSDLTexture();

		int getWidth();
		int getHeight();

	private:

		SDL_Texture* texture;

		int width, height;

	};

}