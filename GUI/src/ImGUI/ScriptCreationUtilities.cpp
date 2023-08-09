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

	int scrollx, scrolly;
	ScriptCreation::GetScrollPosition(&scrollx, &scrolly);

	auto position = ImVec2(x + scrollx, y + scrolly);
	auto size = ImVec2(w, h);

	ImGui::Begin(GetStringId().c_str(), NULL, ImGuiWindowFlags_None);

	if (ScriptCreation::ScrolledThisFrame())
		ImGui::SetWindowPos(position, ImGuiCond_None);
	else
		ImGui::SetWindowPos(position, ImGuiCond_Once);
	ImGui::SetWindowSize(size, ImGuiCond_Once);



	render();
	ManagerOutputNode();


	UpdatePosition(scrollx, scrolly);

	ImGui::End();
}

void PEditor::ScriptCreationUtilities::ScriptNode::UpdatePosition(int scrollx, int scrolly)
{
	auto windowPos = ImGui::GetWindowPos();
	x = windowPos.x - scrollx;
	y = windowPos.y - scrolly;
}

void PEditor::ScriptCreationUtilities::ScriptNode::ManagerOutputNode()
{
	auto drawList = ImGui::GetWindowDrawList();

	auto windowSize = ImGui::GetWindowSize();
	auto windowPosition = ImGui::GetWindowPos();

	float nodeSize = ImGui::IsWindowCollapsed() ? 4 : 15;

	auto outputOffset = ImGui::IsWindowCollapsed() ? 4 : 30;

	auto outputNodeTLRelativePosition = ImVec2(windowSize.x - nodeSize - outputOffset, windowSize.y * 0.5f - nodeSize);

	auto outputNodeTLPosition = ImVec2(outputNodeTLRelativePosition.x + windowPosition.x, outputNodeTLRelativePosition.y + windowPosition.y);
	auto outputNodeCenterPosition = ImVec2(outputNodeTLPosition.x + nodeSize, outputNodeTLPosition.y + nodeSize);
	auto outputNodeBRPosition = ImVec2(outputNodeCenterPosition.x + nodeSize, outputNodeCenterPosition.y + nodeSize);

	ImGui::SetCursorPos(outputNodeTLRelativePosition);
	ImGui::BeginChild((GetStringId() + "output node").c_str(), ImVec2(nodeSize * 2, nodeSize * 2), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	auto a = ImVec2(outputNodeCenterPosition.x - nodeSize, outputNodeCenterPosition.y - nodeSize);
	auto b = ImVec2(outputNodeCenterPosition.x - nodeSize, outputNodeCenterPosition.y + nodeSize);
	auto c = ImVec2(outputNodeCenterPosition.x + nodeSize, outputNodeCenterPosition.y);


	bool isCurrentlySelected = currentlySelected == this;

	if (ImGui::IsMouseHoveringRect(outputNodeTLPosition, outputNodeBRPosition, true))
	{
		if (ImGui::IsMouseClicked(0) || isCurrentlySelected)
		{
			currentlySelected = this;
			//drawList->AddCircleFilled(outputNodeCenterPosition, nodeSize, IM_COL32(255, 0, 0, 255), 30);
			drawList->AddTriangleFilled(a, b, c, IM_COL32(255, 255, 255, 255));
		}

		else
			//drawList->AddCircleFilled(outputNodeCenterPosition, nodeSize, IM_COL32(255, 0, 255, 255), 30);
			drawList->AddTriangle(a, b, c, IM_COL32(150, 100, 100, 255), 1);
	}
	else {


		if (isCurrentlySelected)
			drawList->AddTriangleFilled(a, b, c, IM_COL32(255, 255, 255, 255));
		else
			//drawList->AddCircleFilled(outputNodeCenterPosition, nodeSize, IM_COL32(255, 255, 255, 255), 30);
			drawList->AddTriangle(a, b, c, IM_COL32(255, 255, 255, 255), 1);

	}

	ImGui::EndChild();



	if (isCurrentlySelected) {

		auto mousePos = ImGui::GetMousePos();
		Bezier::Draw(outputNodeCenterPosition.x + nodeSize, outputNodeCenterPosition.y, mousePos.x, mousePos.y);
	}



	if (ImGui::IsWindowCollapsed()) {

		Bezier::Draw(windowPosition.x, windowPosition.y, 400, 700);
	}
	else {

		Bezier::Draw(windowPosition.x, windowPosition.y, 1, 700);
	}



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

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::currentlySelected = nullptr;

void PEditor::ScriptCreationUtilities::ScriptNode::SetOutput(ScriptMethod* node)
{
	output = node;
}

PEditor::ScriptCreationUtilities::ScriptMethod* PEditor::ScriptCreationUtilities::ScriptNode::GetOutput()
{
	return output;
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

	//Mover la logica del output a scriptNode

	auto drawList = ImGui::GetWindowDrawList();


	ImGui::Indent();
	for (auto& in : method.getInput()) {

		auto relpos = ImGui::GetCursorPos();
		ImGui::Text(in.getName().c_str());

		auto min = ImGui::GetItemRectMin();
		auto max = ImGui::GetItemRectMax();


		if (ImGui::IsMouseHoveringRect(min, max)) {

			ImGui::SetTooltip(in.getTypeStr().c_str());
		}


		ImGui::SetCursorPos(ImVec2(relpos.x - 5, relpos.y + 3));
		ImGui::BeginChild((GetStringId() + in.getName()).c_str(), ImVec2(5, 10), true, ImGuiWindowFlags_None);




		ImGui::EndChild();



		auto a = ImVec2(min.x - 10, min.y + 3);
		auto b = ImVec2(min.x - 10, min.y + 13);
		auto c = ImVec2(min.x - 15, min.y + 8);



		if (ImGui::IsMouseHoveringRect(ImVec2(c.x, a.y), b)) {

			drawList->AddTriangleFilled(a, b, c, IM_COL32(255, 255, 255, 255));
		}
		else

			drawList->AddTriangle(a, b, c, IM_COL32(255, 255, 255, 255), 1);

		ImGui::Spacing();
	}

}

std::string PEditor::ScriptCreationUtilities::ScriptMethod::GetStringId()
{
	return method.getName() + " - " + method.getComponent() + " (" + std::to_string(id) + ")";
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


int PEditor::ScriptCreationUtilities::Grid::interval = 5;
float PEditor::ScriptCreationUtilities::Grid::thickness = 1;
float PEditor::ScriptCreationUtilities::Grid::intervalScale = 4;
int PEditor::ScriptCreationUtilities::Grid::spacing = 50;
int PEditor::ScriptCreationUtilities::Grid::x_offset = 0;
int PEditor::ScriptCreationUtilities::Grid::y_offset = 0;

int PEditor::ScriptCreationUtilities::Grid::r = 255;
int PEditor::ScriptCreationUtilities::Grid::g = 255;
int PEditor::ScriptCreationUtilities::Grid::b = 255;
int PEditor::ScriptCreationUtilities::Grid::a = 255;



void PEditor::ScriptCreationUtilities::Grid::SetSpacing(int spacing)
{
	Grid::spacing = spacing;
}

void PEditor::ScriptCreationUtilities::Grid::ResetSpacing()
{
	Grid::spacing = 50;
}

void PEditor::ScriptCreationUtilities::Grid::SetOffset(int x, int y)
{
	x_offset = x;
	y_offset = y;
}

void PEditor::ScriptCreationUtilities::Grid::ResetOffset()
{
	x_offset = y_offset = 0;
}

void PEditor::ScriptCreationUtilities::Grid::SetInterval(int interval)
{
	Grid::interval = interval;
}

void PEditor::ScriptCreationUtilities::Grid::ResetInterval()
{
	Grid::interval = 5;
}

void PEditor::ScriptCreationUtilities::Grid::SetColor(int r, int g, int b, int a)
{
	Grid::r = r;
	Grid::g = g;
	Grid::b = b;
	Grid::a = a;
}

void PEditor::ScriptCreationUtilities::Grid::ResetColor()
{
	r = g = b = a = 255;
}

void PEditor::ScriptCreationUtilities::Grid::Draw()
{
	auto drawList = ImGui::GetWindowDrawList();
	auto windowSize = ImGui::GetWindowSize();

	int interval_x = (-x_offset / spacing) % interval;
	int interval_y = (-y_offset / spacing) % interval;

	float x_off = x_offset % spacing;
	float y_off = y_offset % spacing;

	for (int row = y_off; row < windowSize.y; row += spacing) {

		float t = interval_y == 0 ? thickness * intervalScale : thickness;

		drawList->AddLine(ImVec2(0, row), ImVec2(windowSize.x, row), IM_COL32(r, g, b, a), t);

		interval_y = (interval_y + 1) % interval;
	}

	for (int col = x_off; col < windowSize.x; col += spacing) {

		float t = interval_x == 0 ? thickness * intervalScale : thickness;

		drawList->AddLine(ImVec2(col, 0), ImVec2(col, windowSize.y), IM_COL32(r, g, b, a), t);

		interval_x = (interval_x + 1) % interval;
	}




}
