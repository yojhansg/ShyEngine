#include "PrefabManager.h"
#include "nlohmann/json.hpp"
#include "GameObject.h"
#include "Editor.h"
#include "ProjectsManager.h"
#include "ResourcesManager.h"
#include "imgui.h"
#include "Texture.h"
#include "ComponentManager.h";
#include <string>

using nlohmann::json;
using nlohmann::ordered_json;

#define PrefabImage "prefab.png"

namespace ShyEditor {

	PrefabManager* PrefabManager::instance = nullptr;
	int PrefabManager::lastPrefabId = -1;
	std::vector<int> PrefabManager::unusedIds = std::vector<int>();

	PrefabManager::PrefabManager() : Window("Prefab manager", 0)
	{
		editor = Editor::getInstance();

		prefabText = ResourcesManager::GetInstance()->AddTexture(PrefabImage, true);

		Hide();
		instance = this;

		open = false;
		shouldUpdate = false;

		currentlySelected = 0;

		windowWidth = 1080;
		windowHeight = 720;

		canBeDisplayedOnTop = true;

		projectPath = editor->getProjectInfo().path;
	}

	nlohmann::ordered_json PrefabManager::SavePrefabs()
	{
		nlohmann::ordered_json root = nlohmann::ordered_json();

		return root;
	}

	void PrefabManager::DrawList()
	{
		const float iconSize = ImGui::GetTextLineHeight() + 8;

		// TODO: cambiar el color para las carpetas
		// ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));

		for (auto& pair : prefabs) {
			GameObject* prefab = pair.second;

			ImGui::SetNextItemAllowOverlap();
			if (ImGui::Selectable(std::string("##" + prefab->getName()).c_str(), currentlySelected == prefab->getId(), ImGuiSelectableFlags_AllowDoubleClick)) {

				currentlySelected = prefab->getId();
				//OnItemSelected(entry);

			}
			else {

				/*if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					ItemDrag(entry);*/
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
				ImGui::OpenPopup("Prefab Menu##" + prefab->getId());
			}

			ShowPrefabMenuPopup(prefab);

			ImGui::SameLine();

			int yCursor = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(yCursor - 5);
			ImGui::Image(prefabText->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
			ImGui::SetCursorPosY(yCursor);

			ImGui::SameLine();
			ImGui::Text(prefab->getName().c_str());
		}

		ImGui::Unindent();
	}

	void PrefabManager::DrawIcons()
	{
	}

	void PrefabManager::DrawComponents()
	{
		if (currentlySelected != 0) {

			GameObject* prefab = prefabs[currentlySelected];

			if (prefab->IsTransform())
				prefab->drawTransformInEditor();
			else
				prefab->drawOverlayInEditor();

			prefab->drawComponentsInEditor();
			prefab->drawScriptsInEditor();

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

			bool isOverlay = !prefab->IsTransform();

			if (ImGui::CollapsingHeader("Add component"))
			{
				for (auto& comp : ::Components::ComponentManager::GetAllComponents()) {

					const std::string overlayStr = "Overlay";
					const std::string transformStr = "Transform";

					std::string compName = comp.first;

					if (compName == transformStr || compName == overlayStr)
						continue;

					bool constainsOverlay = compName.compare(0, overlayStr.length(), overlayStr) == 0;

					if (isOverlay != constainsOverlay)
						continue;

					if (!prefab->getComponents()->contains(compName))
						if (ImGui::Button(compName.c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {

							prefab->addComponent(comp.second);
						};
				}
			}

			ImGui::PopStyleColor(6); // reset colors

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

			if (ImGui::CollapsingHeader("Add scripts"))
			{

				for (auto& script : ::Components::ComponentManager::GetAllScripts()) {

					if (!prefab->getScripts()->contains(script.GetName()))
						if (ImGui::Button(script.GetName().c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {
							prefab->addScript(script);
						};
				}
			}

			ImGui::Separator();

			if (ImGui::Button("Delete prefab", ImVec2(ImGui::GetColumnWidth(), 40))) {
				prefab->toDelete();
			};

			ImGui::PopStyleColor(6); // reset colors
		}
	}

	void PrefabManager::ShowPrefabMenuPopup(GameObject* prefab)
	{
		if (ImGui::BeginPopup("Prefab Menu##" + prefab->getId()))
		{

			if (ImGui::Button("Delete prefab"))
			{
				prefab->toDelete();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void PrefabManager::Open()
	{
		instance->visible = true;
	}

	void PrefabManager::AddPrefab(GameObject* prefab)
	{
		if (PrefabManager::unusedIds.size() != 0) {
			prefab->setId(PrefabManager::unusedIds.back());
			PrefabManager::unusedIds.pop_back();
		}
		else {
			//Ensure id is not being used already
			while (instance->prefabs.find(PrefabManager::lastPrefabId) != instance->prefabs.end()) {
				PrefabManager::lastPrefabId--;
			}
			
			prefab->setId(PrefabManager::lastPrefabId);
			PrefabManager::lastPrefabId--;
		}

		instance->prefabs.emplace(prefab->getId(), prefab);
	}

	void PrefabManager::Behaviour()
	{
		auto it = prefabs.begin();
		while (it != prefabs.end()) {
			GameObject* prefab = it->second;

			prefab->update();

			if (prefab->isWaitingToDelete()) {
				currentlySelected = 0;

				PrefabManager::unusedIds.push_back(prefab->getId());

				delete prefab;
				it = prefabs.erase(it);
			}
			else {
				++it;
			}
		}

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoHostExtendX;

		if (ImGui::BeginTable("Prefabs", 2, flags))
		{
			ImGui::TableSetupColumn("Prefab list", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Edit", ImGuiTableColumnFlags_WidthFixed);

			ImGui::TableHeadersRow();

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);

			DrawList();  

			ImGui::TableSetColumnIndex(1);
			
			ImGui::SeparatorText("Image");

			if (currentlySelected != 0 && prefabs[currentlySelected]->getTexture() != nullptr) {
				Texture* text = prefabs[currentlySelected]->getTexture();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2 - text->getWidth() / 2);

				ImGui::Image(text->getSDLTexture(), ImVec2(text->getWidth(), text->getHeight()));
			}

			ImGui::SeparatorText("Components");

			DrawComponents();

			ImGui::EndTable();
		}
	}
}