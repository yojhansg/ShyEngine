#include "ColorPalette.h"

#include "ResourcesManager.h"
#include "nlohmann/json.hpp"
#include "LogManager.h"
#include "imgui.h"

#include <fstream>
#include <sstream>

using nlohmann::json;

namespace ShyEditor {

	ColorPalette* ColorPalette::instance = nullptr;

	ColorPalette::ColorPalette(const std::string& name) : Window("Palette selector", ImGuiWindowFlags_NoCollapse), filename(name)
	{
		initialisation = true;
		current = {};
		instance = this;
		pendingApply = false;
		changes = false;

		LoadFonts();
		LoadDefaultPalette();
		LoadPalettes();

		windowWidth = 500;
		windowHeight = 500;
		canBeDisplayedOnTop = true;

		SwapPalette("Engine default");
		Apply();

		initialisation = false;

	}

	ColorPalette::~ColorPalette()
	{
		instance = nullptr;
	}

	void ColorPalette::LoadDefaultPalette()
	{
		Palette palette{};

		palette.name = "Engine default";
		palette.text = { 0.9254901960784314f , 0.9411764705882353f , 0.9450980392156863f };
		palette.head = { 0.1607843137254902f , 0.5019607843137255f , 0.7254901960784314f };
		palette.area = { 0.2235294117647059f , 0.3098039215686275f , 0.4117647058823529f };
		palette.body = { 0.1725490196078431f , 0.2431372549019608f , 0.3137254901960784f };
		palette.popups = { 0.1294117647058824f , 0.1803921568627451f , 0.2352941176470588f };

		palette.scripting.scriptBackground = { .12f, .12f, .12f };

		palette.scripting.hover = { 1, .6f, .6f };
		palette.scripting.buttonThickness = 1;

		palette.windowRounding = 0;
		palette.scripting.nodeRounding = 10;

		palette.scripting.line = { 1, 1, 1 };
		palette.scripting.lineThickness = 1;
		palette.scripting.lineOutline = { 1, 1, 1 };
		palette.scripting.lineOutlineThickness = 0;
		palette.scripting.lineCurvature = 0.5f;
		palette.scripting.lineAlpha = 1;

		palette.scripting.flowline = { 1, 1, 1 };
		palette.scripting.flowlineThickness = 5;
		palette.scripting.flowlineOutline = { 1, 1, 1 };
		palette.scripting.flowlineOutlineThickness = 0;
		palette.scripting.flowlineCurvature = 0.5f;
		palette.scripting.flowlineAlpha = 1;

		palette.scripting.grid = { .4f, .4f, .4f };
		palette.scripting.gridInterval = 5;
		palette.scripting.gridThickness = 1;
		palette.scripting.gridIntervalScale = 4;
		palette.scripting.gridSpacing = 50;



		palette.scene.background = { .2f, .2f, .2f };
		palette.scene.backgroundAlpha = 0;
		palette.scene.frame = { 0, 0 ,0 };
		palette.scene.frameWidth = 10;
		palette.scene.objectFrame = { 255, 0, 0 };
		palette.scene.objectFrameWidth = 3;
		palette.scene.UIBorder = { 1, 1, 1 };
		palette.scene.UIBorderHover = { 1, .2f, .2f };
		palette.scene.UIBorderWidth = 5;
		palette.scene.gridInterval = 5;
		palette.scene.gridSpacing = 50;
		palette.scene.gridAlpha = .5f;
		palette.scene.gridThickness = 1;
		palette.scene.grid = { 255, 255, 255 };



		std::string defaultFont = "Montserrat - Regular.ttf_18";
		if (fonts.contains(defaultFont))
			palette.fontPtr = fonts[defaultFont];


		if (palette.fontPtr == nullptr)
			LogManager::LogError("Could not load default font");

		palettes.emplace(palette.name, palette);
	}


	void ColorPalette::LoadFonts()
	{
		std::string palettePath = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes\\Fonts.palette";

		std::ifstream fileStream(palettePath);

		if (!fileStream.good() || !json::accept(fileStream)) {
			LogManager::LogError("Could not load fonts: " + palettePath);
			return;
		}

		fileStream.clear();
		fileStream.seekg(0);

		json root = json::parse(fileStream);
		fileStream.close();


		for (auto& fontJs : root) {

			std::string font = fontJs["font"];
			int size = fontJs["size"];

			std::string key = font + "_" + std::to_string(size);

			auto fontPtr = ImGui::GetIO().Fonts->AddFontFromFileTTF((ResourcesManager::EDITORASSETSFOLDER + "\\Fonts\\" + font).c_str(), size);

			fonts[key] = fontPtr;
		}

	}

	void ColorPalette::LoadPalettes() {

		std::string palettePath = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes" + "\\" + filename + ".palette";

		std::ifstream fileStream(palettePath);

		if (!fileStream.good() || !json::accept(fileStream)) {
			LogManager::LogError("Could not load palette with path: " + palettePath);
			return;
		}

		fileStream.clear();
		fileStream.seekg(0);

		json root = json::parse(fileStream);
		fileStream.close();

		for (auto& paletteInfo : root) {

			std::string name = paletteInfo["name"].get<std::string>();

			Palette palette{};

			palette.name = name;

			std::string font = paletteInfo["font"].get<std::string>();
			palette.fontPtr = fonts.contains(font) ? fonts[font] : nullptr;


			//	ImGui::GetIO().Fonts->GetTexDataAsRGBA32(NULL, NULL, NULL, NULL);
#define ReadPaletteValue(value, palette, data) palette.value = Color::FromHexString(data[#value].get<std::string>());

			ReadPaletteValue(text, palette, paletteInfo);
			ReadPaletteValue(head, palette, paletteInfo);
			ReadPaletteValue(area, palette, paletteInfo);
			ReadPaletteValue(body, palette, paletteInfo);
			ReadPaletteValue(popups, palette, paletteInfo);
			ReadPaletteValue(scripting.scriptBackground, palette, paletteInfo);
			ReadPaletteValue(scripting.grid, palette, paletteInfo);
			ReadPaletteValue(scripting.line, palette, paletteInfo);

			//TODO: leer el resto de colores

#undef ReadPaletteValue


		//TODO: leer valores que no sean colores

			palettes.emplace(name, palette);
		}

		ImGui::GetIO().Fonts->Build();
	}

	void ColorPalette::SwapPalette(const std::string& palette)
	{
		current = palettes[palette];
		pendingApply = true;
	}

	void ColorPalette::Apply()
	{
		auto& io = ImGui::GetIO();

		if (current.fontPtr != nullptr)
			io.FontDefault = current.fontPtr;

		ImGuiStyle& style = ImGui::GetStyle();

#define toVec4(col, a) ImVec4(col.r, col.g, col.b, a)

		style.Colors[ImGuiCol_Text] = toVec4(current.text, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = toVec4(current.text, 0.58f);
		style.Colors[ImGuiCol_WindowBg] = toVec4(current.body, 0.95f);
		style.Colors[ImGuiCol_ChildBg] = toVec4(current.area, 0.58f);
		style.Colors[ImGuiCol_Border] = toVec4(current.body, 0.00f);
		style.Colors[ImGuiCol_BorderShadow] = toVec4(current.body, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = toVec4(current.area, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = toVec4(current.head, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = toVec4(current.area, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = toVec4(current.area, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = toVec4(current.area, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = toVec4(current.area, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = toVec4(current.head, 0.21f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = toVec4(current.head, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = toVec4(current.head, 1.00f);
		//style.Colors[ImGuiCol_ComboBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = toVec4(current.head, 0.80f);
		style.Colors[ImGuiCol_SliderGrab] = toVec4(current.head, 0.50f);
		style.Colors[ImGuiCol_SliderGrabActive] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_Button] = toVec4(current.head, 0.50f);
		style.Colors[ImGuiCol_ButtonHovered] = toVec4(current.head, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_Header] = toVec4(current.head, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = toVec4(current.head, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = toVec4(current.head, 1.00f);
		//style.Colors[ImGuiCol_Column] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
		//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
		//style.Colors[ImGuiCol_ColumnActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = toVec4(current.head, 0.15f);
		style.Colors[ImGuiCol_ResizeGripHovered] = toVec4(current.head, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = toVec4(current.head, 1.00f);
		//style.Colors[ImGuiCol_CloseButton] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.16f);
		//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.39f);
		//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = toVec4(current.text, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = toVec4(current.text, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = toVec4(current.head, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = toVec4(current.head, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = toVec4(current.popups, 0.92f);
		//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);

		style.WindowRounding = current.windowRounding;

		pendingApply = false;
	}

	void ColorPalette::Serialize()
	{
		//TODO: serializar el valor actual
	}

	void ColorPalette::Open()
	{
		instance->Show();

	}

	void ColorPalette::Behaviour()
	{
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
		ImGui::Begin(windowName.c_str(), &visible, flags);


		ImGui::TextWrapped("Choose theme");

		if (ImGui::BeginCombo("Palette selector", current.name.c_str())) {
			for (auto& palette : palettes) {

				//TODO: push de los colores
				ImGui::PushStyleColor(ImGuiCol_Text, toVec4(palette.second.text, 1));
				ImGui::PushStyleColor(ImGuiCol_Header, toVec4(palette.second.body, 1));
				ImGui::PushFont(palette.second.fontPtr);


				if (ImGui::Selectable(palette.first.c_str(), current.name == palette.second.name)) {

					SwapPalette(palette.first);
				}


				ImGui::PopStyleColor(2);
				ImGui::PopFont();
			}

			ImGui::EndCombo();
		}

		ImGui::ShowFontSelector("Font");

#define ShowColor(name, color) if (ImGui::ColorEdit3(filename, &color.r)) areThereChanges = true;
#define ShowFloat(name, value, speed, min, max)\
	if (ImGui::DragFloat(filename, &value), speed, min, max, "%.3f", ImGuiSliderFlags_AlwaysClamp) {\
	 areThereChanges = true; if(value < min) value = min; if(value > max) value = max;}



		if (ImGui::CollapsingHeader("Editor")) {

			ImGui::Indent();

			Col("Text", &current.text);
			Col("Area", &current.area);
			Col("Body", &current.body);
			Col("Head", &current.head);
			Col("Pop ups", &current.popups);
			Float("Window rounding", &current.windowRounding);

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Scene")) {

			ImGui::Indent();

			Col("Background", &current.scene.background);
			NormalizedFloat("Background alpha", &current.scene.backgroundAlpha);

			Col("Frame", &current.scene.frame);
			Int("Frame thickness", &current.scene.frameWidth);

			Col("Entity frame", &current.scene.objectFrame);
			Int("Entity frame thickness", &current.scene.objectFrameWidth);

			Col("UI frame", &current.scene.UIBorder);
			Col("UI hover", &current.scene.UIBorderHover);
			Int("UI frame thickness", &current.scene.UIBorderWidth);


			Col("Grid", &current.scene.grid);
			Float("Grid alpha", &current.scene.gridAlpha);
			Int("Grid thickness", &current.scene.gridThickness, 0, 100, 1, true);
			Int("Grid spacing", &current.scene.gridSpacing, 1, 1000, 1, true);
			Int("Grid interval", &current.scene.gridInterval, 1, 100, .2f);


			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Scripting")) {
			ImGui::Indent();

			Col("Background", &current.scripting.scriptBackground);
			Col("Hover", &current.scripting.hover);
			Float("Button thickness", &current.scripting.buttonThickness);
			Float("Node rounding", &current.scripting.nodeRounding);

			if (ImGui::CollapsingHeader("Graphics")) {
				ImGui::Indent();

				if (ImGui::CollapsingHeader("Input line")) {
					ImGui::Indent();

					Float("Thickness##Input", &current.scripting.lineThickness);
					Col("Color##Input", &current.scripting.line);
					Float("Outline thickness##Input", &current.scripting.lineOutlineThickness);

					if (current.scripting.lineOutlineThickness > 0)
						Col("Outline color##Input", &current.scripting.lineOutline);

					NormalizedFloat("Curvature##Input", &current.scripting.lineCurvature);
					NormalizedFloat("Alpha##Input", &current.scripting.lineAlpha);

					ImGui::Unindent();
				}
				if (ImGui::CollapsingHeader("Flow line")) {
					ImGui::Indent();

					Float("Thickness##Flow", &current.scripting.flowlineThickness);
					Col("Color##Flow", &current.scripting.flowline);
					Float("Outline thickness##Flow", &current.scripting.flowlineOutlineThickness);

					if (current.scripting.flowlineOutlineThickness > 0)
						Col("Outline color##Flow", &current.scripting.flowlineOutline);

					NormalizedFloat("Curvature##Flow", &current.scripting.flowlineCurvature);
					NormalizedFloat("Alpha##Flow", &current.scripting.flowlineAlpha);

					ImGui::Unindent();
				}

				if (ImGui::CollapsingHeader("Grid")) {
					ImGui::Indent();

					Int("Spacing##Grid", &current.scripting.gridSpacing, 2, 1000, 2, true);
					Int("Thickness##Grid", &current.scripting.gridThickness, 0, 100, 1, true);

					Col("Color##Grid", &current.scripting.grid);
					Int("Interval##Grid", &current.scripting.gridInterval, 1, 100, .2f);
					Float("Interval Scale##Grid", &current.scripting.gridIntervalScale);
					ImGui::Unindent();
				}

				ImGui::Unindent();
			}


			ImGui::Unindent();
		}


		if (changes) {

			pendingApply = true;
		}


		ImGui::End();
	}


	void ColorPalette::Col(const char* name, Color* value)
	{
		if (ImGui::ColorEdit3(name, &value->r))
			changes = true;
	}

	void ColorPalette::Int(const char* name, int* value, int min, int max, float speed, bool log)
	{
		int flags = ImGuiSliderFlags_AlwaysClamp;

		if (log)
			flags |= ImGuiSliderFlags_Logarithmic;


		if (ImGui::DragInt(name, value, speed, min, max, "%d", flags))
			changes = true;
	}

	void ColorPalette::NormalizedFloat(const char* name, float* value)
	{
		if (ImGui::SliderFloat(name, value, 0, 1))
			changes = true;
	}

	void ColorPalette::Float(const char* name, float* value, float min, float max, float speed, bool log)
	{
		int flags = ImGuiSliderFlags_AlwaysClamp;

		if (log)
			flags |= ImGuiSliderFlags_Logarithmic;

		if (ImGui::DragFloat(name, value, speed, min, max, "%.3f", flags))
			changes = true;
	}

	void ColorPalette::ApplyPalette()
	{
		if (instance->pendingApply)
			instance->Apply();
	}

	ColorPalette::Palette& ColorPalette::GetCurrentPalette()
	{
		return instance->current;
	}

	ColorPalette::Color ColorPalette::Color::FromHexString(const std::string& hex)
	{
		Color color{};
		if (hex.size() == 7 && hex[0] == '#') {

			unsigned int value;
			std::stringstream ss;
			ss << std::hex << hex.substr(1);
			ss >> value;

			color.r = (float)((value >> 16) & 0xFF) / 255.0f;
			color.g = (float)((value >> 8) & 0xFF) / 255.0f;
			color.b = (float)(value & 0xFF) / 255.0f;
		}
		else {

			color = { 1, 0, 1 };
		}

		return color;
	}

}
