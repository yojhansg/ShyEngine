#pragma once


#include <string>
#include <Component.h>

namespace Renderer {
	class Font;
	class Texture;
}

namespace ECS {


	class Overlay;
	class OverlayText : public Component {

	public:

		//TODO: color del texto
		//TODO: poner outline
		//TODO: opciones de calidad del texto
		//TODO: ver si hacer el texto en unicode

		enum class Fit {
			Clamp, Overflow, WrapClamp, WrapOverflow, Expand
		};

		enum class VerticalAlignment {
			Top, Center, Bottom
		};

		enum class HorizontalAlignment {
			Left, Center, Right
		};

		OverlayText();
		~OverlayText();
	private:

		Overlay* overlay;

		Renderer::Font* font;
		Renderer::Texture* texture;

		reflect int pointSize;
		reflect std::string path;
		reflect std::string text;

		reflect int horizontalAlignment;
		reflect int verticalAlignment;

		reflect int fit;

		reflect int lineSpacing;

		void init() override;
		void render() override;

		void createFont();
		void freeTexture();
		void createTexture();

	publish:


		int GetFit();
		void SetFit(int fit);

		int GetVerticalAlignment();
		int GetHorizontalAlignment();

		void SetVerticalAlignment(int align);
		void SetHorizontalAlignment(int align);

		std::string GetFont();
		void SetFont(std::string font);

		std::string GetText();
		void SetText(std::string text);

		int GetPointSize();
		void SetPointSize(int size);
	};

}