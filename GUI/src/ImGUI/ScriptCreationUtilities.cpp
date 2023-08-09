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

	ImGui::Begin(GetStringId().c_str(), NULL, ImGuiWindowFlags_None);

	ImGui::SetWindowPos(position, ImGuiCond_Once);
	ImGui::SetWindowSize(size, ImGuiCond_Once);
	render();
	
	ImGui::End();
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

std::string PEditor::ScriptCreationUtilities::ScriptNode::GetStringId()
{
	return std::string();
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
	auto drawList = ImGui::GetForegroundDrawList();

	auto windowSize = ImGui::GetWindowSize();
	auto windowPosition = ImGui::GetWindowPos();

	float nodeSize = 15;
	auto nodeConnectionPosition = ImVec2(windowPosition.x + windowSize.x, windowPosition.y + windowSize.y * 0.5f);


	for (auto& in : method.getInput()) {

		ImGui::Text(in.getName().c_str());

	}

	/*ImGui::SetCursorPos(nodeConnectionPosition);
	ImGui::BeginChild((GetStringId() + "output node").c_str(), ImVec2(nodeSize, nodeSize), true, ImGuiWindowFlags_None);

	ImGui::Text("Output");


	ImGui::EndChild();*/

	if (ImGui::IsWindowCollapsed()) {

		Bezier::Draw(windowPosition.x, windowPosition.y, 400, 700);
	}
	else {

		Bezier::Draw(windowPosition.x, windowPosition.y, 1, 700);
	}

	drawList->AddCircleFilled(nodeConnectionPosition, nodeSize, IM_COL32(255, 255, 255, 255), 30);
}

std::string PEditor::ScriptCreationUtilities::ScriptMethod::GetStringId()
{
	return method.getName() + " - " + method.getComponent();
}




int PEditor::ScriptCreationUtilities::Bezier::pointCount = 50;
float PEditor::ScriptCreationUtilities::Bezier::thickness = 1;


void PEditor::ScriptCreationUtilities::Bezier::ResetThickness()
{
	thickness = 1;
}

void PEditor::ScriptCreationUtilities::Bezier::SetThickness(float t)
{
	thickness = 3;
}

void PEditor::ScriptCreationUtilities::Bezier::SetPointCount(int c)
{
	pointCount = c;
}

void PEditor::ScriptCreationUtilities::Bezier::ResetPointCount()
{
	pointCount = 50;
}

void PEditor::ScriptCreationUtilities::Bezier::Draw(int x, int y, int x1, int y1)
{
	ImVec2 a = ImVec2(x, y);
	ImVec2 d = ImVec2(x1, y1);

	float x_distance = d.x - a.x;
	const float bezierMagnitude = 0.5f;

	float x_increment = x_distance * bezierMagnitude;

	ImVec2 b = ImVec2(x + x_increment, y);
	ImVec2 c = ImVec2(x1 - x_increment, y1);


	auto drawList = ImGui::GetForegroundDrawList();
	drawList->AddBezierCubic(a, b, c, d, IM_COL32(255, 255, 255, 255), thickness, pointCount);
}
