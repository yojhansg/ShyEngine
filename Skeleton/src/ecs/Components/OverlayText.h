#pragma once


#include <string>
#include <Component.h>

using cString = std::string const&;

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

		enum class Fit {
			Clamp, Overflow, WrapClamp, WrapOverflow, Expand
		};
		static int ConvertFit(cString str);

		enum class VerticalAlignment {
			Top, Center, Bottom
		};
		static int ConvertVerticalAlignment(cString str);

		enum class HorizontalAlignment {
			Left, Center, Right
		};
		static int ConvertHorizontalAlignment(cString str);

		OverlayText();
		~OverlayText();



	private:

		Overlay* overlay;

		Renderer::Font* fontPtr;
		Renderer::Texture* texture;

		reflect std::string text;
		reflect std::string font;
		reflect int fontSize;

		int horizontalAlignment_internal;
		int verticalAlignment_internal;
		int fit_internal;

		reflect std::string horizontalAlignment;
		reflect std::string verticalAlignment;

		reflect std::string fit;

		reflect int lineSpacing;

		void init() override;
		void render() override;

		void createFont();
		void freeTexture();
		void createTexture();

	publish:


		std::string GetFit();
		void SetFit(cstring fit);

		std::string GetVerticalAlignment();
		std::string GetHorizontalAlignment();

		void SetVerticalAlignment(cString align);
		void SetHorizontalAlignment(cString align);


		std::string GetFont();
		void SetFont(std::string font);

		std::string GetText();
		void SetText(std::string text);

		int GetPointSize();
		void SetPointSize(int size);
	};

}