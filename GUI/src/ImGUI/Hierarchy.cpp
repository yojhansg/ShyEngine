#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"

PEditor::Hierarchy::Hierarchy() : Window("Hierarchy", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(413 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);
	oriWidth = windowSize.x;
	oriHeight = windowSize.y;

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(0, 20));

	oriPosX = posX;
	oriPosY = posY;
}		

void PEditor::Hierarchy::render()
{
	
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	PEditor::Scene* scene = imGUIManager->getScene();
	
	std::vector<GameObject*> gameObjects = scene->getGameObjects();

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(width, height));

	ImGui::SetWindowPos(ImVec2(posX, posY));

	if (ImGui::BeginListBox("##", ImVec2(width - 15, height - 35))) {

		for (auto gameObject : gameObjects)
		{
			ImGui::Selectable("Gameobject: 1");
		}

		ImGui::EndListBox();
	}


	ImGui::End();

}
