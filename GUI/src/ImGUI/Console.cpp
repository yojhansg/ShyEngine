#include "Console.h"
#include "Game.h"
#include "ImGUIManager.h"
#include "imgui.h"

bool PEditor::Console::forceOpen = false;

PEditor::Console::Console() : PEditor::Window("Console", 0)
{
	visible = false;

	gameOpenedLastFrame = false;

	windowWidth = 700;
	windowHeight = 700;

	canBeDisplayedOnTop = true;
}

void PEditor::Console::ForceOpen()
{
	forceOpen = true;
}

void PEditor::Console::render()
{
	bool gameOpened = Game::IsRunning();

	if (!gameOpenedLastFrame && gameOpened)
	{
		visible = true;
	}

	gameOpenedLastFrame = gameOpened;

	if (forceOpen)
	{
		visible = true;
		forceOpen = false;
	}


	if (!visible) return;

	//ImGui::SetNextWindowFocus();

	while (Game::PendingOutput()) {

		std::string str = Game::FlushOutput();

		if (str != "") {

			size_t divide = str.find_last_of(':') + 1;

			std::string principio = str.substr(0, divide);

			std::string fin = str.substr(divide);

			divide = principio.find(']') + 1;
			std::string fecha = principio.substr(0, divide);
			std::string tipo = principio.substr(divide);

			divide = tipo.find(':') + 1;
			std::string objeto = tipo.substr(0, divide);
			std::string script = tipo.substr(divide);



			messages.push_back({ {51, 153, 255}, fecha + objeto, script, fin });
		}
	}


	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
	ImGui::Begin(windowName.c_str(), &visible, flags);

	int messageIdx = 0;
	for (auto& message : messages) {

		const int padding = 30;

		auto contentSize = ImGui::CalcTextSize(
			(message.date + message.type + message.message).c_str(), nullptr, false, windowWidth);

		ImGui::BeginChild(std::to_string(messageIdx).c_str(), ImVec2(windowWidth, contentSize.y));

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(message.color.r, message.color.g, message.color.b, 255));
		ImGui::TextWrapped(message.date.c_str());
		ImGui::SameLine();

		ImGui::TextWrapped(message.type.c_str());
		
		if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				ImGUIManager::getInstance()->OpenScript(message.type.substr(1, message.type.size() - 2));
			}
		}
		
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
		ImGui::TextWrapped(message.message.c_str());
		ImGui::PopStyleColor();

		//ImGui::Separator();

		ImGui::EndChild();
		messageIdx++;
	}


	auto winSize = ImGui::GetWindowSize();
	windowWidth = winSize.x;
	windowHeight = winSize.y;
	ImGui::End();
}
