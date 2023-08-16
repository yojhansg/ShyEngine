#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "SDL.h"
#include <string>
#include "FileExplorer.h"

PEditor::Hierarchy::Hierarchy() : Window("Hierarchy", NoMove | NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	windowOriWidth = mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO;
	windowOriHeight = mainWindowSize.y * HIERARCHY_WIN_HEIGHT_RATIO;

	float menuBarHeight = ImGui::GetFrameHeight();

	windowOriPosX = 0;
	windowOriPosY = menuBarHeight;

	setSize(ImVec2(windowOriWidth, windowOriHeight));
	setPosition(ImVec2(windowOriPosX, windowOriPosY));
	
	shouldOpenRenamePopup = false;
}		

void PEditor::Hierarchy::render()
{
	
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	PEditor::Scene* scene = imGUIManager->getScene();
	PEditor::FileExplorer* fileExplorer = imGUIManager->getFileExplorer();
	
	std::vector<GameObject*> gameObjects = scene->getGameObjects();

	if (focused) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - mainWindowSize.y * 0.5f - 25), ImVec2(mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO, mainWindowSize.y - mainWindowSize.y * 0.1f - 25));
	}
	else {
		ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - fileExplorer->getSize().y - 25), ImVec2(mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO, mainWindowSize.y - fileExplorer->getSize().y - 25));
	}

	focused = false;

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);

	if (ImGui::IsWindowFocused()) {
		focused = true;
	}

	ImVec2 imGUIWindowSize = ImGui::GetWindowSize();
	ImVec2 imGUIWindowPos = ImGui::GetWindowPos();
	windowWidth = imGUIWindowSize.x;
	windowHeight = imGUIWindowSize.y;
	windowPosX = imGUIWindowPos.x;
	windowPosY = imGUIWindowPos.y;

	float menuBarHeight = ImGui::GetFrameHeight();

	ImGui::SetWindowPos(ImVec2(0, menuBarHeight));
	ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

	if (ImGui::Button("Add empty gameobject")) {
		scene->addGameObject("");
	}

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

			//Checkbox to make the object visible/invisible
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

			//Selectable to select the gameobject in the scene
			if (ImGui::Selectable(nameId.c_str(), gameObject == scene->getSelectedGameObject()))
			{
				scene->setSelectedGameObject(gameObject);
			}

			//If selectable is hovered and rightClicked opens the gameobject menu popup
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
				ImGui::OpenPopup("Gameobject Menu");
			}

		
			//Shows the popups when opened
			showGameObjectMenu(gameObject);
			showRenamePopup(gameObject);

			i++;
		}

		ImGui::EndListBox();
	}

	ImGui::End();
}

void PEditor::Hierarchy::showRenamePopup(GameObject* gameObject)
{
	if (shouldOpenRenamePopup) {
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
}

void PEditor::Hierarchy::showGameObjectMenu(GameObject* gameObject)
{
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
}
