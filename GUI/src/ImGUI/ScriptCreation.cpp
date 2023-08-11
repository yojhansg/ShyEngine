#include "ScriptCreation.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include <iostream>

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

void PEditor::ScriptCreation::AddNode(ScriptCreationUtilities::ScriptNode* node)
{
	node->SetID(nodes.size());
	nodes.push_back(node);
}

void PEditor::ScriptCreation::SetNodeCount(int count)
{
	nodes = std::vector<ScriptCreationUtilities::ScriptNode*>(count);
}

void PEditor::ScriptCreation::SetNode(int n, ScriptCreationUtilities::ScriptNode* node)
{
	nodes[n] = node;
}

void PEditor::ScriptCreation::Load()
{
	menuBar->Load();
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

		for (auto node : nodes) {

			if (node != nullptr)
				node->Render();
		}

		if (!ImGui::IsMouseDown(0))
			ScriptCreationUtilities::ScriptMethod::currentlySelected = nullptr;




		dropDownSelection->Render();

		menuBar->Render();

		scrolled = false;

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
	instance->modified = false;
	for (auto node : nodes) {
		delete node;
	}
	nodes.clear();
}
