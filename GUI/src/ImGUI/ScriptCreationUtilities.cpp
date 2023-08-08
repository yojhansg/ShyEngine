#include "ScriptCreationUtilities.h"

#include "imgui.h"

#include "ComponentManager.h"

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::SetID(int id)
{
	this->id = id;
	return this;
}

void PEditor::ScriptCreationUtilities::ScriptNode::Render()
{
	if (hidden) return;
	render();
}

void PEditor::ScriptCreationUtilities::ScriptNode::Hide()
{
	hidden = true;
}

void PEditor::ScriptCreationUtilities::ScriptNode::Show()
{
	hidden = false;
}

bool PEditor::ScriptCreationUtilities::ScriptNode::IsHidden()
{
	return hidden;
}

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::SetPosition(int x1, int y1)
{
	this->x = x1;
	this->y = y1;
	return this;
}

void PEditor::ScriptCreationUtilities::ScriptNode::render()
{
}

void PEditor::ScriptCreationUtilities::ScriptDropdownSelection::render()
{
	ImGui::SetCursorPos(ImVec2(x, y));

	//TODO: gaurdar el string para no calcularlo en cada frame
	/*if (ImGui::BeginChild(("node " + std::to_string(id)).c_str(), ImVec2(x, y), true, ImGuiWindowFlags_None))
	{



		ImGui::Text("Child Window Content Here");
	}*/


	if (ImGui::BeginPopupContextWindow("El menu este", ImGuiPopupFlags_MouseButtonRight)) {

		bool endMenu = false;

		ImGui::SetWindowFontScale(2);

		auto& allComponents = Components::ComponentManager::GetAllComponents();

		for (auto& comp : allComponents) {


			auto& methods = comp.second.getAllMethods();

			if (methods.size() > 0)
				if (ImGui::CollapsingHeader(comp.first.c_str())) {

					for (auto& method : methods) {

						if (ImGui::MenuItem(method.second.getName().c_str())) {

						}
					}
				}
		}

		ImGui::EndPopup();
	}



	// ImGui::EndChild();

}
