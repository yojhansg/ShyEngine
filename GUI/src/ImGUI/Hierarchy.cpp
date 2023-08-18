#include "Hierarchy.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "SDL.h"
#include <string>
#include "nlohmann/json.hpp"
#include "FileExplorer.h"
#include <fstream>

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
	shouldOpenSavePrefabPopup = false;
}		

void PEditor::Hierarchy::render()
{
	
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	PEditor::Scene* scene = imGUIManager->getScene();
	PEditor::FileExplorer* fileExplorer = imGUIManager->getFileExplorer();
	
	std::unordered_map<int, PEditor::GameObject*> gameObjects = scene->getGameObjects();

	if (focused) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - mainWindowSize.y * 0.5f - 24), ImVec2(mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO, mainWindowSize.y - mainWindowSize.y * 0.1f - 24));
	}
	else {
		ImGui::SetNextWindowSizeConstraints(ImVec2(mainWindowSize.x * 0.1f, mainWindowSize.y - fileExplorer->getSize().y - 24), ImVec2(mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO, mainWindowSize.y - fileExplorer->getSize().y - 24));
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

	if (ImGui::BeginListBox("##", ImVec2(windowWidth - 15, windowHeight - 75))) {
		

		ImGui::Text("Visible");
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(50, 0));
		ImGui::SameLine();
		ImGui::Text("GameObject");
		ImGui::Separator();

		int i = 0;
		for (auto pair : gameObjects)
		{
			if(pair.second->getParent() == nullptr)
				renderGameObjectHierarchy(pair.second, 0);
		}

		ImGui::EndListBox();
	}

	ImGui::End();
}

void PEditor::Hierarchy::handleDragAndDrop(GameObject* source, GameObject* destination)
{
	if (destination == source->getParent()) {
		destination->removeChild(source);

		source->setParent(destination->getParent());

		if (source->getParent() != nullptr) {
			source->getParent()->addChild(source);
		}
	}
	else if (source->getId() != destination->getId() && !source->isAscendant(destination) && destination != source->getParent())
	{
		// Remove source from its current parent
		GameObject* parent = source->getParent();
		if (parent)
		{
			parent->removeChild(source);
		}

		// Set the destination as the new parent
		destination->addChild(source);
	}
}


bool PEditor::Hierarchy::isChildrenTheSelectedObject(GameObject* go)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	PEditor::Scene* scene = imGUIManager->getScene();

	for (auto child : go->getChildren()) {
		if (child.second == scene->getSelectedGameObject()) {
			return true;
		}

		return isChildrenTheSelectedObject(child.second);
	}

	return false;
}


void PEditor::Hierarchy::renderGameObjectHierarchy(GameObject* gameObject, int indentLevel)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	PEditor::Scene* scene = imGUIManager->getScene();

	// Calculate the indentation based on the indent level
	float indentSpacing = 20.0f * indentLevel;
	ImVec2 dummySize = ImVec2(indentSpacing, 0);

	ImGui::Dummy(dummySize);
	ImGui::SameLine();

	bool isVisible = gameObject->isVisible();
	std::string checkboxId = "##" + std::to_string(gameObject->getId());

	if (ImGui::Checkbox(checkboxId.c_str(), &isVisible)) {
		gameObject->setVisible(isVisible);
	}

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(67, 0));
	ImGui::SameLine();

	std::string nameId = gameObject->getName() + "##" + std::to_string(gameObject->getId());


	if (ImGui::Selectable(nameId.c_str(), gameObject == scene->getSelectedGameObject(), ImGuiSelectableFlags_AllowItemOverlap)) {
		scene->setSelectedGameObject(gameObject);
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover))
	{
		ImGui::SetDragDropPayload("payload", &gameObject, sizeof(GameObject*));

		ImGui::TextUnformatted(gameObject->getName().c_str());

		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("payload"))
		{
			GameObject* sourceObject = *(GameObject**)payload->Data;

			handleDragAndDrop(sourceObject, gameObject);
		}

		ImGui::EndDragDropTarget();
	}


	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
		ImGui::OpenPopup("Gameobject Menu##" + gameObject->getId());
	}

	showGameObjectMenu(gameObject);
	showRenamePopup(gameObject);
	showSavePrefabPopup(gameObject);

	if (gameObject->getChildren().size() > 0) {

		ImGui::SameLine();
		
		float treeNodeWidth = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.x;
		ImGui::SetNextItemWidth(treeNodeWidth);

		bool isChildSelected = isChildrenTheSelectedObject(gameObject);
		if (isChildSelected) {
			ImGui::SetNextItemOpen(true);
		}

		if(ImGui::TreeNodeEx((void*)(intptr_t)gameObject->getId(), ImGuiTreeNodeFlags_None, ""))
		{
			// Recursively render children
			for (auto pair : gameObject->getChildren()) {
				renderGameObjectHierarchy(pair.second, indentLevel + 1);
			}

			ImGui::TreePop();
		}

		if (isChildSelected && ImGui::IsItemClicked()) {
			scene->setSelectedGameObject(nullptr);
		}
	}
}

void PEditor::Hierarchy::showRenamePopup(GameObject* gameObject)
{
	if (shouldOpenRenamePopup) {
		ImGui::OpenPopup("Rename Object##" + gameObject->getId());
		shouldOpenRenamePopup = false;
	}

	if (ImGui::BeginPopup("Rename Object##" + gameObject->getId()))
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
	if (ImGui::BeginPopup("Gameobject Menu##" + gameObject->getId()))
	{
		if (ImGui::MenuItem("Create prefab", NULL, false)) {
			shouldOpenSavePrefabPopup = true;
		}

		if (ImGui::MenuItem("Add script", NULL, false)) {

			ImGUIManager::getInstance()->OpenScript("");
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

void PEditor::Hierarchy::showSavePrefabPopup(GameObject* go)
{
	if (shouldOpenSavePrefabPopup)
	{
		ImGui::OpenPopup("Save prefab##" + go->getId());
		shouldOpenSavePrefabPopup = false;
	}

	if (ImGui::BeginPopup("Save prefab##" + go->getId()))
	{
		ImGui::Text(("Insert name for the prefab:"));

		ImGui::Separator();

		static char nameBuffer[256];

		if (ImGui::InputText("Prefab name", nameBuffer, sizeof(nameBuffer)))
		{
		}

		if (ImGui::Button("Ok"))
		{
			if (strlen(nameBuffer) > 0) {
				nlohmann::ordered_json j;

				j = j.parse(go->toJson());


				std::string filePath = "Prefabs/" + std::string(nameBuffer) + ".prefab";

				if (!std::filesystem::exists(filePath)) {
					std::filesystem::create_directories("Prefabs");
				}

				std::ofstream outputFile(filePath);
				if (outputFile.is_open()) {
					outputFile << j.dump(4);
					outputFile.close();
				}
				else {
					//ERROR HANDLING
				}
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
