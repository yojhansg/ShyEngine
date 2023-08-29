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

	ColorPalette::ColorPalette(const std::string& name) : Window("Palette selector", ImGuiWindowFlags_NoCollapse), name(name)
	{
		initialisation = true;
		current = {};
		instance = this;
		pendingApply = false;

		LoadDefaultPalette();
		LoadPalettes();

		windowWidth = 500;
		windowHeight = 500;
		canBeDisplayedOnTop = true;

		SwapPalette("Default");
		Apply();

		initialisation = false;

		Hide();
	}

	ColorPalette::~ColorPalette()
	{
		instance = nullptr;
	}

	void ColorPalette::LoadDefaultPalette() 
	{
		Palette palette{};

		palette.text = { 0.9254901960784314f , 0.9411764705882353f , 0.9450980392156863f };
		palette.head = { 0.1607843137254902f , 0.5019607843137255f , 0.7254901960784314f };
		palette.area = { 0.2235294117647059f , 0.3098039215686275f , 0.4117647058823529f };
		palette.body = { 0.1725490196078431f , 0.2431372549019608f , 0.3137254901960784f };
		palette.popups = { 0.1294117647058824f , 0.1803921568627451f , 0.2352941176470588f };

		palette.scriptBackground = { .12f, .12f, .12f };

		palette.hover = { 1, .6f, .6f };
		palette.buttonThickness = 1;

		palette.windowRounding = 0;
		palette.nodeRounding = 10;

		palette.line = { 1, 1, 1 };
		palette.lineThickness = 1;
		palette.lineOutline = { 1, 1, 1 };
		palette.lineOutlineThickness = 0;
		palette.lineCurvature = 0.5f;
		palette.lineAlpha = 1;

		palette.flowline = { 1, 1, 1 };
		palette.flowlineThickness = 5;
		palette.flowlineOutline = { 1, 1, 1 };
		palette.flowlineOutlineThickness = 0;
		palette.flowlineCurvature = 0.5f;
		palette.flowlineAlpha = 1;

		palette.grid = { .4f, .4f, .4f };
		palette.gridInterval = 5;
		palette.gridThickness = 1;
		palette.gridIntervalScale = 4;
		palette.gridSpacing = 50;



		palette.scene.background = { .2f, .2f, .2f };
		palette.scene.backgroundAlpha = 0;
		palette.scene.frame = { 0, 0 ,0 };
		palette.scene.frameWidth = 10;
		palette.scene.objectFrame = {255, 0, 0};
		palette.scene.objectFrameWidth = 3;
		palette.scene.UIBorder = { 1, 1, 1 };
		palette.scene.UIBorderHover = { 1, .2f, .2f };
		palette.scene.UIBorderWidth = 5;
		palette.scene.gridInterval = 5;
		palette.scene.gridSpacing = 50;
		palette.scene.gridAlpha = .5f;
		palette.scene.gridThickness = 1;
		palette.scene.grid = { 255, 255, 255 };




		palette.font = ResourcesManager::EDITORASSETSFOLDER + "\\Fonts\\Montserrat-Regular.ttf";
		palette.fontSize = 18.f;
		palette.fontPtr = ImGui::GetIO().Fonts->AddFontFromFileTTF(palette.font.c_str(), palette.fontSize);


		if (palette.fontPtr == nullptr)
			LogManager::LogError("Could not load font with path: " + palette.font);

		palettes.emplace("Default", palette);
	}


	void ColorPalette::LoadPalettes() {

		std::string palettePath = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes" + "\\" + name + ".palette";

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

			Palette palette{};
			std::string name = paletteInfo["name"].get<std::string>();
			palette.font = ResourcesManager::EDITORASSETSFOLDER + "\\Fonts\\" + paletteInfo["font"].get<std::string>();
			palette.fontSize = paletteInfo["fontSize"].get<int>();

			palette.fontPtr = ImGui::GetIO().Fonts->AddFontFromFileTTF(palette.font.c_str(), palette.fontSize);

			if (palette.fontPtr == nullptr)
				LogManager::LogError("Could not load font with path: " + palette.font);

			//	ImGui::GetIO().Fonts->GetTexDataAsRGBA32(NULL, NULL, NULL, NULL);

#define ReadPaletteValue(value, palette, data) palette.value = Color::FromHexString(data[#value].get<std::string>());

			ReadPaletteValue(text, palette, paletteInfo);
			ReadPaletteValue(head, palette, paletteInfo);
			ReadPaletteValue(area, palette, paletteInfo);
			ReadPaletteValue(body, palette, paletteInfo);
			ReadPaletteValue(popups, palette, paletteInfo);
			ReadPaletteValue(scriptBackground, palette, paletteInfo);
			ReadPaletteValue(grid, palette, paletteInfo);
			ReadPaletteValue(line, palette, paletteInfo);

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


		for (auto& palette : palettes) {

			//TODO: push de los colores
			ImGui::PushFont(palette.second.fontPtr);


			if (ImGui::Button(palette.first.c_str())) {

				SwapPalette(palette.first);
			}


			ImGui::PopFont();
		}

		ImGui::ShowFontSelector("Font");

#define ShowColor(name, color) if (ImGui::ColorEdit3(name, &color.r)) areThereChanges = true;
#define ShowFloat(name, value, speed, min, max)\
	if (ImGui::DragFloat(name, &value), speed, min, max, "%.3f", ImGuiSliderFlags_AlwaysClamp) {\
	 areThereChanges = true; if(value < min) value = min; if(value > max) value = max;}


		bool areThereChanges = false;

		if (ImGui::CollapsingHeader("Editor")) {

			ImGui::Indent();
			if (ImGui::ColorEdit3("Text", &current.text.r)) areThereChanges = true;

			if (ImGui::ColorEdit3("Area", &current.area.r)) areThereChanges = true;

			if (ImGui::ColorEdit3("Body", &current.body.r)) areThereChanges = true;

			if (ImGui::ColorEdit3("Head", &current.head.r)) areThereChanges = true;

			if (ImGui::ColorEdit3("Pop ups", &current.popups.r)) areThereChanges = true;

			if (ImGui::DragFloat("Window rounding", &current.windowRounding, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;
			//ShowFloat("Window rounding", current.windowRounding, 1, 0, 100);

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Scene")) {

			ImGui::Indent();
			if (ImGui::ColorEdit3("Background", &current.scene.background.r)) areThereChanges = true;
			if (ImGui::DragFloat("Background alpha", &current.scene.backgroundAlpha, .1f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;

			if (ImGui::ColorEdit3("Frame", &current.scene.frame.r)) areThereChanges = true;
			if (ImGui::DragInt("Frame thickness", &current.scene.frameWidth, 1, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;

			if (ImGui::ColorEdit3("Entity frame", &current.scene.objectFrame.r)) areThereChanges = true;
			if (ImGui::DragInt("Entity frame thickness", &current.scene.objectFrameWidth, 1, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;

			if (ImGui::ColorEdit3("UI frame", &current.scene.UIBorder.r)) areThereChanges = true;
			if (ImGui::ColorEdit3("UI hover", &current.scene.UIBorderHover.r)) areThereChanges = true;
			if (ImGui::DragInt("UI frame thickness", &current.scene.UIBorderWidth, 1, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;


			if (ImGui::ColorEdit3("Grid", &current.scene.grid.r)) areThereChanges = true;
			if (ImGui::DragFloat("Grid alpha", &current.scene.gridAlpha, .1f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;
			if (ImGui::DragInt("Grid thickness", &current.scene.gridThickness, 1, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;
			if (ImGui::DragInt("Grid spacing", &current.scene.gridSpacing, 1, 0, 500, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;
			if (ImGui::DragInt("Grid interval", &current.scene.gridInterval, 1, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;



			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Scripting")) {
			ImGui::Indent();
			if (ImGui::ColorEdit3("Background", &current.scriptBackground.r)) areThereChanges = true;
			if (ImGui::ColorEdit3("Hover", &current.hover.r)) areThereChanges = true;
			if (ImGui::DragFloat("Button thickness", &current.buttonThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;
			if (ImGui::DragFloat("Node rounding", &current.nodeRounding, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp)) areThereChanges = true;

			if (ImGui::CollapsingHeader("Graphics")) {

				ImGui::Indent();
				if (ImGui::CollapsingHeader("Input line")) {
					ImGui::Indent();
					if (ImGui::DragFloat("Thickness##Input", &current.lineThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (ImGui::ColorEdit3("Color##Input", &current.line.r)) areThereChanges = true;
					if (ImGui::DragFloat("Outline thickness##Input", &current.lineOutlineThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (current.lineOutlineThickness > 0)
						if (ImGui::ColorEdit3("Outline color##Input", &current.lineOutline.r)) areThereChanges = true;
					if (ImGui::DragFloat("Curvature##Input", &current.lineCurvature, .01f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (ImGui::DragFloat("Alpha##Input", &current.lineAlpha, .01f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;

					ImGui::Unindent();
				}
				if (ImGui::CollapsingHeader("Flow line")) {
					ImGui::Indent();
					if (ImGui::DragFloat("Thickness##Flow", &current.flowlineThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (ImGui::ColorEdit3("Color##Flow", &current.flowline.r)) areThereChanges = true;
					if (ImGui::DragFloat("Outline thickness##Flow", &current.flowlineOutlineThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (current.flowlineOutlineThickness > 0)
						if (ImGui::ColorEdit3("Outline color##Flow", &current.flowlineOutline.r)) areThereChanges = true;
					if (ImGui::DragFloat("Curvature##Flow", &current.flowlineCurvature, .01f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					if (ImGui::DragFloat("Alpha##Flow", &current.flowlineAlpha, .01f, 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp))areThereChanges = true;
					ImGui::Unindent();
				}

				if (ImGui::CollapsingHeader("Grid")) {
					ImGui::Indent();
					if (ImGui::DragFloat("Spacing##Grid", &current.gridSpacing, 2, 2, 1000, "%.3f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic))areThereChanges = true;
					if (ImGui::DragFloat("Thickness##Grid", &current.gridThickness, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic))areThereChanges = true;
					if (ImGui::ColorEdit3("Color##Grid", &current.grid.r)) areThereChanges = true;
					if (ImGui::DragInt("Interval##Grid", &current.gridInterval, .2, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic))areThereChanges = true;
					if (ImGui::DragFloat("Interval Scale##Grid", &current.gridIntervalScale, 1, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic))areThereChanges = true;
					ImGui::Unindent();
				}

				ImGui::Unindent();
			}


			ImGui::Unindent();
		}


		if (areThereChanges) {

			pendingApply = true;
		}


		ImGui::End();
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
