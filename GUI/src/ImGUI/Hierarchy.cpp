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

#include "CheckML.h"


namespace ShyEditor {

	Hierarchy::Hierarchy() : Window("Hierarchy", ImGuiWindowFlags_NoCollapse)
	{
		shouldOpenRenamePopup = false;
		shouldOpenSavePrefabPopup = false;

		docked = true;
	}

	void Hierarchy::Behaviour()
	{
		Editor* editor = Editor::getInstance();

		ImVec2 mainWindowSize = editor->getMainWindowSize();

		Scene* scene = editor->getScene();
		FileExplorer* fileExplorer = editor->getFileExplorer();


		if (ImGui::Button("Add transform")) {
			scene->SetSelectedGameObject(scene->AddGameObject(""));
		}

		if (ImGui::Button("Add overlay")) {
			scene->SetSelectedGameObject(scene->AddOverlay(""));
		}


		ImGui::SeparatorText("Transforms");

		int i = 0;
		for (auto& pair : scene->getGameObjects())
		{
			if (pair.second->getParent() == nullptr) {


				RenderGameObject(pair.second, "Transform");
			}
		}

		ImGui::SeparatorText("Overlays");

		for (auto& overlay : scene->getOverlays()) {

			if (overlay->getParent() == nullptr) {

				RenderGameObject(overlay, "Overlay");
			}
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
		Scene* scene = Editor::getInstance()->getScene();

		for (auto child : go->getChildren()) {
			if (child.second == scene->GetSelectedGameObject()) {
				return true;
			}

			return isChildrenTheSelectedObject(child.second);
		}

		return false;
	}


	void Hierarchy::RenderGameObject(GameObject* gameObject, const char* type)
	{
		Scene* scene = Editor::getInstance()->getScene();


		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap;

		if (gameObject->getChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (scene->GetSelectedGameObject() == gameObject)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(gameObject);
		bool isOpen = ImGui::TreeNodeEx("##Root", flags);

		ImGui::SameLine();
		ImGui::Text(gameObject->getName().c_str());

		//char buffer[256]{};

		//memcpy(buffer, gameObject->getName().c_str(), 256);
		//ImGui::SameLine();
		//if (ImGui::InputText("##RenameInPlace", buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {


		//}

		//gameObject->setName(buffer);


		if (ImGui::IsItemClicked()) {

			scene->SetSelectedGameObject(gameObject);
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("Gameobject Menu##" + gameObject->getId());
		}

		showGameObjectMenu(gameObject);
		showRenamePopup(gameObject);
		showSavePrefabPopup(gameObject);


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {

			ImGui::SetDragDropPayload(type, &gameObject, sizeof(GameObject*));
			ImGui::EndDragDropSource();
		}


		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				GameObject* sourceObject = *(GameObject**)payload->Data;

				handleDragAndDrop(sourceObject, gameObject);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("Gameobject Menu##" + gameObject->getId());
		}

		if (isOpen) {


			for (auto& child : gameObject->getChildren()) {


				RenderGameObject(child.second, type);
			}


			ImGui::TreePop();
		}

		ImGui::PopID();
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

				Editor::getInstance()->OpenScript("");
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
