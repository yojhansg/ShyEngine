#include "Components.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"

#include "ComponentManager.h"


PEditor::Components::Components() : Window("Components", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(417 * mainWindowSize.x / 1920, 755 * mainWindowSize.y / 1080);

	windowOriWidth = windowSize.x;
	windowOriHeight = windowSize.y;

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(846 * mainWindowSize.x / 1080, 20));

	windowOriPosX = windowPosX;
	windowOriPosY = windowPosY;

}

void PEditor::Components::render()
{

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	PEditor::Scene* scene = imGUIManager->getScene();

	GameObject* gameObject = scene->getSelectedGameObject();


	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);


	ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));

	if (gameObject != nullptr) {

		std::list<Component*>* components = gameObject->getComponents();

		gameObject->drawTransformInEditor();

		for (auto it = components->begin(); it != components->end(); it++) {
			(*it)->drawEditor();
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

	if (gameObject != nullptr) {
		if (ImGui::CollapsingHeader("Add component"))
		{



			for (auto& comp : ::Components::ComponentManager::GetAllComponents()) {

				ImGui::Button(comp.first.c_str());
			}



			//ImGui::Button("Transform");
			//ImGui::Button("RigidBody");
			//ImGui::Button("PlayerMovement");
		}
	}

	ImGui::PopStyleColor(6); // reset colors

	ImGui::End();

}
