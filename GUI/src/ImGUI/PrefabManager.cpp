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
				GameObject* prefab = GameObject::fromJson(prefabData.dump());

				AddPrefab(prefab);
			}

			std::map<int, GameObject*> sceneGameObjects = editor->getScene()->getGameObjects();

			json prefabInstancesArray = root["prefabInstances"];

			for (const auto& item : prefabInstancesArray.items()) {
				int prefabId = std::stoi(item.key());

				std::vector<int> prefabInstances;
				for (const auto& instanceId : item.value()) {
					if (sceneGameObjects[instanceId] != nullptr || IdIsInOverlays(instanceId) != nullptr) {
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
				if (prefabInstances.find(prefab->getId()) != prefabInstances.end()) {
					for (auto instanceId : prefabInstances.find(prefab->getId())->second) {
						if (prefab->IsTransform()) {
							editor->getScene()->getGameObjects()[instanceId]->setPrefabId(0);
						}
						else {
							IdIsInOverlays(instanceId)->setPrefabId(0);
						}
					}

					prefabInstances.erase(prefab->getId());
				}

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
			std::map<int, GameObject*> sceneGameObjects = editor->getScene()->getGameObjects();

			if (prefabInstances.find(prefab->getId()) != prefabInstances.end()) {
				std::vector<int> instances = prefabInstances.find(prefab->getId())->second;

				for (auto instanceId : instances) {
					GameObject* instance = nullptr;

					if (sceneGameObjects.find(instanceId) != sceneGameObjects.end()) {
						instance = sceneGameObjects.find(instanceId)->second;
					}
					else if (IdIsInOverlays(instanceId) != nullptr) {
						instance = IdIsInOverlays(instanceId);
					}

					instance->setComponents(prefab->getComponentsCopy());
					instance->setScripts(prefab->getScriptsCopy());
				}
			}
		}
	}

	GameObject* PrefabManager::IdIsInOverlays(int id)
	{
		std::vector<GameObject*> sceneOverlays = editor->getScene()->getOverlays();

		for (GameObject* overlay : sceneOverlays) {
			if (overlay->getId() == id) {
				return overlay;
			}
		}
		return nullptr;
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

		windowWidth = 700;
		windowHeight = 700;

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

			if (prefab->getParent() == nullptr)
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
		if (instance->prefabs.find(id) != instance->prefabs.end()) {
			return instance->prefabs[id];
		}

		return nullptr;
	}

	void PrefabManager::DrawList()
	{

		// TODO: cambiar el color para las carpetas
		// ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));

		for (auto& pair : prefabs) {
			GameObject* prefab = pair.second;

			if (prefab->getParent() == nullptr)
				DrawPrefab(prefab);
		}

		ImGui::Unindent();
	}

	void PrefabManager::DrawPrefab(GameObject* prefab)
	{
		const float iconSize = ImGui::GetTextLineHeight() + 8;

		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (prefab->getChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (currentlySelected != 0 && prefabs[currentlySelected] == prefab)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(prefab);
		bool isOpen = ImGui::TreeNodeEx("##Root", flags);

		ImGui::SameLine();

		ImGui::SetNextItemAllowOverlap();
		if (ImGui::Selectable(std::string("##" + prefab->getName()).c_str(), currentlySelected == prefab->getId())) {
			currentlySelected = prefab->getId();
		}

		if(prefab->getParent() == nullptr){
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
		}
		
		ImGui::SameLine();

		int yCursor = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(yCursor - 5);
		ImGui::Image(prefabText->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
		ImGui::SetCursorPosY(yCursor);

		ImGui::SameLine();
		ImGui::Text(prefab->getName().c_str());

		if (isOpen) {

			for (auto& child : prefab->getChildren()) {
				DrawPrefab(child.second);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void PrefabManager::DrawImage()
	{
		if (currentlySelected != 0) {
			GameObject* prefab = prefabs[currentlySelected];
			if ((prefab->IsTransform() && prefab->getTexture() != nullptr) ||
				(!prefab->IsTransform() && prefab->GetOverlay()->GetImage()->GetTexture() != nullptr))
			{
				Texture* text = prefab->IsTransform() ? prefab->getTexture() : prefab->GetOverlay()->GetImage()->GetTexture();

				float scaleX = prefab->IsTransform() ? prefab->getScale_x() : prefab->GetOverlay()->GetSize().x;
				float scaleY = prefab->IsTransform() ? prefab->getScale_y() : prefab->GetOverlay()->GetSize().y;

				float sizeX = text->getWidth() * scaleX;
				float sizeY = text->getHeight() * scaleY;
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

			if (prefab->drawComponentsInEditor()) {
				UpdatePrefabInstances();
			}

			if (prefab->drawScriptsInEditor()) {
				UpdatePrefabInstances();
			}

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
							UpdatePrefabInstances();
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
							UpdatePrefabInstances();
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
		instance->Show();
	}

	PrefabManager::~PrefabManager()
	{
		PrefabManager::SavePrefabs(editor->getProjectInfo().path + "\\Assets");

		for (auto it = prefabs.begin(); it != prefabs.end(); it++)
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

		for (auto child : prefab->getChildren()) {
			child.second->setPrefabId(prefab->getId());
			AddPrefab(child.second);
		}
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

	void PrefabManager::AddInstance(int prefabId, int prefabInstanceId)
	{
		if (instance->prefabInstances.find(prefabId) == instance->prefabInstances.end()) {
			std::vector<int> instances;
			instances.push_back(prefabInstanceId);

			instance->prefabInstances.emplace(prefabId, instances);
		}
		else {
			instance->prefabInstances[prefabId].push_back(prefabInstanceId);
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

		for (auto child : instance->prefabs[prefabId]->getChildren())
		{
			RemoveInstance(child.second->getId(), prefabInstanceId);
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