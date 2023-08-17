#include "ColorPalette.h"
#include "imgui.h"

#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"

using nlohmann::json;

PEditor::ColorPalette* PEditor::ColorPalette::instance = nullptr;


PEditor::ColorPalette::ColorPalette(const std::string& path) : Window("Palette selector", 0), path(path)
{
	initialisation = true;
	current = {};
	instance = this;
	visible = false;
	pendingApply = false;

	LoadDefaultPalette();
	LoadPalettes();

	windowWidth = 500;
	windowHeight = 500;
	canBeDisplayedOnTop = true;

	SwapPalette("Default");
	Apply();

	initialisation = false;
}

PEditor::ColorPalette::~ColorPalette()
{
	instance = nullptr;
}

void PEditor::ColorPalette::LoadDefaultPalette()
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

	palette.font = "Fonts/Montserrat-Regular.ttf";
	palette.fontSize = 18.f;
	palette.fontPtr = ImGui::GetIO().Fonts->AddFontFromFileTTF(palette.font.c_str(), palette.fontSize);

	palettes.emplace("Default", palette);
}


void PEditor::ColorPalette::LoadPalettes() {

	std::ifstream fileStream(path + ".palette");

	if (!fileStream.good() || !json::accept(fileStream))
	{
		std::cout << "No se ha podido cargar la paleta" << std::endl;
		return;
	}

	fileStream.clear();
	fileStream.seekg(0);

	json root = json::parse(fileStream);
	fileStream.close();

	for (auto& paletteInfo : root) {

		Palette palette{};
		std::string name = paletteInfo["name"].get<std::string>();
		palette.font = paletteInfo["font"].get<std::string>();
		palette.fontSize = paletteInfo["fontSize"].get<int>();

		palette.fontPtr = ImGui::GetIO().Fonts->AddFontFromFileTTF(palette.font.c_str(), palette.fontSize);
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

void PEditor::ColorPalette::SwapPalette(const std::string& palette)
{
	current = palettes[palette];
	pendingApply = true;
}

void PEditor::ColorPalette::Apply()
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

void PEditor::ColorPalette::Serialize()
{
	//TODO: serializar el valor actual
}

void PEditor::ColorPalette::Open()
{
	instance->open = true;
}

void PEditor::ColorPalette::render()
{
	if (open) {

		open = false;
		visible = true;

		ImGui::SetNextWindowFocus();
	}

	if (!visible) return;


	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
	ImGui::Begin(windowName.c_str(), &visible, flags);


	ImGui::TextWrapped("Que pasa nene? Te toca elegir el tema venga");


	for (auto& palette : palettes) {

		//TODO: push de los colores
		ImGui::PushFont(palette.second.fontPtr);


		if (ImGui::Button(palette.first.c_str())) {

			SwapPalette(palette.first);
		}


		ImGui::PopFont();
	}

	bool areThereChanges = false;

	if (ImGui::CollapsingHeader("Editor")) {

		if (ImGui::ColorEdit3("Text", &current.text.r)) areThereChanges = true;

		if (ImGui::ColorEdit3("Area", &current.area.r)) areThereChanges = true;

		if (ImGui::ColorEdit3("Body", &current.body.r)) areThereChanges = true;

		if (ImGui::ColorEdit3("Head", &current.head.r)) areThereChanges = true;

		if (ImGui::ColorEdit3("Pop ups", &current.popups.r)) areThereChanges = true;

		if (ImGui::InputFloat("Window rounding", &current.windowRounding)) areThereChanges = true;
	}

	if (ImGui::CollapsingHeader("Scripting")) {

		if (ImGui::ColorEdit3("Background", &current.scriptBackground.r)) areThereChanges = true;
		if (ImGui::ColorEdit3("Hover", &current.hover.r)) areThereChanges = true;
		if (ImGui::InputFloat("Button thickness", &current.buttonThickness)) areThereChanges = true;
		if (ImGui::InputFloat("Node rounding", &current.nodeRounding)) areThereChanges = true;


		if (ImGui::ColorEdit3("Lines", &current.line.r)) areThereChanges = true;
		if (ImGui::InputFloat("Line thickness", &current.lineThickness))areThereChanges = true;
		if(ImGui::InputFloat("Line outline",&current.lineOutlineThickness))areThereChanges = true;
		if(current.lineOutlineThickness > 0)
			if (ImGui::ColorEdit3("Outline color", &current.lineOutline.r)) areThereChanges = true;
		if (ImGui::InputFloat("Line curvature", &current.lineCurvature))areThereChanges = true;
		if (ImGui::InputFloat("Line alpha", &current.lineAlpha))areThereChanges = true;


		if (ImGui::ColorEdit3("Flow lines", &current.flowline.r)) areThereChanges = true;
		if (ImGui::InputFloat("Flow line thickness", &current.flowlineThickness))areThereChanges = true;
		if (ImGui::InputFloat("Flow line outline", &current.flowlineOutlineThickness))areThereChanges = true;
		if (current.flowlineOutlineThickness > 0)
			if (ImGui::ColorEdit3("Flow outline color", &current.flowlineOutline.r)) areThereChanges = true;
		if (ImGui::InputFloat("Flow line curvature", &current.flowlineCurvature))areThereChanges = true;
		if (ImGui::InputFloat("Flow line alpha", &current.flowlineAlpha))areThereChanges = true;



		if (ImGui::ColorEdit3("Grid", &current.grid.r)) areThereChanges = true;
		if (ImGui::InputFloat("Grid spacing", &current.gridSpacing))areThereChanges = true;
		if (ImGui::InputFloat("Grid thickness", &current.gridThickness))areThereChanges = true;
		if (ImGui::InputInt("Grid interval", &current.gridInterval))areThereChanges = true;
		if (ImGui::InputFloat("Grid interval scale", &current.gridIntervalScale))areThereChanges = true;

	}


	if (areThereChanges) {

		pendingApply = true;
	}


	ImGui::End();
}

void PEditor::ColorPalette::ApplyPalette()
{
	if (instance->pendingApply)
		instance->Apply();
}

PEditor::ColorPalette::Palette& PEditor::ColorPalette::GetCurrentPalette()
{
	return instance->current;
}

PEditor::ColorPalette::Color PEditor::ColorPalette::Color::FromHexString(const std::string& hex)
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
