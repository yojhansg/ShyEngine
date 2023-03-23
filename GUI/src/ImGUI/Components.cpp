#include "Components.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "Transform.h"

PEditor::Components::Components() : Window("Components", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(417 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);

	oriWidth = windowSize.x;
	oriHeight = windowSize.y;

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(846 * mainWindowSize.x / 1080, 20));

	oriPosX = posX;
	oriPosY = posY;

}

void PEditor::Components::render()
{

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	PEditor::Scene* scene = imGUIManager->getScene();

	GameObject* gameObject = scene->getSelectedGameObject();
	std::unordered_map<int, Component*>* components = gameObject->getComponents();

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(width, height));

	ImGui::SetWindowPos(ImVec2(posX, posY));

	for (auto it = components->begin(); it != components->end(); it++) {
		it->second->drawEditor();
	}


	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

	if (ImGui::CollapsingHeader("Add component"))
	{
		ImGui::Button("Transform");
		ImGui::Button("RigidBody");
		ImGui::Button("PlayerMovement");
	}

	ImGui::PopStyleColor(6); // reset colors

	ImGui::End();

}
