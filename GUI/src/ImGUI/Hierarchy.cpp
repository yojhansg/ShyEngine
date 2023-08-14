#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "SDL.h"
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

		ImGui::Text("Visible");
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(50, 0));
		ImGui::SameLine();
		ImGui::Text("GameObject");
		ImGui::Separator();

		int i = 0;
		for (auto gameObject : gameObjects)
		{
			ImGui::Dummy(ImVec2(7, 0));
			ImGui::SameLine();

			bool isVisible = gameObject->isVisible();
			std::string checkboxId = "##" + std::to_string(i);

			if (ImGui::Checkbox(checkboxId.c_str(), &isVisible))
			{
				gameObject->setVisible(isVisible);
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(67, 0)); 
			ImGui::SameLine();

			std::string nameId = gameObject->getName() + "##" + std::to_string(i);

			if (ImGui::Selectable(nameId.c_str(), gameObject == scene->getSelectedGameObject()))
			{
				scene->setSelectedGameObject(gameObject);
			}


			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
				ImGui::OpenPopup("Gameobject Menu");
			}

			bool shouldOpenRenamePopup = false;

			if (ImGui::BeginPopup("Gameobject Menu"))
			{
				if (ImGui::MenuItem("Create prefab", NULL, false)) {

				}

				if (ImGui::MenuItem("Add script", NULL, false)) {
					ImGUIManager::getInstance()->changeEditorState(ImGUIManager::EDITOR_STATE::SCRIPTING_WINDOW);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Rename", NULL, false))
				{
					shouldOpenRenamePopup = true;
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Delete", NULL, false)) {
					gameObject->toDelete();
				}

				ImGui::EndMenu();

			}

			if (shouldOpenRenamePopup)
			{
				ImGui::OpenPopup("Rename Object");
				shouldOpenRenamePopup = false;
			}

			if (ImGui::BeginPopup("Rename Object"))
				{
				ImGui::Text(("Insert new name for GameObject: " + gameObject->getName()).c_str());

				ImGui::Separator();

				static char nameBuffer[256];  // Buffer to hold the new name

				// Display an input text field for renaming
				if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
				{
				}

				if (ImGui::Button("Ok"))
				{
					if (strlen(nameBuffer) > 0) {
						gameObject->setName(nameBuffer);
					}

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			i++;
		}

		ImGui::EndListBox();
	}

	ImGui::End();
}