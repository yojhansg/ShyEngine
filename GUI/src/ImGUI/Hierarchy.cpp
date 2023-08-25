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
			if (pair.second->getParent() == nullptr) {


				RenderGameObject(pair.second, "Transform");
			}
		}

		ImGui::SeparatorText("UI");

		for (auto& overlay : scene->getOverlays()) {

			if (overlay->getParent() == nullptr) {

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
					auto goIt = objects.find(selectedGo->getId());

					if (dir < 0) {

						if (goIt == objects.begin()) {
							if (overlays.rbegin() != overlays.rend()) {
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
							if (overlays.rbegin() != overlays.rend()) {
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
							if (objects.rbegin() != objects.rend()) {
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
							if (objects.rbegin() != objects.rend()) {
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
				if (event->key.keysym.scancode == SDL_SCANCODE_C) {
					Scene* scene = Editor::getInstance()->getScene();
					auto selectedGo = scene->GetSelectedGameObject();

					if (selectedGo == nullptr) return;

					if (copiedObject != nullptr) {
						GameObject::unusedIds.push_back(copiedObject->getId());
						delete copiedObject;
					}
		
					copiedObject = new GameObject(*selectedGo);

				}

				if (event->key.keysym.scancode == SDL_SCANCODE_V) {
					Scene* scene = Editor::getInstance()->getScene();

					if (copiedObject == nullptr) return;

					if (copiedObject->IsTransform()) {
						scene->AddGameObject(copiedObject);
					}
					else {
						scene->AddOverlay(copiedObject);
					}

					copiedObject = new GameObject(*copiedObject);
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
			go->setName(asset.name);

			scene->SetSelectedGameObject(go);
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


		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (gameObject->getChildren().size() == 0)
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
			ImGui::OpenPopup("Gameobject Menu##" + gameObject->getId());
		}


		ImGui::SameLine();

		if (gameObject->getPrefabId() != 0) {
			ImGui::TextColored(ImVec4(0.831f, 0.168f, 0.604f, 1.0f) , gameObject->getName().c_str());
		}
		else {
			ImGui::Text(gameObject->getName().c_str());
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
					GameObject* prefab = new GameObject(*go);
					prefab->setName(nameBuffer);

					PrefabManager::AddPrefab(prefab);

					go->setPrefabId(prefab->getId());
					PrefabManager::AddInstance(prefab, go);
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
