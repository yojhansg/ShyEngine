#pragma once

#include "Window.h"

#include <unordered_map>
#include <string>


struct ImFont;

namespace PEditor {

	class ColorPalette : public Window
	{
	public:
		struct Color {

			float r, g, b;

			static Color FromHexString(const std::string& hex);

		};

#define ColorPaletteParams(color) color.r, color.g, color.b
#define ColorPalette2ImVec4(color) ImVec4(ColorPaletteParams(color), 1)
#define ColorPalette2ImColor(color) ImColor(ColorPalette2ImVec4(color)
#define RGBA2ImColor(r, g, b, a) ImColor(ImVec4(r, g, b, a))
#define RGB2ImColor(r, g, b) RGBA2ImColor(r, g, b, 1)

		struct Palette {

			Color text;
			Color head;
			Color area;
			Color body;
			Color popups;

			Color scriptBackground;
			Color line;
			Color grid;

			ImFont* fontPtr;
			std::string font;
			float fontSize;
		};


	private:

		static ColorPalette* instance;

		Palette current;

		std::string path;
		std::unordered_map<std::string, Palette> palettes;

		bool open;
		bool visible;
		bool pendingApply;
		bool initialisation;

		int loadedFonts;

		void LoadPalettes();
		void LoadDefaultPalette();

		void Apply();
	public:


		ColorPalette(const std::string& path);
		~ColorPalette();

		void SwapPalette(const std::string& palette);

		void Serialize();

		static void Open();

		void render() override;

		static void ApplyPalette();


		static Palette& GetCurrentPalette();

	};

}