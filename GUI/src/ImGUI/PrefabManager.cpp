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
#include "Scene.h"

using nlohmann::json;
using nlohmann::ordered_json;

#define PrefabImage "prefab.png"

namespace ShyEditor {

	PrefabManager* PrefabManager::instance = nullptr;
	int PrefabManager::lastPrefabId = -1;
	std::vector<int> PrefabManager::unusedIds = std::vector<int>();

	void PrefabManager::LoadPrefabs()
	{
		std::ifstream inputFile(editor->getProjectInfo().path + "\\Assets\\prefabs.json");
		if (inputFile.is_open()) {
			json root;

			inputFile >> root;

			json prefabArray = root["prefabs"];

			for (const json& prefabData : prefabArray) {
				//TODO: wait for overlay serialisation to change boolean
				GameObject* prefab = GameObject::fromJson(prefabData.dump(), true);

				AddPrefab(prefab);
			}

			std::unordered_map<int, GameObject*> sceneGameObjects = editor->getScene()->getGameObjects();
			json prefabInstancesArray = root["prefabInstances"];

			for (const auto& item : prefabInstancesArray.items()) {
				int prefabId = std::stoi(item.key());
					
				std::vector<int> prefabInstances;
				for (const auto& instanceId : item.value()) {
					if (sceneGameObjects[instanceId] != nullptr) {
						prefabInstances.push_back(instanceId);
					}
				}
				this->prefabInstances.emplace(prefabId, prefabInstances);
			}

			inputFile.close();
		}
		else {
			// Handle error opening the file
			std::cerr << "Error opening file: " << editor->getProjectInfo().path + "\\prefabs.json" << std::endl;
		}
	}

	void PrefabManager::HandleDeletion()
	{
		auto it = prefabs.begin();
		while (it != prefabs.end()) {
			GameObject* prefab = it->second;

			prefab->update();

			if (prefab->isWaitingToDelete()) {
				currentlySelected = 0;

				PrefabManager::unusedIds.push_back(prefab->getId());

				//Stop the instances being referenced
				for (auto instance : prefabInstances.find(prefab->getId())->second) {
					editor->getScene()->getGameObjects()[instance]->setPrefabId(0);
				}

				prefabInstances.erase(prefab->getId());

				delete prefab;
				it = prefabs.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void PrefabManager::UpdatePrefabInstances()
	{
		if (currentlySelected != 0) {
			GameObject* prefab = prefabs[currentlySelected];
			std::unordered_map<int, GameObject*> sceneGameObjects = editor->getScene()->getGameObjects();
		
			if (prefabInstances.find(prefab->getId()) != prefabInstances.end()) {
				std::vector<int> instances = prefabInstances.find(prefab->getId())->second;

				for (auto instanceId : instances) {
					GameObject* instance = sceneGameObjects.find(instanceId)->second;

					instance->setComponents(prefab->getComponentsCopy());
				}
			}
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
		windowHeight = 1080;

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

		json prefabInstancesArray;

		for (auto pair : instance->prefabInstances) {
			json prefabInstances;
			for (int gameObjectId : pair.second) {
				prefabInstances.push_back(gameObjectId);
			}
			prefabInstancesArray[std::to_string(pair.first)] = prefabInstances;
		}

		// Add the array to the root object
		root["prefabs"] = prefabArray;

		root["prefabInstances"] = prefabInstancesArray;

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

	void PrefabManager::DrawImage()
	{
		if (currentlySelected != 0) {
			GameObject* prefab = prefabs[currentlySelected];
			if (prefab->getTexture() != nullptr) {
				Texture* text = prefabs[currentlySelected]->getTexture();

				float sizeX = text->getWidth() * prefab->getScale_x();
				float sizeY = text->getHeight() * prefab->getScale_y();
				float textAspectRatio = sizeX / sizeY;

				//Fixed size so big gameObjects doesnt fill the whole window
				if (sizeX > sizeY) {
					sizeX = 150;
					sizeY = sizeX / textAspectRatio;
				}
				else {
					sizeY = 150;
					sizeX = sizeY * textAspectRatio;
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() / 2 - sizeX / 2);

				ImGui::Image(text->getSDLTexture(), ImVec2(sizeX, sizeY));
			}
		}
	}

	void PrefabManager::DrawComponents()
	{
		if (currentlySelected != 0) {

			GameObject* prefab = prefabs[currentlySelected];

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

			ImGui::PopStyleColor(6); // reset colors

			ImGui::Separator();


			if (ImGui::Button("Update prefab Instances", ImVec2(ImGui::GetColumnWidth(), 40))) {
				UpdatePrefabInstances();
			};


			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.2f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.2f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.2f, 0.2f, 1.0f)); // change header active color

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
		PrefabManager::SavePrefabs(editor->getProjectInfo().path + "\\Assets");

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

		prefab->setPrefabId(prefab->getId());
		instance->prefabs.emplace(prefab->getId(), prefab);
	}

	void PrefabManager::AddInstance(GameObject* prefab, GameObject* prefabInstance)
	{
		if (instance->prefabInstances.find(prefab->getId()) == instance->prefabInstances.end()) {
			std::vector<int> instances;
			instances.push_back(prefabInstance->getId());

			instance->prefabInstances.emplace(prefab->getId(), instances);
		}
		else {
			instance->prefabInstances[prefab->getId()].push_back(prefabInstance->getId());
		}
	}

	void PrefabManager::RemoveInstance(int prefabId, int prefabInstanceId)
	{
		if (instance->prefabInstances.find(prefabId) != instance->prefabInstances.end()) {
			std::vector<int>* instances = &instance->prefabInstances.find(prefabId)->second;
			
			auto it = std::find(instances->begin(), instances->end(), prefabInstanceId);

			// If the element is found, erase it
			if (it != instances->end()) {
				instances->erase(it);
			}
		}
	}

	void PrefabManager::Behaviour()
	{
		HandleDeletion();

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

			DrawImage();

			ImGui::SeparatorText("Components");

			DrawComponents();

			ImGui::EndTable();
		}
	}
}