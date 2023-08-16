#include "Preferences.h"
#include "Game.h"
#include "ImGUIManager.h"
#include "imgui.h"


bool PEditor::Preferences::open = false;

PEditor::Preferences::Preferences() : PEditor::Window("Preferences", 0)
{
	visible = false;
	windowWidth = 700;
	windowHeight = 700;

	canBeDisplayedOnTop = true;
}

void PEditor::Preferences::Open()
{
	open = true;
}

void PEditor::Preferences::render()
{
	if (open) {

		open = false;
		visible = true;

		ImGui::SetNextWindowFocus();
	}

	if (!visible) return;

	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
	ImGui::Begin(windowName.c_str(), &visible, flags);


	ImGui::TextWrapped("Hola! Estas son las preferencias. Aqui es donde estaran disponibles a modificar los "
		"distintos valores del editor. Vuelve pronto!");


	ImGui::End();
}
