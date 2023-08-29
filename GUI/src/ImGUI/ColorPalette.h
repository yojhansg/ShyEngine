#pragma once

#include "Window.h"

#include <unordered_map>
#include <string>

struct ImFont;

namespace ShyEditor {

	class ColorPalette : public Window {

	public:

		struct Color {

			float r, g, b;

			static Color FromHexString(const std::string& hex);

		};

		struct Palette {

			//General
			Color text;
			Color head;
			Color area;
			Color body;
			Color popups;

			Color scriptBackground;

			float windowRounding;


			//Scene

			struct {

				Color background;
				float backgroundAlpha;

				Color frame;
				int frameWidth;

				Color objectFrame;
				int objectFrameWidth;

				Color UIBorder;
				Color UIBorderHover;
				int UIBorderWidth;


				Color grid;
				int gridSpacing;
				int gridInterval;
				float gridAlpha;
				int gridThickness;

			} scene;


			// Scripting
			float nodeRounding;

			Color grid;
			float gridThickness;
			float gridSpacing;
			float gridIntervalScale;
			int gridInterval;
			
			Color hover;
			float buttonThickness;

			Color line;
			float lineThickness;
			Color lineOutline;
			float lineOutlineThickness;
			float lineCurvature;
			float lineAlpha;

			Color flowline;
			float flowlineThickness;
			Color flowlineOutline;
			float flowlineOutlineThickness;
			float flowlineCurvature;
			float flowlineAlpha;


			//Font

			ImFont* fontPtr;
			std::string font;
			float fontSize;
		};

	public:

		ColorPalette(const std::string& name);
		~ColorPalette();

		void SwapPalette(const std::string& palette);

		void Serialize();

		static void Open();

		void Behaviour() override;

		static void ApplyPalette();

		static Palette& GetCurrentPalette();


	private:

		void LoadPalettes();
		void LoadDefaultPalette();

		void Apply();

		static ColorPalette* instance;

		Palette current;

		std::string name;
		std::unordered_map<std::string, Palette> palettes;

		bool open;
		bool pendingApply;
		bool initialisation;

	};

}


#define ColorPaletteParams(color) color.r, color.g, color.b
#define ColorPaletteParamsInt(color) color.r * 255, color.g * 255, color.b * 255

#define ColorPalette2ImVec4(color) ImVec4(ColorPaletteParams(color), 1)
#define ColorPaletteAlpha2ImVec4(color, a) ImVec4(ColorPaletteParams(color), a)
#define ColorPalette2ImColor(color) ImColor(ColorPalette2ImVec4(color))
#define ColorPaletteAlpha2ImColor(color, a) ImColor(ColorPaletteAlpha2ImVec4(color, a))
#define RGBA2ImColor(r, g, b, a) ImColor(ImVec4(r, g, b, a))
#define RGB2ImColor(r, g, b) RGBA2ImColor(r, g, b, 1)