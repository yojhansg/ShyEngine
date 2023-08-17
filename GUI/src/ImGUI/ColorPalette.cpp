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

	loadedFonts = 0;

	LoadDefaultPalette();
	LoadPalettes();

	windowWidth = 500;
	windowHeight = 500;

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

	palette.line = { 1, 1, 1 };
	palette.grid = { .4f, .4f, .4f };
	palette.scriptBackground = { .12f, .12f, .12f };


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

#undef ReadPaletteValue

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

	style.WindowRounding = 0;

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


	float color[3]{ 0 ,0 ,0 };
#define Color2FloatArr(color, arr) arr[0] = color.r;arr[1] = color.g;arr[2] = color.b
#define FloatArr2Color(color, arr) color.r = arr[0]; color.g = arr[1]; color.b = arr[1]

	//Color2FloatArr(current.text, color);
	//ImGui::ColorEdit3("Text", &current.text.r);
	//FloatArr2Color(current.text, color);

	//Color2FloatArr(current.area, color);
	//ImGui::ColorEdit3("Area", color);
	//FloatArr2Color(current.area, color);

	//Color2FloatArr(current.body, color);
	//ImGui::ColorEdit3("Body", color);
	//FloatArr2Color(current.body, color);

	//Color2FloatArr(current.head, color);
	//ImGui::ColorEdit3("Head", color);
	//FloatArr2Color(current.head, color);

	//Color2FloatArr(current.popups, color);
	//ImGui::ColorEdit3("Pop ups", color);
	//FloatArr2Color(current.popups, color);


	ImGui::ColorEdit3("Text", &current.text.r);

	ImGui::ColorEdit3("Area", &current.area.r);

	ImGui::ColorEdit3("Body", &current.body.r);

	ImGui::ColorEdit3("Head", &current.head.r);

	ImGui::ColorEdit3("Pop ups", &current.popups.r);

	if (ImGui::Button("Apply")) {

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
