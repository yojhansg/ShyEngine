#include "ScriptCreation.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include <iostream>

#include <fstream>
#include "ScriptCreationUtilities.h"

#include "ComponentManager.h"

#include "nlohmann/json.hpp"
using nlohmann::json;

PEditor::ScriptCreation* PEditor::ScriptCreation::instance = nullptr;

PEditor::ScriptCreation::ScriptCreation() : Window("", None)
{
	imGuiManager = ImGUIManager::getInstance();

	dropDownSelection = new ScriptCreationUtilities::ScriptDropdownSelection(this);

	menuBar = new ScriptCreationUtilities::ScriptMenuBar(this);

	xpos = ypos = 0;
	scrollx = scrolly = 0;

	lerping = false;
	lerpDuration = 1;
	lerp_t = 0;
	initialx = initialy = finalx = finaly = 0;

	modified = false;
	scrolled = false;

	instance = this;
}

PEditor::ScriptCreation::~ScriptCreation()
{
	ClearScript();

	instance = nullptr;
	delete dropDownSelection;
	delete menuBar;

}

PEditor::ScriptCreationUtilities::ScriptEvent* PEditor::ScriptCreation::ContainsEvent(const std::string& event)
{
	if (events.contains(event))
		return events[event];

	return nullptr;
}

void PEditor::ScriptCreation::AddEvent(const std::string& name, ScriptCreationUtilities::ScriptEvent* event)
{
	events.emplace(name, event);

	event->SetID(-1);
	nodes.push_back(event);
}

void PEditor::ScriptCreation::AddNode(ScriptCreationUtilities::ScriptNode* node)
{
	node->SetID(nodes.size());
	nodes.push_back(node);
}

void PEditor::ScriptCreation::RemoveEvent(const std::string& name)
{
	instance->events.erase(name);
}

void PEditor::ScriptCreation::SetNodeCount(int count)
{
	nodes = std::vector<ScriptCreationUtilities::ScriptNode*>(count);
}

void PEditor::ScriptCreation::SetNode(int n, ScriptCreationUtilities::ScriptNode* node)
{
	nodes[n] = node;
}

void PEditor::ScriptCreation::Save()
{
	auto& allNodes = GetNodes();

	json root;
	json functions = json::array(), consts = json::array();

	root["nodeCount"] = allNodes.size();

	std::vector<ScriptCreationUtilities::ScriptNode*> serializedValues;

	for (auto node : allNodes) {

		if (node->GetId() < 0) {
			//es un evento
			continue;
		}

		if (node->GetType() == ScriptCreationUtilities::ScriptNode::Node::Method) {

			functions.push_back(node->ToJson());
		}


		else if (node->GetType() == ScriptCreationUtilities::ScriptNode::Node::Input) {


			consts.push_back(node->ToJson());
		}
	}



	root["functions"] = functions;
	root["consts"] = consts;

	std::ofstream file("scripts/" + std::string(menuBar->GetName()) + ".script");

	file << root.dump(4);

	file.close();

	ScriptCreation::ResetModified();
}

void PEditor::ScriptCreation::Load()
{
	std::string fileName = std::string(menuBar->GetName());
	if (fileName.size() == 0) {


		ScriptCreationUtilities::ScriptEvent* start = new ScriptCreationUtilities::ScriptEvent("start");
		ScriptCreationUtilities::ScriptEvent* update = new ScriptCreationUtilities::ScriptEvent("update");

		start->SetPosition(250, 250);
		update->SetPosition(250, 750);


		AddEvent("start", start);
		AddEvent("update", update);

		modified = true;
		return;
	}

	std::ifstream fileStream("scripts/" + fileName + ".script");

	if (!fileStream.good() || !json::accept(fileStream))
	{
		return;
	}

	fileStream.clear();
	fileStream.seekg(0);

	json file = json::parse(fileStream);
	fileStream.close();

	json& functions = file["functions"];
	json& consts = file["consts"];

	SetNodeCount(file["nodeCount"].get<int>());


	struct MethodInput {
		class ScriptCreationUtilities::ScriptMethod* node;
		int pos;
		int inputNode;
	};

	std::vector<MethodInput> methodInfo;


	for (auto& funcNode : functions) {

		std::string completeName = funcNode["function"];
		size_t separator = completeName.find("_");
		std::string className = completeName.substr(0, separator);
		std::string functionName = completeName.substr(separator + 1);

		ScriptCreationUtilities::ScriptMethod* method;

		if (Components::ComponentManager::GetAllComponents().contains(className)) {

			method = new ScriptCreationUtilities::ScriptMethod(Components::ComponentManager::GetAllComponents()[className].getMethod(functionName));
		}
		else {
			method = new ScriptCreationUtilities::ScriptMethod(Components::ComponentManager::GetAllManagers()[className].getMethod(functionName));
		}


		int idx = 0;
		for (auto& in : funcNode["input"]) {

			int inputNode = in.get<int>();
			if (inputNode >= 0)
				methodInfo.push_back({ method, idx, inputNode });

			idx++;
		}



		method->FromJson(funcNode);

		SetNode(method->GetId(), method);
	}

	for (auto& constNode : consts) {

		::Components::AttributesType type = ::Components::AttributesType::NONE;
		::Components::AttributeValue value;

		std::string typeStr = constNode["type"].get<std::string>();

		if (typeStr == "null") {
			type = ::Components::AttributesType::NONE;
			value.value.valueFloat = 0;
		}
		else if (typeStr == "float") {
			type = ::Components::AttributesType::FLOAT;
			value.value.valueFloat = constNode["value"].get<float>();
		}
		else if (typeStr == "vector2D") {
			//TODO: despues de la serializacion
		}
		else if (typeStr == "string") {
			type = ::Components::AttributesType::STRING;
			value.valueString = constNode["value"].get<float>();
		}
		else if (typeStr == "bool") {
			type = ::Components::AttributesType::BOOL;
			value.value.valueBool = constNode["value"].get<bool>();
		}
		else if (typeStr == "color") {

			//TODO: despues de la serializacion
		}

		ScriptCreationUtilities::ScriptInput* input = new ScriptCreationUtilities::ScriptInput(type);
		input->SetValue(value);

		input->FromJson(constNode);


		SetNode(input->GetId(), input);
	}


	for (auto& mi : methodInfo) {

		mi.node->SetInput(mi.pos, GetNodes()[mi.inputNode]);
	}

}

void PEditor::ScriptCreation::GetScrollPosition(int* x, int* y)
{
	if (x != nullptr)
		*x = instance->xpos + instance->scrollx;

	if (y != nullptr)
		*y = instance->ypos + instance->scrolly;
}

bool PEditor::ScriptCreation::ScrolledThisFrame()
{
	return instance->scrolled;
}

bool PEditor::ScriptCreation::IsFileModified()
{
	return instance->modified;
}

void PEditor::ScriptCreation::SetFileModified()
{
	instance->modified = true;
}

void PEditor::ScriptCreation::ResetModified()
{
	instance->modified = false;
}

std::vector<PEditor::ScriptCreationUtilities::ScriptNode*>& PEditor::ScriptCreation::GetNodes()
{
	return nodes;
}

void PEditor::ScriptCreation::render()
{
	ImGui::OpenPopup("Create script");
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));


	if (ImGui::BeginPopup("Create script", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		//ImGui::SetWindowFocus();


		auto background = ImGui::GetWindowDrawList();

		background->AddRectFilled(ImVec2(0, 0), ImGui::GetWindowSize(), ImColor(30, 30, 30, 255));



		if (lerping) {

			ManageLerp();
		}
		else {

			auto mouseScroll = ImGui::GetMouseDragDelta(2);

			scrollx = mouseScroll.x;
			scrolly = mouseScroll.y;

			if (scrollx != 0 || scrolly != 0)
				scrolled = true;

			if (ImGui::IsMouseReleased(2)) {

				xpos += scrollx;
				ypos += scrolly;

				scrollx = scrolly = 0;
				scrolled = true;
			}
		}

		ScriptCreationUtilities::Grid::SetOffset(xpos + scrollx, ypos + scrolly);
		ScriptCreationUtilities::Grid::SetColor(100, 100, 100, 255);
		ScriptCreationUtilities::Grid::Draw();

		int nodeIdx = 0;
		bool eraseNode = false;



		for (auto node : nodes) {

			if (node != nullptr) {

				if (node->UpdateAndRenderWindow()) {

					eraseNode = true;
				}

				if (eraseNode)
					node->SetID(node->GetId() - 1);
			}

			if (!eraseNode)
				nodeIdx++;
		}

		if (eraseNode)
		{
			scrolled = true;
			nodes[nodeIdx]->OnRemoved();
			delete nodes[nodeIdx];
			nodes.erase(nodes.begin() + nodeIdx);
		}


		dropDownSelection->UpdateAndRender();

		menuBar->UpdateAndRender();


		scrolled = false;


		if (ImGui::IsMouseReleased(0)) {

			ScriptCreationUtilities::ScriptMethod::currentlySelectedOutput = nullptr;
			ScriptCreationUtilities::ScriptFlow::currentSelectedFlow = nullptr;
		}

		ImGui::EndPopup();
	}


}

void PEditor::ScriptCreation::SetName(const std::string& name)
{
	menuBar->SetName(name);
}


void PEditor::ScriptCreation::RenderBox(const std::string& name, ImVec2 position, ImVec2 size)
{

	ImGui::SetCursorPos(position);

	if (ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_None))
	{

		ImGui::Text("Child Window Content Here");
	}
	ImGui::EndChild();

}


void PEditor::ScriptCreation::Lerp(int x, int y, float lerpDuration)
{
	instance->initialx = instance->xpos;
	instance->initialy = instance->ypos;

	instance->finalx = x;
	instance->finaly = y;

	instance->lerping = true;
	instance->lerpDuration = lerpDuration;

	instance->lerp_t = 0;

	instance->scrollx = instance->scrolly = 0;
}

void PEditor::ScriptCreation::ManageLerp()
{
	float dt = ImGui::GetIO().DeltaTime;

	lerp_t += dt;
	instance->scrolled = true;

	auto size = ImGui::GetWindowSize();

	float x = -finalx + size.x * 0.5f;
	float y = -finaly + size.y * 0.5f;



	if (lerp_t > lerpDuration) {

		lerping = false;
		xpos = x;
		ypos = y;

		return;
	}

	float t = lerp_t / lerpDuration;

	auto lerp = [](float a, float b, float t) {
		return a + t * (b - a);
	};

	auto cubicEaseOut = [](float x) {

		float inv = 1 - x;
		return 1 - (inv * inv * inv);
	};



	xpos = lerp(initialx, x, cubicEaseOut(t));
	ypos = lerp(initialy, y, cubicEaseOut(t));
}

void PEditor::ScriptCreation::ClearScript()
{
	scrollx = scrolly = 0;
	xpos = ypos = 0;

	ScriptCreationUtilities::Grid::ResetOffset();
	instance->modified = false;
	for (auto node : nodes) {
		delete node;
	}
	nodes.clear();
}
