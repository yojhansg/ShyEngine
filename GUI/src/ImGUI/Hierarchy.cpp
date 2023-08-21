#include "Hierarchy.h"

#include "nlohmann/json.hpp"
#include "FileExplorer.h"
#include "GameObject.h"
#include "Editor.h"
#include "Scene.h"
#include "SDL.h"

#include <imgui.h>
#include <fstream>
#include <string>


namespace ShyEditor {

	Hierarchy::Hierarchy() : Window("Hierarchy", NoMove | NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus)
	{
		editor = Editor::getInstance();
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		windowOriWidth = mainWindowSize.x * HIERARCHY_WIN_WIDTH_RATIO;
		windowOriHeight = mainWindowSize.y * HIERARCHY_WIN_HEIGHT_RATIO;

		float menuBarHeight = ImGui::GetFrameHeight();

		windowOriPosX = 0;
		windowOriPosY = menuBarHeight;

		setSize(ImVec2(windowOriWidth, windowOriHeight));
		setPosition(ImVec2(windowOriPosX, windowOriPosY));

		shouldOpenRenamePopup = false;
		shouldOpenSavePrefabPopup = false;

		docked = true;
	}

	void Hierarchy::Behaviour()
	{
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		Scene* scene = editor->getScene();
		FileExplorer* fileExplorer = editor->getFileExplorer();

		std::unordered_map<int, GameObject*> gameObjects = scene->getGameObjects();


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
				if (pair.second->getParent() == nullptr)
					renderGameObjectHierarchy(pair.second, 0);
			}

			ImGui::EndListBox();
		}

	}

	void Hierarchy::handleDragAndDrop(GameObject* source, GameObject* destination)
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


	bool Hierarchy::isChildrenTheSelectedObject(GameObject* go)
	{
		Scene* scene = editor->getScene();

		for (auto child : go->getChildren()) {
			if (child.second == scene->getSelectedGameObject()) {
				return true;
			}

			return isChildrenTheSelectedObject(child.second);
		}

		return false;
	}


	void Hierarchy::renderGameObjectHierarchy(GameObject* gameObject, int indentLevel)
	{
		Scene* scene = editor->getScene();

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

			if (ImGui::TreeNodeEx((void*)(intptr_t)gameObject->getId(), ImGuiTreeNodeFlags_None, ""))
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

	void Hierarchy::showRenamePopup(GameObject* gameObject)
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

	void Hierarchy::showGameObjectMenu(GameObject* gameObject)
	{
		if (ImGui::BeginPopup("Gameobject Menu##" + gameObject->getId()))
		{
			if (ImGui::MenuItem("Create prefab", NULL, false)) {
				shouldOpenSavePrefabPopup = true;
			}

			if (ImGui::MenuItem("Add script", NULL, false)) {

				editor->OpenScript("");
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

	void Hierarchy::showSavePrefabPopup(GameObject* go)
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

}
