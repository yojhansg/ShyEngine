#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"
#include <string>

PEditor::Hierarchy::Hierarchy() : Window("Hierarchy", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(413 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);
	windowOriWidth = windowSize.x;
	windowOriHeight = windowSize.y;

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(0, 20));

	windowOriPosX = windowPosX;
	windowOriPosY = windowPosY;
}		

void PEditor::Hierarchy::render()
{
	
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	PEditor::Scene* scene = imGUIManager->getScene();
	
	std::vector<GameObject*> gameObjects = scene->getGameObjects();

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));

	if (ImGui::BeginListBox("##", ImVec2(windowWidth - 15, windowHeight - 35))) {

		int i = 0;
		for (auto gameObject : gameObjects)
		{
			std::string selectableId = gameObject->getName() + "##" + std::to_string(i);

			if (ImGui::Selectable(selectableId.c_str(), gameObject == scene->getSelectedGameObject()))
			{
				scene->setSelectedGameObject(gameObject);
			}

			i++;
		}

		ImGui::EndListBox();
	}


	ImGui::End();

}
