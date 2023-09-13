#include "Hierarchy.h"

#include "nlohmann/json.hpp"
#include "FileExplorer.h"
#include "Entity.h"
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

		docked = true;

		acceptAssetDrop = true;

		copiedEntity = nullptr;
		cutEntity = nullptr;

	}

	Hierarchy::~Hierarchy()
	{
		if (copiedEntity != nullptr) {
			delete copiedEntity;
		}

		if (cutEntity != nullptr) {
			delete cutEntity;
		}

		Window::~Window();
	}

	void Hierarchy::Behaviour()
	{
		Editor* editor = Editor::GetInstance();

		ImVec2 mainWindowSize = editor->GetMainWindowSize();

		Scene* scene = editor->GetScene();
		FileExplorer* fileExplorer = editor->GetFileExplorer();


		if (ImGui::Button("Create world entity")) {
			if (editor->IsAnySceneOpened())
				scene->SetSelectedEntity(scene->AddEntity("Empty entity"));
		}

		ImGui::SameLine();
		if (ImGui::Button("Create UI element")) {
			if (editor->IsAnySceneOpened())
				scene->SetSelectedEntity(scene->AddOverlay("Empty UI element"));
		}


		ImGui::SeparatorText("World");

		int i = 0;
		for (auto& pair : scene->GetEntities())
		{
			if (pair.second->GetParent() == nullptr) {


				RenderEntity(pair.second, "Transform");
			}
		}

		ImGui::SeparatorText("UI");

		for (auto& overlay : scene->GetOverlays()) {

			if (overlay->GetParent() == nullptr) {

				RenderEntity(overlay, "Overlay");
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

				Scene* scene = Editor::GetInstance()->GetScene();

				auto& entities = scene->GetEntities();
				auto& overlays = scene->GetOverlays();

				if (entities.size() == 0 && overlays.size() == 0) return;


				auto selectedEntity = scene->GetSelectedEntity();

				if (selectedEntity == nullptr) return;

				if (selectedEntity->IsTransform()) {
					auto entityIt = entities.find(selectedEntity->GetId());

					if (dir < 0) {

						if (entityIt == entities.begin()) {
							if (overlays.size() > 0) {
								scene->SetSelectedEntity(*overlays.rbegin());
							}
							else {
								scene->SetSelectedEntity(entities.rbegin()->second);
							}
						}				
						else {
							scene->SetSelectedEntity(std::prev(entityIt)->second);
						}
					}

					else {

						if (std::next(entityIt) == entities.end()) {
							if (overlays.size() > 0) {
								scene->SetSelectedEntity(*overlays.begin());
							}
							else {
								scene->SetSelectedEntity(entities.begin()->second);
							}
						}
						else {
							scene->SetSelectedEntity(std::next(entityIt)->second);
						}
					}
				}
				else {

					auto entityIt = std::find(overlays.begin(), overlays.end(), selectedEntity);

					if (dir < 0) {

						if (entityIt == overlays.begin()) {
							if (entities.size() > 0) {
								scene->SetSelectedEntity(entities.rbegin()->second);
							}
							else {
								scene->SetSelectedEntity(*overlays.rbegin());
							}
						}
						else {
							scene->SetSelectedEntity(*std::prev(entityIt));

						}
					}

					else {

						if (std::next(entityIt) == overlays.end()) {
							if (entities.size() > 0) {
								scene->SetSelectedEntity(entities.begin()->second);
							}
							else {
								scene->SetSelectedEntity(*overlays.begin());
							}
						}
						else {
							scene->SetSelectedEntity(*std::next(entityIt));
						}

					}

				}

			}

			if (event->key.keysym.mod & KMOD_CTRL) {
				Scene* scene = Editor::GetInstance()->GetScene();
				auto selectedEntity = scene->GetSelectedEntity();

				if (event->key.keysym.scancode == SDL_SCANCODE_C) {
					if (selectedEntity == nullptr) return;

					if (copiedEntity != nullptr) {
						Entity::unusedIds.push_back(copiedEntity->GetId());
						delete copiedEntity;
					}
		
					copiedEntity = new Entity(*selectedEntity, false);
					cutEntity = nullptr;

				}

				if (event->key.keysym.scancode == SDL_SCANCODE_V) {
					Entity* entityToPaste;

					if (copiedEntity != nullptr) {
						entityToPaste = copiedEntity;
					}
					else if (cutEntity != nullptr) {
						entityToPaste = cutEntity;
					}
					else return;

					if (entityToPaste->IsTransform()) {
						scene->AddEntity(entityToPaste);
						scene->AddEntityChildsToScene(entityToPaste);
					}
					else {
						scene->AddOverlay(entityToPaste);
						scene->AddOverlayChildsToScene(entityToPaste);
					}

					if (copiedEntity != nullptr) {
						copiedEntity = new Entity(*copiedEntity, false);
					}
					
					cutEntity = nullptr;
				}

				if (event->key.keysym.scancode == SDL_SCANCODE_D) {
					if (selectedEntity == nullptr) return;

					Entity* entityToDuplicate = new Entity(*selectedEntity, false);

					if (entityToDuplicate->IsTransform()) {
						scene->AddEntity(entityToDuplicate);
						scene->AddEntityChildsToScene(entityToDuplicate);
					}
					else {
						scene->AddOverlay(entityToDuplicate);
						scene->AddOverlayChildsToScene(entityToDuplicate);
					}
				}

				if (event->key.keysym.scancode == SDL_SCANCODE_X) {
					if (selectedEntity == nullptr) return;

					if (cutEntity != nullptr) {
						Entity::unusedIds.push_back(cutEntity->GetId());
						delete cutEntity;
					}

					cutEntity = new Entity(*selectedEntity, false);
					copiedEntity = nullptr;

					if (selectedEntity->IsTransform()) {
						scene->GetEntities()[selectedEntity->GetId()]->ToDelete();
					}
					else {
						for (auto& overlay : scene->GetOverlays()) {
							
							if (overlay->GetId() == selectedEntity->GetId()) {
								overlay->ToDelete();
								break;
							}

							for (auto& child : overlay->GetChildren()) {
								if (child.second->GetId() == selectedEntity->GetId()) {
									child.second->ToDelete();
									break;
								}

								break;
							}
						}
					}
				}

			}

		}

	}

	void Hierarchy::ReceiveAssetDrop(Asset& asset)
	{
		std::string extension = asset.extension;

		if (extension == ".png" || extension == ".jpg") {

			auto scene = Editor::GetInstance()->GetScene();

			Entity* entity = scene->AddEntity(asset.relativePath);
			entity->SetName(asset.name);

			scene->SetSelectedEntity(entity);
		}
	}

	void Hierarchy::handleDragAndDrop(Entity* source, Entity* destination)
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
			Entity* parent = source->GetParent();
			if (parent)
			{
				parent->RemoveChild(source);
			}

			// Set the destination as the new parent
			destination->AddChild(source);
		}
	}


	bool Hierarchy::isChildrenTheSelectedEntity(Entity* entity)
	{
		Scene* scene = Editor::GetInstance()->GetScene();

		for (auto child : entity->GetChildren()) {
			if (child.second == scene->GetSelectedEntity()) {
				return true;
			}

			return isChildrenTheSelectedEntity(child.second);
		}

		return false;
	}

	bool Hierarchy::isParentFromPrefab(Entity* entity)
	{
		if (entity->GetParent() != nullptr) {
			if (entity->GetParent()->IsPrefabInstance() != 0) return true;
			
			return isParentFromPrefab(entity->GetParent());
		}
		return false;
	}


	void Hierarchy::RenderEntity(Entity* entity, const char* type)
	{
		Scene* scene = Editor::GetInstance()->GetScene();
		const float iconSize = ImGui::GetTextLineHeight() + 8;

		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (entity->GetChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (scene->GetSelectedEntity() == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(entity);
		bool isOpen = ImGui::TreeNodeEx("##Root", flags);


		if (ImGui::IsItemClicked()) {

			scene->SetSelectedEntity(entity);
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("Entity Menu##" + entity->GetId());
		}


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {

			ImGui::SetDragDropPayload(type, &entity, sizeof(Entity*));

			if (entity->GetTexture() != nullptr) {
				ImGui::Image(entity->GetTexture()->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
				ImGui::SameLine();
			}
			ImGui::Text(entity->GetName().c_str());

			ImGui::EndDragDropSource();
		}


		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				Entity* sourceEntity = *(Entity**)payload->Data;

				handleDragAndDrop(sourceEntity, entity);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		if (entity->IsPrefabInstance() || (entity->IsPrefabInstance() && isParentFromPrefab(entity))) {
			ImGui::TextColored(ImVec4(0.831f, 0.168f, 0.604f, 1.0f) , entity->GetName().c_str());
		}
		else {
			ImGui::Text(entity->GetName().c_str());
		}

		ShowEntityMenu(entity);
		ShowRenamePopup(entity);

		if (isOpen) {


			for (auto& child : entity->GetChildren()) {


				RenderEntity(child.second, type);
			}


			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void Hierarchy::ShowRenamePopup(Entity* entity)
	{
		if (shouldOpenRenamePopup) {
			ImGui::OpenPopup("Rename entity##" + entity->GetId());
			shouldOpenRenamePopup = false;
		}

		if (ImGui::BeginPopup("Rename entity##" + entity->GetId()))
		{
			ImGui::Text(("Insert new name for Entity: " + entity->GetName()).c_str());

			ImGui::Separator();

			static char nameBuffer[256];  // Buffer to hold the new name

			// Display an input text field for renaming
			if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
			{

			}

			if (ImGui::Button("Ok"))
			{
				if (strlen(nameBuffer) > 0) {
					entity->SetName(nameBuffer);
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Hierarchy::ShowEntityMenu(Entity* entity)
	{
		if (ImGui::BeginPopup("Entity Menu##" + entity->GetId()))
		{
			if (ImGui::MenuItem("Create prefab", NULL, false)) {
				Entity* prefab = new Entity(*entity);

				PrefabManager::AddPrefab(prefab);
			}

			if (ImGui::MenuItem("Add script", NULL, false)) {

				Editor::GetInstance()->OpenScript("");
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Rename", NULL, false))
			{
				shouldOpenRenamePopup = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Delete", NULL, false)) {
				entity->ToDelete();
			}

			ImGui::EndMenu();

		}
	}
}
