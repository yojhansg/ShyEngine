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
			std::string ToHexString();
		};

		struct Palette {

			std::string name;

			//General

			Color text; //texto
			Color primary; //cabeceras
			Color secondary; //elementos
			Color background; //fondo ventanas
			Color mainWindowBackground; //fondo ventana sdl
			Color popups; //fondo pop ups
			Color border; //borde ventana


			float windowRounding;

			float windowBorder;
			float frameBorder;
			float popUpBorder;
			float windowAlpha;

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
			struct {
				Color background;
				float nodeRounding;

				Color grid;
				int gridThickness;
				int gridSpacing;
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

			} scripting;
			//Font
			std::string fontName;
			ImFont* fontPtr;
		};

	public:

		ColorPalette();
		~ColorPalette();

		void SwapPalette(const std::string& palette);

		static void Open();

		void Behaviour() override;

		static void ApplyPalette();

		static Palette& GetCurrentPalette();


	private:

		void Col(const char* name, Color* value);
		void Int(const char* name, int* value, int min = 0.0f, int max = 100, float speed = 1, bool log = false);
		void NormalizedFloat(const char* name, float* value);
		void Float(const char* name, float* value, float min = 0.0f, float max = 100, float speed = 1.f, bool log = false);


		void LoadFonts();
		void LoadPalettes(const std::string& file);
		void LoadDefaultPalette();

		void Apply();

		std::string LoadActivePalette();
		void SaveActivePalette();


		void SavePalette();


		static ColorPalette* instance;

		Palette current;

		std::unordered_map<std::string, Palette> palettes;

		std::unordered_map<std::string, ImFont*> fonts;

		bool pendingApply;
		bool initialisation;
		bool changes;
		bool showSavePopup;
		bool showErasePopup;

		char paletteName[16];
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