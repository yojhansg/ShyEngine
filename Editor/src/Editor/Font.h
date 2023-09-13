#pragma once

#include <string>

typedef struct _TTF_Font TTF_Font;

namespace ShyEditor {

	class Texture;
	class Font {

	public:

		Font(const std::string& filepath, int pointSize);

		~Font();

		TTF_Font* getSDLFont();
		Texture* CreateText(std::string const& message);
		Texture* CreateWrappedText(std::string const& message, int maxSize);

	private:

		int pointSize;
		std::string path;

		TTF_Font* font;
	};

}