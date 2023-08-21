#include "ScriptCreation.h"

#include "ScriptCreationUtilities.h"
#include "ComponentManager.h"
#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "ColorPalette.h"
#include "Editor.h"
#include "imgui.h"

#include <iostream>
#include <fstream>

using nlohmann::json;

namespace ShyEditor {

	ScriptCreation* ScriptCreation::instance = nullptr;

	ScriptCreation::ScriptCreation() : Window("", None)
	{
		editor = Editor::getInstance();

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

	ScriptCreation::~ScriptCreation()
	{
		ClearScript();

		instance = nullptr;
		delete dropDownSelection;
		delete menuBar;

	}

	ScriptCreationUtilities::ScriptEvent* ScriptCreation::ContainsEvent(const std::string& event)
	{
		if (events.contains(event))
			return events[event];

		return nullptr;
	}

	void ScriptCreation::AddEvent(const std::string& name, ScriptCreationUtilities::ScriptEvent* event)
	{
		events.emplace(name, event);

		AddNode(event);
	}

	void ScriptCreation::AddNode(ScriptCreationUtilities::ScriptNode* node)
	{
		node->SetID(nodes.size());
		nodes.push_back(node);
	}

	void ScriptCreation::RemoveEvent(const std::string& name)
	{
		instance->events.erase(name);
	}

	void ScriptCreation::SetNodeCount(int count)
	{
		nodes = std::vector<ScriptCreationUtilities::ScriptNode*>(count);
	}

	void ScriptCreation::SetNode(int n, ScriptCreationUtilities::ScriptNode* node)
	{
		nodes[n] = node;
	}

	void ScriptCreation::Save()
	{
		auto& allNodes = GetNodes();

		json root;
		json functions = json::array(), consts = json::array(), forks = json::array();


		json eventData = json::array(), comments = json::array();


		json serializedValues = json::array();

		for (auto node : allNodes) {

			switch (node->GetType())
			{

			case ScriptCreationUtilities::ScriptNode::Node::Method:
				functions.push_back(node->ToJson());
				break;
			case ScriptCreationUtilities::ScriptNode::Node::Input:
			{
				auto jsonNode = node->ToJson();

				ScriptCreationUtilities::ScriptInput* input =
					static_cast<ScriptCreationUtilities::ScriptInput*>(node);

				if (input->IsSerialized())
					serializedValues.push_back({

						{"type", input->GetOutputTypeString()},
						{"name", input->GetName()},
						{"defaultValue", jsonNode["value"]}
						});

				consts.push_back(jsonNode);

				break;
			}
			case ScriptCreationUtilities::ScriptNode::Node::Fork:
				forks.push_back(node->ToJson());
				break;
			case ScriptCreationUtilities::ScriptNode::Node::Event:
				eventData.push_back(node->ToJson());
				break;
			case ScriptCreationUtilities::ScriptNode::Node::Comment:
				comments.push_back(node->ToJson());
				break;

			default:
				break;
			}

		}

		root["nodeCount"] = allNodes.size();
		root["functions"] = functions;
		root["consts"] = consts;
		root["forks"] = forks;
		root["events"] = eventData;
		root["comments"] = comments;
		root["serializedValues"] = serializedValues;


		for (auto& event : events) {

			auto next = event.second->GetScriptFlow()->GetNext();
			if (next != nullptr)
				root[event.first] = next->GetId();
		}


		std::ofstream file(editor->getProjectInfo().path + "/Scripts/" + std::string(menuBar->GetName()) + ".script");

		file << root.dump(4);

		file.close();



		ScriptCreation::ResetModified();
		Components::ComponentManager::ReloadScripts();
	}

	void ScriptCreation::Load()
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

		std::ifstream fileStream(editor->getProjectInfo().path + "/Scripts/" + fileName + ".script");

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
		json& forks = file["forks"];
		json& comments = file["comments"];
		json& events = file["events"];

		SetNodeCount(file["nodeCount"].get<int>());

		struct MethodInput {
			class ScriptCreationUtilities::ScriptMethod* node;
			std::vector<int> input;

			int next;
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


			std::vector<int> input;
			for (auto& in : funcNode["input"]) {

				input.push_back(in.get<int>());

			}


			int next = -1;
			if (funcNode.contains("next"))
				next = funcNode["next"].get<int>();

			methodInfo.push_back({ method, input, next });


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
			else if (typeStr == "Vector2D") {
				//TODO: despues de la serializacion
			}
			else if (typeStr == "string") {
				type = ::Components::AttributesType::STRING;
				value.valueString = constNode["value"].get<std::string>();
			}
			else if (typeStr == "bool") {
				type = ::Components::AttributesType::BOOL;
				value.value.valueBool = constNode["value"].get<bool>();
			}
			else if (typeStr == "color") {

				//TODO: despues de la serializacion
			}
			else if (typeStr == "Entity") {
				type = ::Components::AttributesType::GAMEOBJECT;
				value.value.entityIdx = -1;
			}



			ScriptCreationUtilities::ScriptInput* input = new ScriptCreationUtilities::ScriptInput(type);
			input->SetValue(value);
			input->FromJson(constNode);

			if (constNode.contains("serialized")) {

				bool serialized = constNode["serialized"].get<bool>();
				if (serialized) {

					input->SetSerialized(serialized, constNode["name"].get<std::string>());
				}
			}


			SetNode(input->GetId(), input);
		}


		struct ForkData {
			class ScriptCreationUtilities::ScriptFork* fork;
			int condition;
			int A;
			int B;
		};

		std::vector<ForkData> forkData;


		for (auto& forkInfo : forks) {


			ScriptCreationUtilities::ScriptFork* fork = new ScriptCreationUtilities::ScriptFork(
				(ScriptCreationUtilities::ScriptFork::Fork)forkInfo["type"].get<int>()
			);

			fork->FromJson(forkInfo);

			forkData.push_back({
				fork, forkInfo["condition"].get<int>(),
				forkInfo["A"].get<int>(), forkInfo["B"].get<int>() });

			SetNode(fork->GetId(), fork);
		}


		for (auto& commentData : comments) {

			ScriptCreationUtilities::ScriptComment* comment = new ScriptCreationUtilities::ScriptComment(
				commentData["comment"].get<std::string>()
			);

			comment->FromJson(commentData);

			comment->SetSize(commentData["w"].get<float>(), commentData["h"].get<float>());

			SetNode(comment->GetId(), comment);
		}


		for (auto& eventData : events) {

			std::string type = eventData["type"].get<std::string>();
			ScriptCreationUtilities::ScriptEvent* event = new ScriptCreationUtilities::ScriptEvent(
				type
			);

			event->FromJson(eventData);

			if (file.contains(type)) {
				int idx = file[type].get<int>();
				event->GetScriptFlow()->SetNext(GetNodes()[idx]->GetScriptFlow());
			}
			SetNode(event->GetId(), event);
			this->events.emplace(type, event);
		}


		for (auto& mi : methodInfo) {

			int idx = 0;
			for (int i : mi.input) {

				if (i >= 0)
					mi.node->SetInput(idx, GetNodes()[i]);

				idx++;
			}

			if (mi.next >= 0)
				mi.node->SetNext(GetNodes()[mi.next]->GetScriptFlow());
		}


		for (auto& fd : forkData) {

			if (fd.condition >= 0)
				fd.fork->SetCondition(GetNodes()[fd.condition]);
			if (fd.A >= 0)
				fd.fork->SetA(GetNodes()[fd.A]->GetScriptFlow());
			if (fd.B >= 0)
				fd.fork->SetB(GetNodes()[fd.B]->GetScriptFlow());
		}
	}

	void ScriptCreation::GetScrollPosition(int* x, int* y)
	{
		if (x != nullptr)
			*x = instance->xpos + instance->scrollx;

		if (y != nullptr)
			*y = instance->ypos + instance->scrolly;
	}

	bool ScriptCreation::ScrolledThisFrame()
	{
		return instance->scrolled;
	}

	bool ScriptCreation::IsFileModified()
	{
		return instance->modified;
	}

	void ScriptCreation::SetFileModified()
	{
		instance->modified = true;
	}

	void ScriptCreation::ResetModified()
	{
		instance->modified = false;
	}

	std::vector<ScriptCreationUtilities::ScriptNode*>& ScriptCreation::GetNodes()
	{
		return nodes;
	}

	void ScriptCreation::Behaviour()
	{
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		if (ImGui::Begin("Create script", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{

			auto background = ImGui::GetWindowDrawList();

			auto& palette = ColorPalette::GetCurrentPalette();
			auto color = palette.scriptBackground;
			background->AddRectFilled(ImVec2(0, 0), ImGui::GetWindowSize(), ColorPalette2ImColor(color));

			auto lineColor = palette.line;
			ScriptCreationUtilities::Bezier::SetColor(ColorPaletteParams(lineColor));

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


			auto gridColor = palette.grid;

			ScriptCreationUtilities::Grid::SetOffset(xpos + scrollx, ypos + scrolly);
			ScriptCreationUtilities::Grid::SetThickness(palette.gridThickness);
			ScriptCreationUtilities::Grid::SetSpacing(palette.gridSpacing);
			ScriptCreationUtilities::Grid::SetIntervalScale(palette.gridIntervalScale);
			ScriptCreationUtilities::Grid::SetInterval(palette.gridInterval);
			ScriptCreationUtilities::Grid::SetColor(ColorPaletteParams(gridColor));
			ScriptCreationUtilities::Grid::Draw();

			ScriptCreationUtilities::Bezier::SetAlpha(palette.lineAlpha);
			ScriptCreationUtilities::Bezier::SetColor(ColorPaletteParams(palette.line));
			ScriptCreationUtilities::Bezier::SetThickness(palette.lineThickness);
			ScriptCreationUtilities::Bezier::SetCurvature(palette.lineCurvature);

			ScriptCreationUtilities::Bezier::SetOutlineThickness(palette.lineOutlineThickness);
			ScriptCreationUtilities::Bezier::SetOutlineColor(ColorPaletteParams(palette.lineOutline));


			int nodeIdx = 0;
			bool eraseNode = false;


			auto& style = ImGui::GetStyle();
			style.WindowRounding = palette.nodeRounding;
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
			style.WindowRounding = 0;

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

		}

		ImGui::End();

	}

	void ScriptCreation::SetName(const std::string& name)
	{
		menuBar->SetName(name);
	}


	void ScriptCreation::RenderBox(const std::string& name, ImVec2 position, ImVec2 size)
	{

		ImGui::SetCursorPos(position);

		if (ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_None))
		{

			ImGui::Text("Child Window Content Here");
		}
		ImGui::EndChild();

	}


	void ScriptCreation::Lerp(int x, int y, float lerpDuration)
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

	void ScriptCreation::ManageLerp()
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

	void ScriptCreation::ClearScript()
	{
		scrollx = scrolly = 0;
		xpos = ypos = 0;

		ScriptCreationUtilities::Grid::ResetOffset();
		instance->modified = false;
		for (auto node : nodes) {
			delete node;
		}
		nodes.clear();
		events.clear();
	}

}
