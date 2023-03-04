#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "Image.h"
#include "Scene.h"

PEditor::Hierarchy::Hierarchy() : Window("Hierarchy", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(412 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(0, 20));

}		

void PEditor::Hierarchy::render()
{
	
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	PEditor::Scene* scene = imGUIManager->getScene();
	
	std::vector<Image*> gameObjects = scene->getGameObjects();

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(width, height));

	ImGui::SetWindowPos(ImVec2(posX, posY));

	for (auto component : components)
	{
		component->render();
	}

	if (ImGui::BeginListBox("##", ImVec2(width - 15, height - 35))) {

		for (auto gameObject : gameObjects)
		{
			ImGui::Selectable("Gameobject: 1");
		}

		ImGui::EndListBox();
	}


	ImGui::End();

}
