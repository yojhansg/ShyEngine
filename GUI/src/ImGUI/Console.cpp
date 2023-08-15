#include "Console.h"
#include "Game.h"

#include "imgui.h"


PEditor::Console::Console(): PEditor::Window("Console", 0)
{
	close = false;

	windowWidth = 700;
	windowHeight = 700;

}

void PEditor::Console::render()
{
	if (!Game::IsRunning())
		return;

	//ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
	ImGui::Begin(windowName.c_str(), &close, flags);

	ImGui::Text(Game::GetOutput().c_str());
	
	ImGui::End();
}
