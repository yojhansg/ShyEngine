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

#include "PrefabManager.h"


namespace ShyEditor {

	Hierarchy::Hierarchy() : Window("Hierarchy", ImGuiWindowFlags_NoCollapse)
	{
		shouldOpenRenamePopup = false;
		shouldOpenSavePrefabPopup = false;

		docked = true;

		acceptAssetDrop = true;

		copiedObject = nullptr;
	}

	Hierarchy::~Hierarchy()
	{
		if (copiedObject != nullptr) {
			delete copiedObject;
		}

		Window::~Window();
	}

	void Hierarchy::Behaviour()
	{
		Editor* editor = Editor::getInstance();

		ImVec2 mainWindowSize = editor->getMainWindowSize();

		Scene* scene = editor->getScene();
		FileExplorer* fileExplorer = editor->getFileExplorer();


		if (ImGui::Button("Create world entity")) {
			scene->SetSelectedGameObject(scene->AddGameObject("Empty entity"));
		}

		ImGui::SameLine();
		if (ImGui::Button("Create UI element")) {
			scene->SetSelectedGameObject(scene->AddOverlay("Empty UI element"));
		}


		ImGui::SeparatorText("World");

		int i = 0;
		for (auto& pair : scene->getGameObjects())
		{
			if (pair.second->GetParent() == nullptr) {


				RenderGameObject(pair.second, "Transform");
			}
		}

		ImGui::SeparatorText("UI");

		for (auto& overlay : scene->getOverlays()) {

			if (overlay->GetParent() == nullptr) {

				RenderGameObject(overlay, "Overlay");
			}
		}

	}

	void Hierarchy::HandleInput(SDL_Event* event)
	{

		if (event->type == SDL_KEYDOWN) {

			int dir = 0;
			if (event->key.keysym.scancode == SDL_SCANCODE_UP)
			{
				dir = -1;

			}
			else if (event->key.keysym.scancode == SDL_SCANCODE_DOWN)
			{
				dir = 1;
			}

			if (dir != 0) {

				Scene* scene = Editor::getInstance()->getScene();

				auto& objects = scene->getGameObjects();
				auto& overlays = scene->getOverlays();

				if (objects.size() == 0 && overlays.size() == 0) return;


				auto selectedGo = scene->GetSelectedGameObject();

				if (selectedGo == nullptr) return;

				if (selectedGo->IsTransform()) {
					auto goIt = objects.find(selectedGo->GetId());

					if (dir < 0) {

						if (goIt == objects.begin()) {
							if (overlays.size() > 0) {
								scene->SetSelectedGameObject(*overlays.rbegin());
							}
							else {
								scene->SetSelectedGameObject(objects.rbegin()->second);
							}
						}				
						else {
							scene->SetSelectedGameObject(std::prev(goIt)->second);
						}
					}

					else {

						if (std::next(goIt) == objects.end()) {
							if (overlays.size() > 0) {
								scene->SetSelectedGameObject(*overlays.begin());
							}
							else {
								scene->SetSelectedGameObject(objects.begin()->second);
							}
						}
						else {
							scene->SetSelectedGameObject(std::next(goIt)->second);
						}
					}
				}
				else {

					auto goIt = std::find(overlays.begin(), overlays.end(), selectedGo);

					if (dir < 0) {

						if (goIt == overlays.begin()) {
							if (objects.size() > 0) {
								scene->SetSelectedGameObject(objects.rbegin()->second);
							}
							else {
								scene->SetSelectedGameObject(*overlays.rbegin());
							}
						}
						else {
							scene->SetSelectedGameObject(*std::prev(goIt));

						}
					}

					else {

						if (std::next(goIt) == overlays.end()) {
							if (objects.size() > 0) {
								scene->SetSelectedGameObject(objects.begin()->second);
							}
							else {
								scene->SetSelectedGameObject(*overlays.begin());
							}
						}
						else {
							scene->SetSelectedGameObject(*std::next(goIt));
						}

					}

				}

			}

			if (event->key.keysym.mod & KMOD_CTRL) {
				Scene* scene = Editor::getInstance()->getScene();
				auto selectedGo = scene->GetSelectedGameObject();

				if (event->key.keysym.scancode == SDL_SCANCODE_C) {
					if (selectedGo == nullptr) return;

					if (copiedObject != nullptr) {
						GameObject::unusedIds.push_back(copiedObject->GetId());
						delete copiedObject;
					}
		
					copiedObject = new GameObject(*selectedGo);

				}

				if (event->key.keysym.scancode == SDL_SCANCODE_V) {

					if (copiedObject == nullptr) return;

					if (copiedObject->IsTransform()) {
						scene->AddGameObject(copiedObject);
					}
					else {
						scene->AddOverlay(copiedObject);
					}

					copiedObject = new GameObject(*copiedObject);
				}

				if (event->key.keysym.scancode == SDL_SCANCODE_D) {
					if (selectedGo == nullptr) return;

					GameObject* goToDuplicate = new GameObject(*selectedGo);

					if (goToDuplicate->IsTransform()) {
						scene->AddGameObject(goToDuplicate);
					}
					else {
						scene->AddOverlay(goToDuplicate);
					}
				}

				if (event->key.keysym.scancode == SDL_SCANCODE_X) {
					if (selectedGo == nullptr) return;

					if (copiedObject != nullptr) {
						GameObject::unusedIds.push_back(copiedObject->GetId());
						delete copiedObject;
					}

					copiedObject = new GameObject(*selectedGo);

					if (selectedGo->IsTransform()) {
						scene->getGameObjects()[selectedGo->GetId()]->ToDelete();
					}
					else {
						scene->getOverlays()[selectedGo->GetId()]->ToDelete();
					}
				}

			}

		}

	}

	void Hierarchy::ReceiveAssetDrop(Asset& asset)
	{
		std::string extension = asset.extension;

		if (extension == ".png" || extension == ".jpg") {

			auto scene = Editor::getInstance()->getScene();

			GameObject* go = scene->AddGameObject(asset.relativePath);
			go->SetName(asset.name);

			scene->SetSelectedGameObject(go);
		}
	}

	void Hierarchy::handleDragAndDrop(GameObject* source, GameObject* destination)
	{
		if (destination == source->GetParent()) {
			destination->RemoveChild(source);

			source->SetParent(destination->GetParent());

			if (source->GetParent() != nullptr) {
				source->GetParent()->AddChild(source);
			}
		}
		else if (source->GetId() != destination->GetId() && !source->IsAscendant(destination) && destination != source->GetParent())
		{
			// Remove source from its current parent
			GameObject* parent = source->GetParent();
			if (parent)
			{
				parent->RemoveChild(source);
			}

			// Set the destination as the new parent
			destination->AddChild(source);
		}
	}


	bool Hierarchy::isChildrenTheSelectedObject(GameObject* go)
	{
		Scene* scene = Editor::getInstance()->getScene();

		for (auto child : go->GetChildren()) {
			if (child.second == scene->GetSelectedGameObject()) {
				return true;
			}

			return isChildrenTheSelectedObject(child.second);
		}

		return false;
	}

	bool Hierarchy::isParentFromPrefab(GameObject* go)
	{
		if (go->GetParent() != nullptr) {
			if (go->GetParent()->GetPrefabId() != 0) return true;
			
			return isParentFromPrefab(go->GetParent());
		}
		return false;
	}


	void Hierarchy::RenderGameObject(GameObject* gameObject, const char* type)
	{
		Scene* scene = Editor::getInstance()->getScene();
		const float iconSize = ImGui::GetTextLineHeight() + 8;

		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (gameObject->GetChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (scene->GetSelectedGameObject() == gameObject)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(gameObject);
		bool isOpen = ImGui::TreeNodeEx("##Root", flags);


		if (ImGui::IsItemClicked()) {

			scene->SetSelectedGameObject(gameObject);
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("Gameobject Menu##" + gameObject->GetId());
		}


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {

			ImGui::SetDragDropPayload(type, &gameObject, sizeof(GameObject*));

			ImGui::Image(gameObject->GetTexture()->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
			ImGui::SameLine();
			ImGui::Text(gameObject->GetName().c_str());

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

		ImGui::SameLine();

		if (gameObject->GetPrefabId() != 0 || isParentFromPrefab(gameObject)) {
			ImGui::TextColored(ImVec4(0.831f, 0.168f, 0.604f, 1.0f) , gameObject->GetName().c_str());
		}
		else {
			ImGui::Text(gameObject->GetName().c_str());
		}

		showGameObjectMenu(gameObject);
		showRenamePopup(gameObject);
		showSavePrefabPopup(gameObject);

		if (isOpen) {


			for (auto& child : gameObject->GetChildren()) {


				RenderGameObject(child.second, type);
			}


			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void Hierarchy::showRenamePopup(GameObject* gameObject)
	{
		if (shouldOpenRenamePopup) {
			ImGui::OpenPopup("Rename Object##" + gameObject->GetId());
			shouldOpenRenamePopup = false;
		}

		if (ImGui::BeginPopup("Rename Object##" + gameObject->GetId()))
		{
			ImGui::Text(("Insert new name for GameObject: " + gameObject->GetName()).c_str());

			ImGui::Separator();

			static char nameBuffer[256];  // Buffer to hold the new name

			// Display an input text field for renaming
			if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
			{

			}

			if (ImGui::Button("Ok"))
			{
				if (strlen(nameBuffer) > 0) {
					gameObject->SetName(nameBuffer);
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Hierarchy::showGameObjectMenu(GameObject* gameObject)
	{
		if (ImGui::BeginPopup("Gameobject Menu##" + gameObject->GetId()))
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
				gameObject->ToDelete();
			}

			ImGui::EndMenu();

		}
	}

	void Hierarchy::showSavePrefabPopup(GameObject* go)
	{
		if (shouldOpenSavePrefabPopup)
		{
			ImGui::OpenPopup("Save prefab##" + go->GetId());
			shouldOpenSavePrefabPopup = false;
		}

		if (ImGui::BeginPopup("Save prefab##" + go->GetId()))
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
					GameObject* prefab = new GameObject(*go);
					prefab->SetName(nameBuffer);

					PrefabManager::AddPrefab(prefab);
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
