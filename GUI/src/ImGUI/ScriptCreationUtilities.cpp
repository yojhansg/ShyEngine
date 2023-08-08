#include "ScriptCreationUtilities.h"

#include "imgui.h"
#include "ScriptCreation.h"
#include "ComponentManager.h"

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::SetID(int id)
{
	this->id = id;
	w = 200;
	h = 300;

	return this;
}

void PEditor::ScriptCreationUtilities::ScriptNode::Render()
{
	if (hidden) return;

	auto position = ImVec2(x, y);
	auto size = ImVec2(w, h);

	ImGui::SetNextWindowPos(position);

	if (ImGui::Begin(std::to_string(id).c_str(), NULL, ImGuiWindowFlags_None)) {
		render();

	}
	ImGui::End();

	//if (ImGui::BeginChild(std::to_string(id).c_str(), size, true, ImGuiWindowFlags_None))
	//{
	//	render();
	//	ImGui::EndChild();
	//}
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

int PEditor::ScriptCreationUtilities::ScriptNode::GetId()
{
	return id;
}

int PEditor::ScriptCreationUtilities::ScriptNode::GetY()
{
	return y;
}

int PEditor::ScriptCreationUtilities::ScriptNode::GetX()
{
	return x;
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

PEditor::ScriptCreationUtilities::ScriptDropdownSelection::ScriptDropdownSelection(ScriptCreation* creator) : creator(creator)
{
	mousex = mousey = 0;
}

void PEditor::ScriptCreationUtilities::ScriptDropdownSelection::Render()
{
	if (ImGui::IsMouseReleased(1)) {
		auto mousePos = ImGui::GetMousePos();

		mousex = mousePos.x;
		mousey = mousePos.y;
	}

	if (ImGui::BeginPopupContextWindow("PopupScript", ImGuiPopupFlags_MouseButtonRight)) {

		bool endMenu = false;

		ImGui::SetWindowFontScale(2);

		auto& allComponents = Components::ComponentManager::GetAllComponents();

		for (auto& comp : allComponents) {


			auto& methods = comp.second.getAllMethods();

			if (methods.size() > 0)
				if (ImGui::CollapsingHeader(comp.first.c_str())) {

					for (auto& method : methods) {

						std::string name = method.first;
						if (ImGui::MenuItem(name.c_str())) {

							ScriptNode* node = new ScriptMethod(method.second);

							node->SetPosition(mousex, mousey);

							creator->AddNode(node);
						}
					}
				}
		}

		ImGui::EndPopup();
	}
}

PEditor::ScriptCreationUtilities::ScriptMethod::ScriptMethod(::Components::Method& method) : method(method)
{
}

void PEditor::ScriptCreationUtilities::ScriptMethod::render()
{
	ImGui::Text((method.getName() + " - " + method.getComponent()).c_str());




}
