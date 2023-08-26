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
		static std::string ConvertFit(int idx);

		enum class VerticalAlignment {
			Top, Center, Bottom
		};
		static int ConvertVerticalAlignment(cString str);
		static std::string ConvertVerticalAlignment(int idx);

		enum class HorizontalAlignment {
			Left, Center, Right
		};
		static int ConvertHorizontalAlignment(cString str);
		static std::string ConvertHorizontalAlignment(int idx);

		OverlayText();
		~OverlayText();



	private:

		Overlay* overlay;

		Renderer::Font* fontPtr;
		Renderer::Texture* texture;

		reflect std::string text;
		reflect std::string font;
		reflect int fontSize;

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