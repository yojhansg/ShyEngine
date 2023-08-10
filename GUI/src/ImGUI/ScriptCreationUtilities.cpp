#include "ScriptCreationUtilities.h"

#include "imgui.h"
#include "ScriptCreation.h"
#include "ComponentManager.h"
#include "ImGUIManager.h"
#include <fstream>
#include <algorithm>

PEditor::ScriptCreationUtilities::ScriptNode::ScriptNode()
{
	id = x = y = 0;
	w = 200;
	h = 300;
	nodeSize = 15;
}

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::SetID(int id)
{
	this->id = id;


	return this;
}

void PEditor::ScriptCreationUtilities::ScriptNode::Render()
{
	int scrollx, scrolly;
	ScriptCreation::GetScrollPosition(&scrollx, &scrolly);

	auto position = ImVec2(x + scrollx, y + scrolly);
	auto size = ImVec2(w, h);

	auto windowSize = ImGui::GetWindowSize();

	//Sombra

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	//ImGui::Begin((GetStringId() + "_shadow").c_str(), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	//ImGui::SetWindowPos(ImVec2(position.x + 10, position.y + 10), ImGuiCond_None);
	//ImGui::SetWindowSize(size, ImGuiCond_Once);

	//ImGui::End();
	//ImGui::PopStyleVar();


	if (
		position.x <= windowSize.x && position.y <= windowSize.y &&
		position.x + size.x >= 0 && position.y + size.y >= 0
		)
	{
		ImGui::Begin(GetStringId().c_str(), NULL, ImGuiWindowFlags_NoSavedSettings);

		if (ScriptCreation::ScrolledThisFrame())
			ImGui::SetWindowPos(position, ImGuiCond_None);
		else
			ImGui::SetWindowPos(position, ImGuiCond_Once);

		ImGui::SetWindowSize(size, ImGuiCond_None);

		render();
		ManagerOutputNode();


		UpdatePosition(scrollx, scrolly);

		ImGui::End();
	}
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

	auto outputNodePosition = ImVec2(0, 0);
	GetOutputNodePosition(&outputNodePosition.x, &outputNodePosition.y);

	auto outputNodePositionTop = ImVec2(outputNodePosition.x - nodeSize, outputNodePosition.y - nodeSize);
	auto outputNodePositionBottom = ImVec2(outputNodePosition.x - nodeSize, outputNodePosition.y + nodeSize);


	ImGui::SetCursorPos(ImVec2(outputNodePositionTop.x - windowPosition.x, outputNodePositionTop.y - windowPosition.y));
	ImGui::BeginChild((GetStringId() + "output node").c_str(), ImVec2(nodeSize, nodeSize * 2), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	bool isCurrentlySelected = currentlySelected == this;

	if (ImGui::IsMouseHoveringRect(outputNodePositionTop, ImVec2(outputNodePosition.x, outputNodePositionBottom.y), true))
	{
		if (ImGui::IsMouseClicked(0) || isCurrentlySelected)
		{
			currentlySelected = this;
			drawList->AddTriangleFilled(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, IM_COL32(255, 255, 255, 255));
		}

		else
			drawList->AddTriangle(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, IM_COL32(150, 100, 100, 255), 1);
	}
	else {


		if (isCurrentlySelected)
			drawList->AddTriangleFilled(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, IM_COL32(255, 255, 255, 255));
		else
			drawList->AddTriangle(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, IM_COL32(255, 255, 255, 255), 1);

	}

	ImGui::EndChild();



	if (isCurrentlySelected) {

		auto mousePos = ImGui::GetMousePos();
		Bezier::Draw(outputNodePosition.x, outputNodePosition.y, mousePos.x, mousePos.y);
	}



	//if (ImGui::IsWindowCollapsed()) {

	//	Bezier::Draw(windowPosition.x, windowPosition.y, 1, 700);
	//}
	//else {

	//	Bezier::Draw(windowPosition.x, windowPosition.y, 1, 700);
	//}



}

void PEditor::ScriptCreationUtilities::ScriptNode::GetOutputNodePosition(float* x, float* y)
{
	const auto outputOffset = 15;

	int scroll_x, scroll_y;
	ScriptCreation::GetScrollPosition(&scroll_x, &scroll_y);

	if (x != nullptr)
		*x = scroll_x + w - outputOffset + this->x;
	if (y != nullptr)
		*y = scroll_y + h * 0.5f + this->y;

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

int PEditor::ScriptCreationUtilities::ScriptNode::GetW()
{
	return w;
}

int PEditor::ScriptCreationUtilities::ScriptNode::GetH()
{
	return h;
}

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::currentlySelected = nullptr;

PEditor::ScriptCreationUtilities::ScriptNode* PEditor::ScriptCreationUtilities::ScriptNode::SetPosition(int x1, int y1)
{
	this->x = x1;
	this->y = y1;

	return this;
}

std::string PEditor::ScriptCreationUtilities::ScriptNode::GetStringId()
{
	return std::to_string(id);
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

		AddValuesFromVector(Components::ComponentManager::GetAllComponents());
		AddValuesFromVector(Components::ComponentManager::GetAllManagers());

		ImGui::EndPopup();
	}
}

void PEditor::ScriptCreationUtilities::ScriptDropdownSelection::AddValuesFromVector(std::unordered_map<std::string, Components::Component>& v)
{
	for (auto& comp : v) {

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
}



PEditor::ScriptCreationUtilities::ScriptMethod::ScriptMethod(::Components::Method& method) : method(method)
{
	input = std::vector<ScriptNode*>(method.getInput().size());
	type = Node::Method;
}


void PEditor::ScriptCreationUtilities::ScriptMethod::render()
{

	//Mover la logica del output a scriptNode

	auto drawList = ImGui::GetWindowDrawList();


	ImGui::Indent();

	int idx = 0;
	for (auto& in : method.getInput()) {

		auto relpos = ImGui::GetCursorPos();
		ImGui::Text(in.getName().c_str());

		auto min = ImGui::GetItemRectMin();
		auto max = ImGui::GetItemRectMax();


		if (ImGui::IsMouseHoveringRect(min, max)) {

			ImGui::SetTooltip(in.getTypeStr().c_str());
		}


		ImGui::SetCursorPos(ImVec2(relpos.x - 15, relpos.y + 3));
		ImGui::BeginChild((GetStringId() + in.getName()).c_str(), ImVec2(5, 10), true, ImGuiWindowFlags_None);


		ImGui::EndChild();


		auto a = ImVec2(min.x - 10, min.y + 3);
		auto b = ImVec2(min.x - 10, min.y + 13);
		auto c = ImVec2(min.x - 15, min.y + 8);


		if (ImGui::IsMouseHoveringRect(ImVec2(c.x, a.y), b) && ImGui::IsMouseDoubleClicked(0)) {

			input[idx] = nullptr;
		}

		if (ScriptNode::currentlySelected != nullptr && ImGui::IsMouseHoveringRect(ImVec2(c.x, a.y), b)) {


			if (ImGui::IsMouseReleased(0)) {

				input[idx] = ScriptNode::currentlySelected;
				ScriptNode::currentlySelected = nullptr;
			}


			drawList->AddTriangleFilled(a, b, c, IM_COL32(255, 255, 255, 255));
		}
		else

			if (input[idx] != nullptr)
				drawList->AddTriangleFilled(a, b, c, IM_COL32(255, 255, 255, 255));
			else
				drawList->AddTriangle(a, b, c, IM_COL32(255, 255, 255, 255), 1);



		if (input[idx] != nullptr) {

			float output_x, output_y;
			input[idx]->GetOutputNodePosition(&output_x, &output_y);
			Bezier::Draw(c.x, c.y, output_x, output_y);
		}

		ImGui::Spacing();
		idx++;
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

PEditor::ScriptCreationUtilities::ScriptInput::ScriptInput(::Components::AttributesType type) : attrType(type)
{
	reflect = false;

	attrValue = ::Components::AttributeValue();

	attrValue.value.valueFloat = 0;

	this->type = Node::Input;
}

PEditor::ScriptCreationUtilities::ScriptNode::Node PEditor::ScriptCreationUtilities::ScriptNode::GetType()
{
	return type;
}


void PEditor::ScriptCreationUtilities::ScriptInput::render()
{

	switch (attrType) {

	case ::Components::AttributesType::NONE:
		break;
	case ::Components::AttributesType::FLOAT:
		ImGui::InputFloat("Value", &attrValue.value.valueFloat);
		break;
	case ::Components::AttributesType::VECTOR2:
	{

		float values[2]{};
		values[0] = attrValue.value.valueVector2.x;
		values[1] = attrValue.value.valueVector2.y;
		ImGui::InputFloat2("Value", values);

		attrValue.value.valueVector2.x = values[0];
		attrValue.value.valueVector2.y = values[1];
		break;
	}
	case ::Components::AttributesType::BOOL:

		ImGui::Checkbox("Value", &attrValue.value.valueBool);
		break;
	case ::Components::AttributesType::COLOR:
	{
		float values[3]{};
		values[0] = attrValue.value.valueColor.r;
		values[1] = attrValue.value.valueColor.g;
		values[2] = attrValue.value.valueColor.b;
		ImGui::ColorEdit3("Value", values, ImGuiColorEditFlags_DisplayRGB);

		attrValue.value.valueColor.r = values[0];
		attrValue.value.valueColor.g = values[1];
		attrValue.value.valueColor.b = values[2];
		break;
	}
	case ::Components::AttributesType::STRING:

	{
		char values[64]{};
		std::memcpy(values, attrValue.valueString.c_str(), 64);

		ImGui::InputText("Value", values, 64);

		attrValue.valueString = values;
		break;
	}
	}

}


void PEditor::ScriptCreationUtilities::ScriptMenuBar::Close()
{
	nameBuffer[0] = '\0';
	creator->ClearScript();
	ImGUIManager::getInstance()->creatingScript(false);
}

PEditor::ScriptCreationUtilities::ScriptMenuBar::ScriptMenuBar(ScriptCreation* creator) : creator(creator)
{
	showPopup = false;
}

void PEditor::ScriptCreationUtilities::ScriptMenuBar::SetName(const std::string& name)
{
	std::memcpy(nameBuffer, name.c_str(), 256);
}

void PEditor::ScriptCreationUtilities::ScriptMenuBar::Render()
{
	int scrollx, scrolly;
	ScriptCreation::GetScrollPosition(&scrollx, &scrolly);
	auto windowSize = ImGui::GetWindowSize();


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(20, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(1, 0));
	if (ImGui::BeginMenuBar()) {


		ImGui::Text("Name the script");


		ImGui::InputText("", nameBuffer, CharBufferSize);


		if (ImGui::BeginMenu("Values")) {

			::Components::AttributesType type = ::Components::AttributesType::NONE;

			if (ImGui::MenuItem("Number")) {

				type = ::Components::AttributesType::FLOAT;
			}
			if (ImGui::MenuItem("Vector")) {

				type = ::Components::AttributesType::VECTOR2;
			}
			if (ImGui::MenuItem("Letter")) {

				//TODO: hacer chars
				//type = ::Components::AttributesType::CHAR;
			}
			if (ImGui::MenuItem("Word")) {

				type = ::Components::AttributesType::STRING;
			}
			if (ImGui::MenuItem("Color")) {

				type = ::Components::AttributesType::COLOR;
			}


			if (type != ::Components::AttributesType::NONE)
			{


				ScriptNode* node = new ScriptInput(type);

				node->SetPosition((windowSize.x - node->GetW()) * 0.5f - scrollx, (windowSize.y - node->GetH()) * 0.5f - scrolly);

				creator->AddNode(node);

				ScriptCreation::SetFileModified();
			}

			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Logic")) {

			ImGui::MenuItem("If");
			ImGui::MenuItem("While");
			ImGui::MenuItem("Loop");
			ImGui::EndMenu();
		}


		if (ImGui::Button("Save script")) {

			Save();
		}

		if (ImGui::Button("Close")) {

			showPopup = true;

		}


		if (ImGui::InputText("Node search", nameSearch, CharBufferSize)) {

			showNodeSearch = std::string(nameSearch).length() > 0;
		}

		if (showNodeSearch) {

			if (ImGui::BeginMenu("Node search result")) {

				AddMatchingMethods(Components::ComponentManager::GetAllComponents(), windowSize.x, windowSize.y);
				AddMatchingMethods(Components::ComponentManager::GetAllManagers(), windowSize.x, windowSize.y);

				ImGui::EndMenu();
			}
		}


		ImGui::EndMenuBar();
	}
	ImGui::PopStyleVar(3);



	if (showPopup) {

		ImGui::OpenPopup("Close without saving");
		auto size = ImGui::GetWindowSize();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0)); // Centrar el pop-up
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);
		ImGui::Begin("Background", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(size.x * 0.5f, size.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Centrar el pop-up

	}
	if (ImGui::BeginPopup("Close without saving")) {

		ImGui::Text("Esto es un mensaje emergente");
		bool closePopup = false;
		if (ImGui::Button("Cancel")) {
			closePopup = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Save without exit")) {
			closePopup = true;

			Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("Save and exit")) {
			closePopup = true;
			Save();
			Close();
		}

		if (closePopup) {

			showPopup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}


void PEditor::ScriptCreationUtilities::ScriptMenuBar::AddMatchingMethods(std::unordered_map<std::string, Components::Component>& v, int windowW, int windowH)
{

	for (auto& comp : v) {

		auto& methods = comp.second.getAllMethods();
		if (methods.size() > 0) {

			std::vector<std::string> methodsToAdd;

			for (auto& method : methods) {

				std::string name = method.first;
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				std::string search = nameSearch;
				std::transform(search.begin(), search.end(), search.begin(), ::tolower);

				if (name.find(search) != name.npos) {

					methodsToAdd.push_back(method.first);
				}

			}

			if (methodsToAdd.size() > 0)
				if (ImGui::CollapsingHeader(comp.first.c_str())) {

					for (auto& method : methodsToAdd) {

						if (ImGui::MenuItem(method.c_str())) {


							int scrollx, scrolly;
							ScriptCreation::GetScrollPosition(&scrollx, &scrolly);

							ScriptNode* node = new ScriptMethod(comp.second.getAllMethods()[method]);
							


							node->SetPosition((windowW - node->GetW()) * 0.5f - scrollx, (windowH - node->GetH()) * 0.5f - scrolly);

							creator->AddNode(node);

							showNodeSearch = false;
							nameSearch[0] = '\0';
						}
					}
				}
		}
	}


}




#include "nlohmann/json.hpp"

using nlohmann::json;


void PEditor::ScriptCreationUtilities::ScriptMenuBar::Load()
{


}



void PEditor::ScriptCreationUtilities::ScriptMenuBar::Save()
{
	auto& allNodes = creator->GetNodes();

	json root;
	json functions = json::array(), consts = json::array();

	root["nodeCount"] = allNodes.size();

	std::vector<ScriptNode*> serializedValues;

	for (auto node : allNodes) {

		if (node->GetType() == ScriptNode::Node::Method) {

			functions.push_back(node->ToJson());
		}


		else if (node->GetType() == ScriptNode::Node::Input) {


			functions.push_back(node->ToJson());
		}
	}


	root["functions"] = functions;
	root["consts"] = consts;

	std::ofstream file("scripts/" + std::string(nameBuffer) + ".script");

	file << root.dump(4);

	file.close();

	ScriptCreation::ResetModified();
}



json PEditor::ScriptCreationUtilities::ScriptNode::ToJson()
{
	json root;

	root["index"] = id;
	root["x"] = x;
	root["y"] = y;

	return root;
}

void PEditor::ScriptCreationUtilities::ScriptNode::FromJson(nlohmann::json& root)
{
	id = root["index"].get<int>();

	x = root["x"].get<float>();
	y = root["y"].get<float>();
}



nlohmann::json PEditor::ScriptCreationUtilities::ScriptMethod::ToJson()
{
	json root = ScriptNode::ToJson();

	root["function"] = method.getName();

	root["input"] = json::array();

	for (auto& in : input) {

		root["input"].push_back(in->GetId());
	}

	return root;
}



nlohmann::json PEditor::ScriptCreationUtilities::ScriptInput::ToJson()
{
	json root = ScriptNode::ToJson();

	//TODO: serialised values

	switch (attrType) {

		switch (attrType)
		{
		case Components::AttributesType::NONE:
			root["type"] = "null";
			root["value"] = nullptr;
			break;
		case Components::AttributesType::FLOAT:
			root["type"] = "float";
			root["value"] = attrValue.value.valueFloat;
			break;
		case Components::AttributesType::VECTOR2:
			root["type"] = "vector2D";
			root["value"] = std::to_string(attrValue.value.valueVector2.x) + "," + std::to_string(attrValue.value.valueVector2.y);
			break;
		case Components::AttributesType::STRING:
			root["type"] = "string";
			root["value"] = attrValue.valueString;
			break;
		case Components::AttributesType::BOOL:
			root["type"] = "number";
			root["value"] = attrValue.value.valueBool;
			break;
		case Components::AttributesType::COLOR:
			root["type"] = "color";
			root["value"] = std::to_string(attrValue.value.valueColor.r) + "," + std::to_string(attrValue.value.valueColor.g) + ", " + std::to_string(attrValue.value.valueColor.b);
			break;
		default:
			break;
		}
	}


	return root;
}
