#include "Components.h"
#include <imgui.h>
#include "Editor.h"
#include "Entity.h"
#include "Scene.h"
#include "ComponentInfo.h"
#include "Window.h"
#include "ComponentManager.h"
#include "FileExplorer.h"
#include "PrefabManager.h"

#include "CheckML.h"


namespace ShyEditor {
	ComponentWindow::ComponentWindow() : Window("Components", ImGuiWindowFlags_NoCollapse)
	{
		docked = true;
	}

	void ComponentWindow::Behaviour()
	{
		Entity* entity = Editor::GetInstance()->GetScene()->GetSelectedEntity();


		if (entity != nullptr) {

			if (entity->IsTransform())
				entity->DrawTransformInEditor();
			else
				entity->DrawOverlayInEditor();

			entity->DrawComponentsInEditor();
			entity->DrawScriptsInEditor();

			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.45f, 0.2f, 1.0f)); // change header color
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.65f, 0.2f, 1.0f)); // change header hover color
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.75f, 0.2f, 1.0f)); // change header active color



			bool isOverlay = !entity->IsTransform();

			if (ImGui::CollapsingHeader("Add component"))
			{
				for (auto& comp : ::Components::ComponentManager::GetAllComponents()) {

					const std::string overlayStr = "Overlay";
					const std::string transformStr = "Transform";

					const std::string compName = comp.first;

					if (compName == transformStr || compName == overlayStr)
						continue;

					bool constainsOverlay = compName.compare(0, overlayStr.length(), overlayStr) == 0;

					if (isOverlay != constainsOverlay)
						continue;

					if (!entity->GetComponents()->contains(compName)) {


						std::string modifiedName = Editor::Beautify(constainsOverlay ? compName.substr(overlayStr.length()) : compName);


						if (ImGui::Button(modifiedName.c_str(), ImVec2(windowWidth, 40))) {

							entity->AddComponent(comp.second);
						};




						std::string componentDescription;
						if (Components::ComponentManager::GetComponentInfo(componentDescription, compName))
						{
							ImGui::SetItemTooltip(componentDescription.c_str());
						}


					}
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

					if (!entity->GetScripts()->contains(script.GetName()))
						if (ImGui::Button(Editor::Beautify(script.GetName()).c_str(), ImVec2(windowWidth, 40))) {
							entity->AddScript(script);
						};
				}
			}

			ImGui::PopStyleColor(6); // reset colors


			// If its a prefab instance we add a button to unlink the entity from the prefab
			if (entity->IsPrefabInstance()) {

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::SeparatorText(("Prefab: " + PrefabManager::GetPrefabById(entity->GetPrefabId())->GetTopParentPrefab()->GetName()).c_str());

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				if (entity->GetParent() == nullptr || !entity->GetParent()->IsPrefabInstance()) {
					if (ImGui::Button("Unlink prefab", ImVec2(windowWidth, 40))) {
						PrefabManager::RemoveInstance(entity);
					}
				}

			}
		}


	}
}