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
#include <fstream>
using nlohmann::json;
using nlohmann::ordered_json;

#define PrefabImage "prefab.png"

namespace ShyEditor {

	PrefabManager* PrefabManager::instance = nullptr;
	int PrefabManager::lastPrefabId = -1;
	std::vector<int> PrefabManager::unusedIds = std::vector<int>();

	void PrefabManager::LoadPrefabs()
	{
		std::ifstream inputFile(editor->getProjectInfo().path + "\\prefabs.json");
		if (inputFile.is_open()) {
			json root;

			inputFile >> root;

			json prefabArray = root["prefabs"];

			for (const json& prefabData : prefabArray) {
				//TODO: wait for overlay serialisation to change boolean
				GameObject* prefab = GameObject::fromJson(prefabData.dump(), true);

				AddPrefab(prefab);
			}

			inputFile.close();
		}
		else {
			// Handle error opening the file
			std::cerr << "Error opening file: " << editor->getProjectInfo().path + "\\prefabs.json" << std::endl;
		}
	}

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

		LoadPrefabs();
	}

	void PrefabManager::SavePrefabs(const std::string& path)
	{
		json root;

		json prefabArray = json::array();

		for (const auto& pair : instance->prefabs) {
			json prefabData;

			GameObject* prefab = pair.second;
	
			prefabArray.push_back(prefabArray.parse(prefab->toJson()));
		}

		// Add the array to the root object
		root["prefabs"] = prefabArray;

		std::ofstream outputFile(path + "\\prefabs.json");
		if (outputFile.is_open()) {
			outputFile << root.dump(4) << std::endl;
			outputFile.close();
		}
	}

	GameObject* PrefabManager::GetPrefabById(int id)
	{
		if (instance->prefabs[id] != nullptr) {
			return instance->prefabs[id];
		}

		return nullptr;
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

			}

			if (ImGui::BeginDragDropSource()) {

				Asset asset;
				asset.isPrefab = true;
				asset.prefabId = prefab->getId();

				ImGui::SetDragDropPayload("Asset", &asset, sizeof(asset));

				ImGui::Image(prefabText->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
				ImGui::SameLine();
				ImGui::Text(prefab->getName().c_str());


				ImGui::EndDragDropSource();
			}

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

	void PrefabManager::Open()
	{
		instance->visible = true;
	}

	PrefabManager::~PrefabManager()
	{
		PrefabManager::SavePrefabs(editor->getProjectInfo().path);

		for (auto it = prefabs.begin(); it !=prefabs.end(); it++)
			delete it->second;

		prefabs.clear();
	}

	void PrefabManager::AddPrefab(GameObject* prefab)
	{
		if (prefab->getId() > 0) {
			GameObject::unusedIds.push_back(prefab->getId());
		}

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