#include "PrefabManager.h"

#include "ComponentManager.h";
#include "nlohmann/json.hpp"
#include "ResourcesManager.h"
#include "ProjectsManager.h"
#include "LogManager.h"
#include "Texture.h"
#include "Entity.h"
#include "Editor.h"
#include "imgui.h"
#include "Scene.h"

#include <string>
#include <fstream>

using nlohmann::json;
using nlohmann::ordered_json;

#define PrefabImage "prefab.png"

namespace ShyEditor {

	PrefabManager* PrefabManager::instance = nullptr;
	int PrefabManager::lastPrefabId = -1;
	std::vector<int> PrefabManager::unusedIds = std::vector<int>();

	PrefabManager::PrefabManager() : Window("Prefab manager", 0)
	{
		instance = this;

		editor = Editor::GetInstance();
		prefabText = ResourcesManager::GetInstance()->AddTexture(PrefabImage, true);

		open = false;	
		canBeDisplayedOnTop = true;

		currentlySelected = 0;

		windowWidth = 700;
		windowHeight = 700;

		Hide();
	}

	PrefabManager::~PrefabManager()
	{
		for (auto it = prefabs.begin(); it != prefabs.end(); it++)
			delete it->second;

		prefabs.clear();
	}
	
	void PrefabManager::Behaviour()
	{
		HandlePrefabDeletion();

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoHostExtendX;

		if (ImGui::BeginTable("Prefabs", 2, flags))
		{
			ImGui::TableSetupColumn("Prefab list", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Edit", ImGuiTableColumnFlags_WidthFixed);

			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			DrawPrefabList();

			ImGui::TableSetColumnIndex(1);


			ImGui::SeparatorText("Image");
			DrawImage();

			ImGui::SeparatorText("Components");
			DrawComponents();

			ImGui::EndTable();
		}
	}

	void PrefabManager::Open()
	{
		instance->Show();
	}

	void PrefabManager::SavePrefabs()
	{
		ordered_json root; // Prefabs JSON
		
		// Save prefabs
		ordered_json prefabArray = ordered_json::array();

		for (const auto& pair : instance->prefabs) {
			Entity* prefab = pair.second;

			if (prefab->GetParent() == nullptr)
				prefabArray.push_back(prefabArray.parse(prefab->ToJson()));
		}


		// Save prefabs instances references
		ordered_json prefabInstancesArray;

		for (auto pair : instance->prefabInstances) {

			ordered_json prefabInstances;

			for (int entityId : pair.second)
				prefabInstances.push_back(entityId);

			prefabInstancesArray[std::to_string(pair.first)] = prefabInstances;
		}

		root["prefabs"] = prefabArray;
		root["prefabInstances"] = prefabInstancesArray;


		// Obtain project file JSON
		std::ifstream projectFile(ProjectsManager::GetProjectFilePath());

		if (!projectFile.good() || !json::accept(projectFile)) {
			LogManager::LogError("Could not open project file to store the prefabs data.");
			return;
		}

		projectFile.clear();
		projectFile.seekg(0);

		ordered_json project = ordered_json::parse(projectFile);


		// Add the prefabs JSON as a new attribute of the project file JSON
		project["Prefabs"] = root;

		std::ofstream output(ProjectsManager::GetProjectFilePath());
		output << project.dump(4);
		output.close();

	}

	void PrefabManager::LoadPrefabs() {

		std::ifstream projectFile(ProjectsManager::GetProjectFilePath());

		if (!projectFile.good() || !json::accept(projectFile)) {
			LogManager::LogError("Could not open project file to load the prefabs data.");
			return;
		}

		projectFile.clear();
		projectFile.seekg(0);

		ordered_json project = ordered_json::parse(projectFile);

		projectFile.close();

		if (!project.contains("Prefabs")) return;

		ordered_json root = project["Prefabs"];

		ordered_json prefabArray = root["prefabs"];

		// Read the prefabs
		for (ordered_json& prefabData : prefabArray) {
			Entity* prefab = Entity::FromJson(prefabData);

			AddPrefab(prefab);
		}

		// Read the prefabs instances
		std::map<int, Entity*> sceneEntities = instance->editor->GetScene()->GetEntities();
		json prefabInstancesArray = root["prefabInstances"];

		for (const auto& item : prefabInstancesArray.items()) {
			int prefabId = std::stoi(item.key());

			std::vector<int> prefabInstances;
			for (const auto& instanceId : item.value()) {

				//We check if its reference is still in the scene cause it could have been deleted
				if (sceneEntities[instanceId] != nullptr || instance->IdIsInOverlays(instanceId) != nullptr) {
					prefabInstances.push_back(instanceId);
				}
			}

			instance->prefabInstances.emplace(prefabId, prefabInstances);
		}

	}

	void PrefabManager::AssignId(Entity* prefab)
	{
		if (prefab->GetId() > 0) {
			Entity::unusedIds.push_back(prefab->GetId());
		}

		if (PrefabManager::unusedIds.size() != 0) {
			prefab->SetId(PrefabManager::unusedIds.back());
			PrefabManager::unusedIds.pop_back();
		}
		else {
			//Ensure id is not being used already
			while (instance->prefabs.find(PrefabManager::lastPrefabId) != instance->prefabs.end()) {
				PrefabManager::lastPrefabId--;
			}

			prefab->SetId(PrefabManager::lastPrefabId);
			PrefabManager::lastPrefabId--;
		}
	}

	void PrefabManager::AddPrefab(Entity* prefab)
	{
		AssignId(prefab);

		instance->prefabs.emplace(prefab->GetId(), prefab);

		RemoveScriptReferences(prefab);

		for (auto child : prefab->GetChildren()) {
			AddPrefab(child.second);
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

	void PrefabManager::RemoveInstance(Entity* instanceEntity)
	{
		PrefabManager::RemoveInstance(instanceEntity->GetPrefabId(), instanceEntity->GetId());
		instanceEntity->SetPrefabId(0);

		for (auto child : instanceEntity->GetChildren()) {
			RemoveInstance(child.second);
		}
	}

	void PrefabManager::RemoveInstance(int prefabId, int prefabInstanceId)
	{
		//Remove the element from the instances vector of that prefab
		auto it = instance->prefabInstances.find(prefabId);
		if (it != instance->prefabInstances.end()) {

			std::vector<int>* instances = &it->second;

			auto it2 = std::find(instances->begin(), instances->end(), prefabInstanceId);

			if (it2 != instances->end()) {
				instances->erase(it2);
			}
		}
	}

	std::unordered_map<int, Entity*>& PrefabManager::GetPrefabs()
	{
		return instance->prefabs;
	}

	Entity* PrefabManager::GetPrefabById(int id)
	{
		auto it = instance->prefabs.find(id);
		if (it != instance->prefabs.end()) {
			return it->second;
		}

		return nullptr;
	}

	void PrefabManager::DrawPrefabList()
	{

		for (auto& pair : prefabs) {
			Entity* prefab = pair.second;

			if (prefab->GetParent() == nullptr)
				DrawPrefab(prefab);

		}

	}

	void PrefabManager::DrawPrefab(Entity* prefab)
	{
		const float iconSize = ImGui::GetTextLineHeight() + 8;

		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (prefab->GetChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (currentlySelected != 0 && prefabs[currentlySelected] == prefab)
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(prefab);
		bool isOpen = ImGui::TreeNodeEx("##Root", flags);

		ImGui::SameLine();
		ImGui::SetNextItemAllowOverlap();


		if (ImGui::Selectable(std::string("##" + prefab->GetName()).c_str(), currentlySelected == prefab->GetId())) {
			currentlySelected = prefab->GetId();
		}

		if(prefab->GetParent() == nullptr){
			if (ImGui::BeginDragDropSource()) {

				Asset asset;
				asset.isPrefab = true;
				asset.prefabId = prefab->GetId();

				ImGui::SetDragDropPayload("Asset", &asset, sizeof(asset));

				ImGui::Image(prefabText->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
				ImGui::SameLine();
				ImGui::Text(prefab->GetName().c_str());


				ImGui::EndDragDropSource();
			}
		}
		
		ImGui::SameLine();

		ImGui::Image(prefabText->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));

		ImGui::SameLine();

		ImGui::Text(prefab->GetName().c_str());

		if (isOpen) {

			for (auto& child : prefab->GetChildren()) {
				DrawPrefab(child.second);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void PrefabManager::DrawImage()
	{
		if (currentlySelected != 0) {

			Entity* prefab = prefabs[currentlySelected];

			bool isTransformWithTexture = prefab->IsTransform() && prefab->GetTexture() != nullptr;
			bool isOverlayWithTexture = !prefab->IsTransform() && prefab->GetOverlay()->GetImage()->GetTexture() != nullptr;

			if (isTransformWithTexture || isOverlayWithTexture)
			{
				Texture* text = prefab->IsTransform() ? prefab->GetTexture() : prefab->GetOverlay()->GetImage()->GetTexture();

				float scaleX = prefab->IsTransform() ? prefab->GetScaleX() : prefab->GetOverlay()->GetSize().x;
				float scaleY = prefab->IsTransform() ? prefab->GetScaleY() : prefab->GetOverlay()->GetSize().y;

				float sizeX = text->getWidth() * scaleX;
				float sizeY = text->getHeight() * scaleY;
				float textAspectRatio = sizeX / sizeY;

				//Fixed size so big entities doesnt fill the whole window
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

			Entity* prefab = prefabs[currentlySelected];

			// If a change is made we update the prefab instances
			if (prefab->DrawComponentsInEditor()) {
				UpdatePrefabInstances();
			}

			if (prefab->DrawScriptsInEditor()) {
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

					if (!prefab->GetComponents()->contains(compName))
						if (ImGui::Button(compName.c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {

							prefab->AddComponent(comp.second);

							// If a component is added we update the prefab instances
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

					if (!prefab->GetScripts()->contains(script.GetName()))
						if (ImGui::Button(script.GetName().c_str(), ImVec2(ImGui::GetColumnWidth(), 40))) {
							
							prefab->AddScript(script);


							// If a script is added we update the prefab instances
							UpdatePrefabInstances();
						};
				}
			}

			ImGui::Separator();

			if (prefab->GetParent() == nullptr) {
				if (ImGui::Button("Delete prefab", ImVec2(ImGui::GetColumnWidth(), 40))) {
					prefab->ToDelete();
				};
			}
			ImGui::PopStyleColor(6); // reset colors
		}
	}

	void PrefabManager::HandlePrefabDeletion()
	{
		auto it = prefabs.begin();

		while (it != prefabs.end()) {
			Entity* prefab = it->second;

			prefab->Update();

			if (prefab->IsWaitingToDelete()) {
				currentlySelected = 0;

				// Add the id to unusedIds to reutilize it later
				PrefabManager::unusedIds.push_back(prefab->GetId());

				//Stop the instances being referenced
				auto prefabIt = prefabInstances.find(prefab->GetId());
				if (prefabIt != prefabInstances.end()) {
					for (auto instanceId : prefabIt->second) {
						if (prefab->IsTransform()) {
							editor->GetScene()->GetEntities()[instanceId]->SetPrefabId(0);
						}
						else {
							IdIsInOverlays(instanceId)->SetPrefabId(0);
						}
					}

					prefabInstances.erase(prefab->GetId());
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
			Entity* prefab = prefabs[currentlySelected];

			std::map<int, Entity*> sceneEntities = editor->GetScene()->GetEntities();

			auto prefabIt = prefabInstances.find(prefab->GetId());
			if (prefabIt != prefabInstances.end()) {
				std::vector<int> instances = prefabIt->second;

				for (auto instanceId : instances) {
					Entity* instance = nullptr;

					if (sceneEntities.find(instanceId) != sceneEntities.end()) {
						instance = sceneEntities.find(instanceId)->second;
					}
					else if (IdIsInOverlays(instanceId) != nullptr) {
						instance = IdIsInOverlays(instanceId);
					}

					instance->SetComponents(prefab->GetComponentsCopy());
					instance->SetScripts(prefab->GetScriptsCopy());
				}
			}
		}
	}

	Entity* PrefabManager::IdIsInOverlays(int id)
	{
		std::vector<Entity*> sceneOverlays = editor->GetScene()->GetOverlays();

		for (Entity* overlay : sceneOverlays) {
			if (overlay->GetId() == id) {
				return overlay;
			}
		}
		return nullptr;
	}
	void PrefabManager::RemoveScriptReferences(Entity* prefab)
	{
		for (auto& script : *prefab->GetScripts()) {
			for (auto& attr : script.second.GetAllAttributes()) {
				if (attr.second.GetType() == Components::AttributesType::ENTITY) {
					int idx = attr.second.value.value.entityIdx;

					if (idx != 0) {
						attr.second.value.value.entityIdx = 0;
					}
				}
			}
		}
	}
}