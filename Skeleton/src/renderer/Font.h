#pragma once

#include <string>


typedef struct _TTF_Font TTF_Font;
namespace RendererManager {

	class Texture;
	class Font {

	public:

		Font(const std::string& filepat, int pointSize = 32);

		~Font();

		TTF_Font* getSDLFont();
		Texture* CreateText(std::string const& message);


	private:

		int pointSize;
		std::string path;

		TTF_Font* font;
	};

}