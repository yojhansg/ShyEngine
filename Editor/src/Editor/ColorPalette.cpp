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

	ColorPalette::ColorPalette() : Window("Palette selector", ImGuiWindowFlags_NoCollapse)
	{
		showSavePopup = showErasePopup = false;

		initialisation = true;
		current = {};
		instance = this;
		pendingApply = false;
		changes = false;

		LoadFonts();
		LoadDefaultPalette();
		LoadPalettes("theme");
		LoadPalettes("user-theme");

		windowWidth = 500;
		windowHeight = 500;
		canBeDisplayedOnTop = true;

		SwapPalette(LoadActivePalette());
		Apply();

		initialisation = false;



	}

	ColorPalette::~ColorPalette()
	{
		SaveActivePalette();

		instance = nullptr;
	}

	void ColorPalette::LoadDefaultPalette()
	{
		Palette palette{};

		palette.name = "Engine default";
		palette.text = { 0.9254901960784314f , 0.9411764705882353f , 0.9450980392156863f };
		palette.primary = { 0.1607843137254902f , 0.5019607843137255f , 0.7254901960784314f };
		palette.secondary = { 0.2235294117647059f , 0.3098039215686275f , 0.4117647058823529f };
		palette.background = { 0.1725490196078431f , 0.2431372549019608f , 0.3137254901960784f };
		palette.border = { 0.1725490196078431f , 0.2431372549019608f , 0.3137254901960784f };
		palette.popups = { 0.1294117647058824f , 0.1803921568627451f , 0.2352941176470588f };

		palette.mainWindowBackground = { 0.2f, 0.2f, 0.2f };
		palette.windowAlpha = 0.90f;

		palette.scripting.background = { .12f, .12f, .12f };

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
		palette.scene.objectFrame = { 1, 0, 0 };
		palette.scene.objectFrameWidth = 3;
		palette.scene.UIBorder = { 1, 1, 1 };
		palette.scene.UIBorderHover = { 1, .2f, .2f };
		palette.scene.UIBorderWidth = 5;
		palette.scene.gridInterval = 5;
		palette.scene.gridSpacing = 50;
		palette.scene.gridAlpha = .5f;
		palette.scene.gridThickness = 1;
		palette.scene.grid = { 1, 1, 1 };



		std::string defaultFont = "Montserrat-Regular.ttf_18";

		if (fonts.contains(defaultFont)) {
			palette.fontPtr = fonts[defaultFont];
			palette.fontName = defaultFont;
		}

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

	void ColorPalette::LoadPalettes(const std::string& file) {

		std::string palettePath = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes" + "\\" + file + ".palette";

		std::ifstream fileStream(palettePath);

		if (!fileStream.good() || !json::accept(fileStream)) {
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
			palette.fontName = font;
			palette.fontPtr = fonts.contains(font) ? fonts[font] : nullptr;



#define SafeCheck(value, data)\
		if(data.contains(#value))

#define ReadPaletteColor(value, palette, data) \
		SafeCheck(value, data)\
		palette.value = Color::FromHexString(data[#value].get<std::string>());\
		else palette.value = palettes["Engine default"].value

#define ReadPaletteNumber(value, palette, data, type) \
		SafeCheck(value, data)\
		palette.value = data[#value].get<type>();\
		else palette.value = palettes["Engine default"].value



#define ReadInt(value) ReadPaletteNumber(value, palette, paletteInfo, int)
#define ReadFloat(value) ReadPaletteNumber(value, palette, paletteInfo, float)
#define ReadColor(value) ReadPaletteColor(value, palette, paletteInfo)


			//General 
			ReadColor(text);
			ReadColor(primary);
			ReadColor(secondary);
			ReadColor(background);
			ReadColor(mainWindowBackground);
			ReadColor(popups);
			ReadColor(border);

			ReadFloat(windowRounding);
			ReadFloat(windowBorder);
			ReadFloat(frameBorder);
			ReadFloat(popUpBorder);
			ReadFloat(windowAlpha);


			//Scene
			ReadColor(scene.background);
			ReadFloat(scene.backgroundAlpha);
			ReadColor(scene.frame);
			ReadInt(scene.frameWidth);
			ReadColor(scene.objectFrame);
			ReadInt(scene.objectFrameWidth);

			ReadColor(scene.UIBorder);
			ReadColor(scene.UIBorderHover);
			ReadInt(scene.UIBorderWidth);

			ReadColor(scene.grid);
			ReadInt(scene.gridSpacing);
			ReadInt(scene.gridInterval);
			ReadFloat(scene.gridAlpha);
			ReadInt(scene.gridThickness);
			//Scripting

			ReadColor(scripting.background);
			ReadFloat(scripting.nodeRounding);
			ReadColor(scripting.grid);
			ReadInt(scripting.gridThickness);
			ReadInt(scripting.gridSpacing);
			ReadFloat(scripting.gridIntervalScale);
			ReadInt(scripting.gridInterval);
			ReadColor(scripting.hover);
			ReadFloat(scripting.buttonThickness);
			ReadColor(scripting.line);
			ReadFloat(scripting.lineThickness);
			ReadColor(scripting.lineOutline);
			ReadFloat(scripting.lineOutlineThickness);
			ReadFloat(scripting.lineCurvature);
			ReadFloat(scripting.lineAlpha);
			ReadColor(scripting.flowline);
			ReadFloat(scripting.flowlineThickness);
			ReadColor(scripting.flowlineOutline);
			ReadFloat(scripting.flowlineOutlineThickness);
			ReadFloat(scripting.flowlineCurvature);
			ReadFloat(scripting.flowlineAlpha);



#undef ReadColor
#undef ReadFloat
#undef ReadInt
#undef ReadPaletteNumber
#undef ReadPaletteColor
#undef SafeCheck

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


#define Light(col) toVec4(col, 0.60f)
#define MediumLight(col) toVec4(col, 0.70f)
#define Medium(col) toVec4(col, 0.80f)
#define MediumStrong(col) toVec4(col, 0.90f)
#define Strong(col) toVec4(col, 1)


#define Hover Strong(current.primary)
#define Active MediumStrong(current.primary)
#define Grab MediumLight(current.primary)


		style.Colors[ImGuiCol_Text] = Strong(current.text);
		style.Colors[ImGuiCol_TextDisabled] = Light(current.text);
		style.Colors[ImGuiCol_WindowBg] = toVec4(current.background, current.windowAlpha);
		style.Colors[ImGuiCol_ChildBg] = Light(current.primary);
		style.Colors[ImGuiCol_PopupBg] = MediumStrong(current.popups);
		style.Colors[ImGuiCol_Border] = Strong(current.border);
		style.Colors[ImGuiCol_BorderShadow] = Light(current.border);
		style.Colors[ImGuiCol_FrameBg] = Light(current.secondary);
		style.Colors[ImGuiCol_FrameBgHovered] = Hover;
		style.Colors[ImGuiCol_FrameBgActive] = Active;
		style.Colors[ImGuiCol_TitleBg] = Strong(current.secondary);
		style.Colors[ImGuiCol_TitleBgActive] = Active;
		style.Colors[ImGuiCol_TitleBgCollapsed] = Medium(current.secondary);
		style.Colors[ImGuiCol_MenuBarBg] = Light(current.secondary);
		style.Colors[ImGuiCol_ScrollbarBg] = Strong(current.secondary);
		style.Colors[ImGuiCol_ScrollbarGrab] = Grab;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = Hover;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = Active;
		style.Colors[ImGuiCol_CheckMark] = Medium(current.primary);
		style.Colors[ImGuiCol_SliderGrab] = Grab;
		style.Colors[ImGuiCol_SliderGrabActive] = Active;
		style.Colors[ImGuiCol_Button] = Light(current.primary);
		style.Colors[ImGuiCol_ButtonHovered] = Hover;
		style.Colors[ImGuiCol_ButtonActive] = Active;
		style.Colors[ImGuiCol_Header] = Medium(current.primary);
		style.Colors[ImGuiCol_HeaderHovered] = Hover;
		style.Colors[ImGuiCol_HeaderActive] = Active;

#define Magenta ImVec4(1, 0, 1, 1)

		style.Colors[ImGuiCol_Separator] = Strong(current.primary);
		style.Colors[ImGuiCol_SeparatorHovered] = Strong(current.primary);
		style.Colors[ImGuiCol_SeparatorActive] = Strong(current.primary);


		style.Colors[ImGuiCol_ResizeGrip] = Light(current.secondary);
		style.Colors[ImGuiCol_ResizeGripHovered] = Hover;
		style.Colors[ImGuiCol_ResizeGripActive] = Active;

		style.Colors[ImGuiCol_Tab] = MediumLight(current.secondary);
		style.Colors[ImGuiCol_TabHovered] = Hover;
		style.Colors[ImGuiCol_TabActive] = Active;
		style.Colors[ImGuiCol_TabUnfocused] = Light(current.secondary);
		style.Colors[ImGuiCol_TabUnfocusedActive] = MediumLight(current.primary);


		style.Colors[ImGuiCol_DockingPreview] = MediumLight(current.primary);
		style.Colors[ImGuiCol_DockingEmptyBg] = MediumStrong(current.primary);


		style.Colors[ImGuiCol_PlotLines] = Light(current.text);
		style.Colors[ImGuiCol_PlotLinesHovered] = Hover;
		style.Colors[ImGuiCol_PlotHistogram] = Light(current.text);
		style.Colors[ImGuiCol_PlotHistogramHovered] = Hover;
		style.Colors[ImGuiCol_TextSelectedBg] = Light(current.primary);


		style.Colors[ImGuiCol_DragDropTarget] = Strong(current.primary);


		style.DockingSeparatorSize = current.windowBorder;
		style.WindowRounding = current.windowRounding;
		style.WindowBorderSize = current.windowBorder;
		style.FrameBorderSize = current.frameBorder;
		style.PopupBorderSize = current.popUpBorder;

		pendingApply = false;
	}

	std::string ColorPalette::LoadActivePalette()
	{
		std::string save = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes\\Active.palette";

		std::ifstream stream(save);

		if (!stream.is_open())
			return "Engine default";

		std::stringstream str;

		str << stream.rdbuf();

		stream.close();
		return str.str();
	}

	void ColorPalette::SaveActivePalette()
	{
		std::string save = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes\\Active.palette";

		std::ofstream stream(save);

		if (!stream.is_open())
			return;


		stream << current.name;

		stream.close();

	}

	void ColorPalette::SavePalette()
	{
		std::string palettePath = ResourcesManager::EDITORASSETSFOLDER + "\\Palettes\\user-theme.palette";

		std::ifstream fileStream(palettePath);

		nlohmann::ordered_json root;

		if (fileStream.good() && json::accept(fileStream)) {

			fileStream.clear();
			fileStream.seekg(0);

			root = json::parse(fileStream);
			fileStream.close();
		}

		nlohmann::ordered_json palette;

		palette["name"] = paletteName;
		palette["font"] = current.fontName;

#define WriteValue(value) palette[#value] = current.value

#define WriteColor(col) palette[#col] = current.col.ToHexString()
#define WriteFloat WriteValue
#define WriteInt WriteValue

		//General 
		WriteColor(text);
		WriteColor(primary);
		WriteColor(secondary);
		WriteColor(background);
		WriteColor(mainWindowBackground);
		WriteColor(popups);
		WriteColor(border);

		WriteFloat(windowRounding);
		WriteFloat(windowBorder);
		WriteFloat(frameBorder);
		WriteFloat(popUpBorder);
		WriteFloat(windowAlpha);


		//Scene
		WriteColor(scene.background);
		WriteFloat(scene.backgroundAlpha);
		WriteColor(scene.frame);
		WriteInt(scene.frameWidth);
		WriteColor(scene.objectFrame);
		WriteInt(scene.objectFrameWidth);

		WriteColor(scene.UIBorder);
		WriteColor(scene.UIBorderHover);
		WriteInt(scene.UIBorderWidth);

		WriteColor(scene.grid);
		WriteInt(scene.gridSpacing);
		WriteInt(scene.gridInterval);
		WriteFloat(scene.gridAlpha);
		WriteInt(scene.gridThickness);
		//Scripting

		WriteColor(scripting.background);
		WriteFloat(scripting.nodeRounding);
		WriteColor(scripting.grid);
		WriteInt(scripting.gridThickness);
		WriteInt(scripting.gridSpacing);
		WriteFloat(scripting.gridIntervalScale);
		WriteInt(scripting.gridInterval);
		WriteColor(scripting.hover);
		WriteFloat(scripting.buttonThickness);
		WriteColor(scripting.line);
		WriteFloat(scripting.lineThickness);
		WriteColor(scripting.lineOutline);
		WriteFloat(scripting.lineOutlineThickness);
		WriteFloat(scripting.lineCurvature);
		WriteFloat(scripting.lineAlpha);
		WriteColor(scripting.flowline);
		WriteFloat(scripting.flowlineThickness);
		WriteColor(scripting.flowlineOutline);
		WriteFloat(scripting.flowlineOutlineThickness);
		WriteFloat(scripting.flowlineCurvature);
		WriteFloat(scripting.flowlineAlpha);

#undef WriteFloat
#undef WriteInt
#undef WriteColor
#undef WriteValue


		root.push_back(palette);


		std::ofstream outputsfileStream(palettePath);

		outputsfileStream << root.dump(4);
		outputsfileStream.close();

		current.name = paletteName;
	}


	void ColorPalette::Open()
	{
		instance->Show();

	}

	void ColorPalette::Behaviour()
	{
		ImGui::TextWrapped("Choose theme");

		if (ImGui::BeginCombo("Palette selector", current.name.c_str())) {
			for (auto& palette : palettes) {

				//TODO: push de los colores


				//ImGui::PushStyleColor(ImGuiCol_Text, toVec4(palette.second.text, 1));
				//ImGui::PushStyleColor(ImGuiCol_Header, toVec4(palette.second.background, 1));
				//ImGui::PushFont(palette.second.fontPtr);


				if (ImGui::Selectable(palette.first.c_str(), current.name == palette.second.name)) {

					SwapPalette(palette.first);
				}


				//ImGui::PopStyleColor(2);
				//ImGui::PopFont();
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Font selector", current.fontName.c_str())) {


			for (auto& font : fonts) {


				if (ImGui::Selectable(font.first.c_str(), current.fontName == font.first)) {

					
					current.fontName = font.first;
					current.fontPtr = font.second;
					changes = true;
				}
			}

			ImGui::EndCombo();
		}

		//ImGui::ShowFontSelector("Font");

#define ShowColor(name, color) if (ImGui::ColorEdit3(filename, &color.r)) areThereChanges = true;
#define ShowFloat(name, value, speed, min, max)\
	if (ImGui::DragFloat(filename, &value), speed, min, max, "%.3f", ImGuiSliderFlags_AlwaysClamp) {\
	 areThereChanges = true; if(value < min) value = min; if(value > max) value = max;}



		if (ImGui::CollapsingHeader("Editor")) {

			ImGui::Indent();

			Col("Tex", &current.text);
			Col("Primary", &current.primary);
			Col("Secondary", &current.secondary);
			Col("Background", &current.background);
			Col("Pop ups", &current.popups);
			Col("Border", &current.border);
			Float("Window rounding", &current.windowRounding);
			Float("Window border size", &current.windowBorder);
			Float("Frame  border size", &current.frameBorder, 0, 10, 0.01);
			Float("Pop up border size", &current.popUpBorder);
			NormalizedFloat("Window transparency", &current.windowAlpha);


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

			Col("Background", &current.scripting.background);
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


		if (ImGui::Button("Save current palette")) {

			showSavePopup = true;
			paletteName[0] = '\0';
		}

		/*if (ImGui::Button("Remove palette")) {

			showErasePopup = true;
			paletteName[0] = '\0';
		}*/


		if (showSavePopup)
		{
			//TODO: center

			ImGui::OpenPopup("Save color palette");

		}

		if (ImGui::BeginPopupModal("Save color palette", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {


			ImGui::InputText("New palette name", paletteName, 16);


			if (ImGui::Button("Save")) {

				showSavePopup = false;
				ImGui::CloseCurrentPopup();

				//TODO: llamar a save
				SavePalette();

			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {

				showSavePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}



		//if (showErasePopup)
		//{
		//	//TODO: center

		//	ImGui::OpenPopup("Erase color palette");
		//}

		//if (ImGui::BeginPopupModal("Erase color palette", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		//{


		//	ImGui::InputText("Palette to erase", paletteName, 16);

		//	if (ImGui::Button("Erase")) {

		//		showErasePopup = false;
		//		ImGui::CloseCurrentPopup();

		//		//TODO: llamar a erase

		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button("Cancel")) {

		//		showErasePopup = false;
		//		ImGui::CloseCurrentPopup();
		//	}

		//	ImGui::EndPopup();
		//}

	}


	void ColorPalette::Col(const char* name, Color* value)
	{
		if (ImGui::ColorEdit3(name, &value->r, ImGuiColorEditFlags_PickerHueWheel))
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

	std::string ColorPalette::Color::ToHexString()
	{
		std::stringstream ss;

		ss << "#" <<
			std::hex << std::setw(2) << std::setfill('0') << int(r * 255) <<
			std::hex << std::setw(2) << std::setfill('0') << int(g * 255) <<
			std::hex << std::setw(2) << std::setfill('0') << int(b * 255);

		return ss.str();
	}

}
