#include "ScriptCreationUtilities.h"

#include "ComponentManager.h"
#include "nlohmann/json.hpp"
#include "ScriptCreation.h"
#include "ColorPalette.h"
#include "Console.h"
#include "Editor.h"
#include "imgui.h"

#include <algorithm>

using nlohmann::json;

namespace ShyEditor {

	void ScriptCreationUtilities::ScriptNode::RemoveInput(ScriptNode* node) {}

	ScriptCreationUtilities::ScriptNode::ScriptNode()
	{
		resizable = false;
		id = x = y = 0;
		w = 200;
		h = 300;
		outputButtonSize = 15;

		ignoreOutput = false;
		initialised = false;
	}

	ScriptCreationUtilities::ScriptNode* ScriptCreationUtilities::ScriptNode::SetID(int id)
	{
		this->id = id;


		return this;
	}

	bool ScriptCreationUtilities::ScriptNode::UpdateAndRenderWindow()
	{
		int scrollx, scrolly;
		ScriptCreation::GetScrollPosition(&scrollx, &scrolly);

		auto position = ImVec2(x + scrollx, y + scrolly);
		auto size = ImVec2(w, h);

		auto windowSize = ImGui::GetWindowSize();


		if (
			position.x <= windowSize.x && position.y <= windowSize.y &&
			position.x + size.x >= 0 && position.y + size.y >= 0
			)
		{

			if (!initialised || ScriptCreation::ScrolledThisFrame())
			{
				initialised = true;
				ImGui::SetNextWindowPos(position, ImGuiCond_None);
			}
			else
				ImGui::SetNextWindowPos(position, ImGuiCond_Once);

			int flag = ImGuiWindowFlags_NoSavedSettings;
			if (!resizable)
			{
				ImGui::SetNextWindowSize(size, ImGuiCond_None);
				flag |= ImGuiWindowFlags_NoResize;
			}


			ImGui::Begin(GetStringId().c_str(), NULL, flag);

			updateAndRender();
			ManageOutputNode();

			bool close = ManageCloseNode();

			if (!close && ImGui::IsWindowFocused() && ImGui::GetIO().KeysData[ImGuiKey_Delete].DownDuration == 0)
			{
				close = true;
			}

			UpdatePositionAfterDrag(scrollx, scrolly);

			if (resizable) {
				auto winSize = ImGui::GetWindowSize();
				w = winSize.x;
				h = winSize.y;
			}
			ImGui::End();

			return close;

		}

		return false;
	}

	void ScriptCreationUtilities::ScriptNode::UpdatePositionAfterDrag(int scrollx, int scrolly)
	{
		auto windowPos = ImGui::GetWindowPos();
		x = windowPos.x - scrollx;
		y = windowPos.y - scrolly;
	}

	void ScriptCreationUtilities::ScriptNode::ManageOutputNode()
	{
		if (ignoreOutput) return;

		auto drawList = ImGui::GetWindowDrawList();

		auto windowSize = ImGui::GetWindowSize();
		auto windowPosition = ImGui::GetWindowPos();

		auto outputNodePosition = ImVec2(0, 0);
		GetOutputNodePosition(&outputNodePosition.x, &outputNodePosition.y);

		auto outputNodePositionTop = ImVec2(outputNodePosition.x - outputButtonSize, outputNodePosition.y - outputButtonSize);
		auto outputNodePositionBottom = ImVec2(outputNodePosition.x - outputButtonSize, outputNodePosition.y + outputButtonSize);


		ImGui::SetCursorPos(ImVec2(outputNodePositionTop.x - windowPosition.x, outputNodePositionTop.y - windowPosition.y));
		ImGui::BeginChild((GetStringId() + "output node").c_str(), ImVec2(outputButtonSize, outputButtonSize * 2), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


		auto& palette = ColorPalette::GetCurrentPalette();
		ImColor nodeColor = ColorPaletteAlpha2ImColor(palette.line, palette.lineAlpha);
		ImColor hoverColor = ColorPaletteAlpha2ImColor(palette.hover, palette.lineAlpha);

		bool isCurrentlySelected = currentlySelectedOutput == this;

		if (ImGui::IsMouseHoveringRect(outputNodePositionTop, ImVec2(outputNodePosition.x, outputNodePositionBottom.y), true))
		{
			if (ImGui::IsMouseClicked(0) || isCurrentlySelected)
			{
				currentlySelectedOutput = this;
				drawList->AddTriangleFilled(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, nodeColor);
			}

			else
				drawList->AddTriangle(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, hoverColor, palette.buttonThickness);



			ImGui::SetTooltip(outputStr.c_str());

		}
		else {


			if (isCurrentlySelected || outputConexions.size() > 0)
				drawList->AddTriangleFilled(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, nodeColor);
			else
				drawList->AddTriangle(outputNodePosition, outputNodePositionTop, outputNodePositionBottom, nodeColor, palette.buttonThickness);

		}

		ImGui::EndChild();



		if (isCurrentlySelected) {

			auto mousePos = ImGui::GetMousePos();
			Bezier::Draw(outputNodePosition.x, outputNodePosition.y, mousePos.x, mousePos.y);
		}
	}

	bool ScriptCreationUtilities::ScriptNode::ManageCloseNode()
	{

		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 buttonSize = ImVec2(200, 30);
		ImGui::SetCursorPos(ImVec2((w - buttonSize.x) * 0.5f, h - buttonSize.y - 10));


		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);
		if (ImGui::Button("Close", buttonSize))
		{
			ImGui::PopStyleVar();
			return true;
		}
		ImGui::PopStyleVar();
		return false;
	}

	void ScriptCreationUtilities::ScriptNode::AddOutput(ScriptNode* output)
	{
		outputConexions.push_back(output);
	}

	bool ScriptCreationUtilities::ScriptNode::RemoveOutput(ScriptNode* output)
	{
		auto pos = std::remove(outputConexions.begin(), outputConexions.end(), output);

		if (pos == outputConexions.end())
			return false;

		outputConexions.erase(pos, outputConexions.end());
		return true;
	}

	void ScriptCreationUtilities::ScriptNode::OnRemoved()
	{
		for (auto output : outputConexions) {

			output->RemoveInput(this);
		}
	}

	void ScriptCreationUtilities::ScriptNode::GetOutputNodePosition(float* x, float* y)
	{
		const auto outputOffset = 15;

		int scroll_x, scroll_y;
		ScriptCreation::GetScrollPosition(&scroll_x, &scroll_y);

		if (x != nullptr)
			*x = scroll_x + w - outputOffset + this->x;
		if (y != nullptr)
			*y = scroll_y + h * 0.5f + this->y;

	}


	void ScriptCreationUtilities::ScriptFlow::GetNextNodePosition(float* x, float* y)
	{
		const auto outputOffset = 10;

		int scroll_x, scroll_y;
		ScriptCreation::GetScrollPosition(&scroll_x, &scroll_y);

		if (x != nullptr)
			*x = scroll_x + node->GetW() - outputOffset + node->GetX() + xoffset;
		if (y != nullptr)
			*y = scroll_y + node->GetH() - 40 - outputOffset - flowNodeSize + node->GetY() + yoffset;

	}

	void ScriptCreationUtilities::ScriptFlow::GetPreviousNodePosition(float* x, float* y)
	{
		const auto outputOffset = 10;

		int scroll_x, scroll_y;
		ScriptCreation::GetScrollPosition(&scroll_x, &scroll_y);

		if (x != nullptr)
			*x = scroll_x + outputOffset + node->GetX();
		if (y != nullptr)
			*y = scroll_y + node->GetH() - 40 - outputOffset - flowNodeSize + node->GetY() + yoffset;

	}

	void ScriptCreationUtilities::ScriptFlow::OnRemoved()
	{
		RemoveNext();

		for (auto p : previous)
		{
			p->next = nullptr;
		}
	}

	void ScriptCreationUtilities::ScriptFlow::RemoveNext()
	{
		if (next != nullptr)
		{
			next->RemovePrevious(this);
			next = nullptr;
		}
	}


	float ScriptCreationUtilities::ScriptFlow::GetButtonSize()
	{
		return flowNodeSize;
	}

	void ScriptCreationUtilities::ScriptFlow::SetOffset(int x, int y)
	{
		xoffset = x;
		yoffset = y;
	}

	ScriptCreationUtilities::ScriptNode*ScriptCreationUtilities::ScriptFlow::GetNext()
	{
		if (next == nullptr) return nullptr;

		return next->node;
	}

	void ScriptCreationUtilities::ScriptFlow::SetNext(ScriptFlow* next)
	{
		this->next = next;
		next->AddPrevious(this);
	}


	int ScriptCreationUtilities::ScriptNode::GetId()
	{
		return id;
	}

	int ScriptCreationUtilities::ScriptNode::GetY()
	{
		return y;
	}

	int ScriptCreationUtilities::ScriptNode::GetX()
	{
		return x;
	}

	int ScriptCreationUtilities::ScriptNode::GetW()
	{
		return w;
	}

	int ScriptCreationUtilities::ScriptNode::GetH()
	{
		return h;
	}

	std::string ScriptCreationUtilities::ScriptNode::GetOutputTypeString()
	{
		return outputStr;
	}

	ScriptCreationUtilities::ScriptNode* ScriptCreationUtilities::ScriptNode::currentlySelectedOutput = nullptr;

	ScriptCreationUtilities::ScriptNode* ScriptCreationUtilities::ScriptNode::SetPosition(int x1, int y1)
	{
		this->x = x1;
		this->y = y1;

		return this;
	}

	std::string ScriptCreationUtilities::ScriptNode::GetStringId()
	{
		return std::to_string(id);
	}

	ScriptCreationUtilities::ScriptFlow* ScriptCreationUtilities::ScriptNode::GetScriptFlow()
	{
		return nullptr;
	}

	void ScriptCreationUtilities::ScriptNode::updateAndRender() {}

	ScriptCreationUtilities::ScriptDropdownSelection::ScriptDropdownSelection(ScriptCreation* creator) : creator(creator)
	{
		mousex = mousey = 0;
	}

	void ScriptCreationUtilities::ScriptDropdownSelection::UpdateAndRender()
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

	void ScriptCreationUtilities::ScriptDropdownSelection::AddValuesFromVector(std::unordered_map<std::string, Components::Component>& v)
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

	ScriptCreationUtilities::ScriptFlow* ScriptCreationUtilities::ScriptFlow::currentSelectedFlow = nullptr;

	ScriptCreationUtilities::ScriptFlow::ScriptFlow(ScriptNode* node, bool onlyOutput) : node(node), onlyOutput(onlyOutput)
	{
		next = nullptr;
		flowNodeSize = 10;

		xoffset = yoffset = 0;
	}

	void ScriptCreationUtilities::ScriptFlow::AddPrevious(ScriptFlow* pre)
	{
		previous.push_back(pre);
	}

	bool ScriptCreationUtilities::ScriptFlow::RemovePrevious(ScriptFlow* pre)
	{
		auto pos = std::remove(previous.begin(), previous.end(), pre);

		if (pos == previous.end())
			return false;

		previous.erase(pos, previous.end());
		return true;

	}

	void ScriptCreationUtilities::ScriptFlow::ManageNextNode(float x, float y, const std::string& tooltip)
	{
		auto& palette = ColorPalette::GetCurrentPalette();

		Bezier::SetColor(ColorPaletteParams(palette.flowline));
		Bezier::SetThickness(palette.flowlineThickness);
		Bezier::SetCurvature(palette.flowlineCurvature);

		Bezier::SetOutlineThickness(palette.flowlineOutlineThickness);
		Bezier::SetOutlineColor(ColorPaletteParams(palette.flowlineOutline));

		Bezier::SetAlpha(palette.flowlineAlpha);

		ImColor nodeColor = ColorPaletteAlpha2ImColor(palette.flowline, palette.flowlineAlpha);
		ImColor hoverColor = ColorPaletteAlpha2ImColor(palette.hover, palette.flowlineAlpha);

		auto drawList = ImGui::GetWindowDrawList();

		auto windowSize = ImGui::GetWindowSize();
		auto windowPosition = ImGui::GetWindowPos();

		auto nextNodePosition = ImVec2(x, y);

		float nodeSize = flowNodeSize;

		auto nextNodePositionTop = ImVec2(nextNodePosition.x - nodeSize, nextNodePosition.y - nodeSize);
		auto nextNodePositionBottom = ImVec2(nextNodePosition.x - nodeSize, nextNodePosition.y + nodeSize);
		auto nextNodePositionLeft = ImVec2(nextNodePosition.x - nodeSize * 2, nextNodePosition.y);


		ImGui::SetCursorPos(ImVec2(nextNodePositionLeft.x - windowPosition.x, nextNodePositionTop.y - windowPosition.y));
		ImGui::BeginChild((node->GetStringId() + "next node" + std::to_string(y)).c_str(), ImVec2(nodeSize * 2, nodeSize * 2), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


		bool isCurrentlySelected = currentSelectedFlow == this;

		if (ImGui::IsMouseHoveringRect(ImVec2(nextNodePositionLeft.x, nextNodePositionTop.y), ImVec2(nextNodePosition.x, nextNodePositionBottom.y), true))
		{
			if (ImGui::IsMouseClicked(0) || isCurrentlySelected)
			{
				currentSelectedFlow = this;
				drawList->AddQuadFilled(nextNodePosition, nextNodePositionBottom, nextNodePositionLeft, nextNodePositionTop, nodeColor);
			}
			else
				drawList->AddQuad(nextNodePosition, nextNodePositionBottom, nextNodePositionLeft, nextNodePositionTop, hoverColor, palette.buttonThickness);


			if (tooltip != "") {
				ImGui::SetTooltip(tooltip.c_str());
			}
		}
		else {

			if (isCurrentlySelected || next != nullptr)
				drawList->AddQuadFilled(nextNodePosition, nextNodePositionBottom, nextNodePositionLeft, nextNodePositionTop, nodeColor);

			else

				drawList->AddQuad(nextNodePosition, nextNodePositionBottom, nextNodePositionLeft, nextNodePositionTop, nodeColor, palette.buttonThickness);
		}

		ImGui::EndChild();



		if (isCurrentlySelected) {

			auto mousePos = ImGui::GetMousePos();
			Bezier::Draw(nextNodePosition.x, nextNodePosition.y, mousePos.x, mousePos.y);
		}

		else if (currentSelectedFlow != nullptr && !onlyOutput) {


			ImVec2 bottomRightCorner = ImVec2(windowPosition.x + windowSize.x, windowPosition.y + windowSize.y);
			if (ImGui::IsMouseHoveringRect(windowPosition, bottomRightCorner)) {

				drawList->AddRectFilled(windowPosition, bottomRightCorner, hoverColor);

				if (ImGui::IsMouseReleased(0)) {

					currentSelectedFlow->RemoveNext();
					currentSelectedFlow->next = this;
					this->AddPrevious(currentSelectedFlow);
					currentSelectedFlow = nullptr;
				}
			}
		}


		if (next != nullptr) {

			auto otherpos = ImVec2(0, 0);
			next->GetPreviousNodePosition(&otherpos.x, &otherpos.y);
			Bezier::Draw(nextNodePosition.x, nextNodePosition.y, otherpos.x, otherpos.y);
		}



		if (previous.size() > 0) {


			ImVec2 previousPos = ImVec2();
			GetPreviousNodePosition(&previousPos.x, &previousPos.y);

			ImVec2 previousPosB = ImVec2(previousPos.x + nodeSize, previousPos.y - nodeSize);
			ImVec2 previousPosC = ImVec2(previousPos.x + nodeSize + nodeSize, previousPos.y);
			ImVec2 previousPosD = ImVec2(previousPos.x + nodeSize, previousPos.y + nodeSize);

			drawList->AddQuadFilled(previousPos, previousPosB, previousPosC, previousPosD, nodeColor);
		}





		Bezier::SetColor(ColorPaletteParams(palette.line));
		Bezier::SetThickness(palette.lineThickness);
		Bezier::SetCurvature(palette.lineCurvature);

		Bezier::SetOutlineThickness(palette.lineOutlineThickness);
		Bezier::SetOutlineColor(ColorPaletteParams(palette.lineOutline));
		Bezier::SetAlpha(palette.lineAlpha);

	}


	ScriptCreationUtilities::ScriptMethod::ScriptMethod(::Components::Method& method) : method(method)
	{
		input = std::vector<ScriptNode*>(method.getInput().size());
		type = Node::Method;

		outputStr = method.getReturn().getTypeStr();

		if (outputStr == "void") {
			ignoreOutput = true;
		}

		flow = new ScriptFlow(this);
	}


	void ScriptCreationUtilities::ScriptMethod::updateAndRender()
	{
		auto drawList = ImGui::GetWindowDrawList();

		auto& palette = ColorPalette::GetCurrentPalette();
		ImColor nodeColor = ColorPaletteAlpha2ImColor(palette.line, palette.lineAlpha);
		ImColor hoverColor = ColorPaletteAlpha2ImColor(palette.hover, palette.lineAlpha);

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


			auto a = ImVec2(min.x - 10, min.y + 3);
			auto b = ImVec2(min.x - 10, min.y + 13);
			auto c = ImVec2(min.x - 15, min.y + 8);


			if (ImGui::IsMouseHoveringRect(ImVec2(c.x, a.y), b) && ImGui::IsMouseDoubleClicked(0)) {

				input[idx] = nullptr;
			}

			if (ScriptNode::currentlySelectedOutput != nullptr &&
				ScriptNode::currentlySelectedOutput != this &&
				ImGui::IsMouseHoveringRect(ImVec2(c.x, a.y), b) &&
				(
					in.getTypeStr() == ScriptNode::currentlySelectedOutput->GetOutputTypeString() ||
					in.getTypeStr() == "any"
					)

				) {

				if (ImGui::IsMouseReleased(0)) {


					input[idx] = ScriptNode::currentlySelectedOutput;
					ScriptNode::currentlySelectedOutput->AddOutput(this);
					ScriptNode::currentlySelectedOutput = nullptr;
				}

				drawList->AddTriangleFilled(a, b, c, hoverColor);
			}
			else

				if (input[idx] != nullptr)
					drawList->AddTriangleFilled(a, b, c, nodeColor);
				else
					drawList->AddTriangle(a, b, c, nodeColor, palette.buttonThickness);



			if (input[idx] != nullptr) {

				float output_x, output_y;
				input[idx]->GetOutputNodePosition(&output_x, &output_y);
				Bezier::Draw(output_x, output_y, c.x, c.y);
			}

			ImGui::Spacing();
			idx++;
		}


		float x, y;
		flow->GetNextNodePosition(&x, &y);
		flow->ManageNextNode(x, y);
	}

	std::string ScriptCreationUtilities::ScriptMethod::GetStringId()
	{
		return method.getName() + " - " + method.getComponent() + " (" + std::to_string(id) + ")";
	}




	int ScriptCreationUtilities::Bezier::pointCount = 50;
	float ScriptCreationUtilities::Bezier::thickness = 1;

	float ScriptCreationUtilities::Bezier::r = 1;
	float ScriptCreationUtilities::Bezier::g = 1;
	float ScriptCreationUtilities::Bezier::b = 1;
	float ScriptCreationUtilities::Bezier::a = 1;

	float ScriptCreationUtilities::Bezier::r_outline = 1;
	float ScriptCreationUtilities::Bezier::g_outline = 1;
	float ScriptCreationUtilities::Bezier::b_outline = 1;

	float ScriptCreationUtilities::Bezier::curvature = .5f;
	float ScriptCreationUtilities::Bezier::outlineThickness = 0;


	void ScriptCreationUtilities::Bezier::ResetThickness()
	{
		thickness = 1;
	}

	void ScriptCreationUtilities::Bezier::SetThickness(float t)
	{
		thickness = t;
	}

	void ScriptCreationUtilities::Bezier::SetPointCount(int c)
	{
		pointCount = c;
	}

	void ScriptCreationUtilities::Bezier::ResetPointCount()
	{
		pointCount = 50;
	}

	void ScriptCreationUtilities::Bezier::SetColor(float r, float g, float b)
	{
		Bezier::r = r;
		Bezier::g = g;
		Bezier::b = b;
	}

	void ScriptCreationUtilities::Bezier::ResetColor()
	{
		Bezier::r = 1;
		Bezier::g = 1;
		Bezier::b = 1;
	}

	void ScriptCreationUtilities::Bezier::SetOutlineColor(float r, float g, float b)
	{
		Bezier::r_outline = r;
		Bezier::g_outline = g;
		Bezier::b_outline = b;
	}

	void ScriptCreationUtilities::Bezier::ResetOutlineColor()
	{
		Bezier::r_outline = 1;
		Bezier::g_outline = 1;
		Bezier::b_outline = 1;
	}

	void ScriptCreationUtilities::Bezier::SetOutlineThickness(float thickness)
	{
		Bezier::outlineThickness = thickness;
	}

	void ScriptCreationUtilities::Bezier::ResetOutlineThickness()
	{
		Bezier::outlineThickness = 0;
	}

	void ScriptCreationUtilities::Bezier::SetAlpha(float a)
	{
		Bezier::a = a;
	}

	void ScriptCreationUtilities::Bezier::ResetAlpha()
	{
		Bezier::a = 1;
	}

	void ScriptCreationUtilities::Bezier::SetCurvature(float curv)
	{
		Bezier::curvature = curv;
	}

	void ScriptCreationUtilities::Bezier::ResetCurvature()
	{
		Bezier::curvature = 0.5f;
	}

	void ScriptCreationUtilities::Bezier::Draw(int x, int y, int x1, int y1)
	{
		auto drawList = ImGui::GetForegroundDrawList();

		ImVec2 a = ImVec2(x, y);
		ImVec2 d = ImVec2(x1, y1);

		float x_distance = d.x - a.x;

		float x_increment = std::abs(x_distance * curvature);

		ImVec2 b = ImVec2(x + x_increment, y);
		ImVec2 c = ImVec2(x1 - x_increment, y1);

		if (outlineThickness > 0)
			drawList->AddBezierCubic(a, b, c, d,
				RGBA2ImColor(Bezier::r_outline, Bezier::g_outline, Bezier::b_outline, Bezier::a), thickness + outlineThickness * 2, pointCount);

		drawList->AddBezierCubic(a, b, c, d, RGBA2ImColor(Bezier::r, Bezier::g, Bezier::b, Bezier::a), thickness, pointCount);
	}




	int ScriptCreationUtilities::Grid::interval = 5;
	float ScriptCreationUtilities::Grid::thickness = 1;
	float ScriptCreationUtilities::Grid::intervalScale = 4;
	int ScriptCreationUtilities::Grid::spacing = 50;
	int ScriptCreationUtilities::Grid::x_offset = 0;
	int ScriptCreationUtilities::Grid::y_offset = 0;

	float ScriptCreationUtilities::Grid::r = 1;
	float ScriptCreationUtilities::Grid::g = 1;
	float ScriptCreationUtilities::Grid::b = 1;
	float ScriptCreationUtilities::Grid::a = 1;



	void ScriptCreationUtilities::Grid::SetSpacing(int spacing)
	{
		Grid::spacing = spacing;
	}

	void ScriptCreationUtilities::Grid::ResetSpacing()
	{
		Grid::spacing = 50;
	}

	void ScriptCreationUtilities::Grid::SetOffset(int x, int y)
	{
		x_offset = x;
		y_offset = y;
	}

	void ScriptCreationUtilities::Grid::ResetOffset()
	{
		x_offset = y_offset = 0;
	}

	void ScriptCreationUtilities::Grid::SetInterval(int interval)
	{
		Grid::interval = interval;
	}

	void ScriptCreationUtilities::Grid::ResetInterval()
	{
		Grid::interval = 5;
	}

	void ScriptCreationUtilities::Grid::SetColor(float r, float g, float b)
	{
		Grid::r = r;
		Grid::g = g;
		Grid::b = b;
	}

	void ScriptCreationUtilities::Grid::ResetColor()
	{
		r = g = b = a = 255;
	}

	void ScriptCreationUtilities::Grid::SetAlpha(float a)
	{
		Grid::a = a;
	}

	void ScriptCreationUtilities::Grid::ResetAlpha()
	{
		Grid::a = 1;
	}

	void ScriptCreationUtilities::Grid::SetThickness(float thickness)
	{
		Grid::thickness = thickness;
	}

	void ScriptCreationUtilities::Grid::ResetThickness()
	{
		Grid::thickness = 1;
	}

	void ScriptCreationUtilities::Grid::SetIntervalScale(float scale)
	{
		Grid::intervalScale = scale;
	}

	void ScriptCreationUtilities::Grid::ResetIntevalScale()
	{
		Grid::intervalScale = 4;
	}

	void ScriptCreationUtilities::Grid::Draw()
	{
		auto drawList = ImGui::GetWindowDrawList();
		auto windowSize = ImGui::GetWindowSize();

		int interval_x = (-x_offset / spacing) % interval;
		int interval_y = (-y_offset / spacing) % interval;

		float x_off = x_offset % spacing;
		float y_off = y_offset % spacing;

		for (int row = y_off; row < windowSize.y; row += spacing) {

			float t = interval_y == 0 ? thickness * intervalScale : thickness;

			if (t > 0)
				drawList->AddLine(ImVec2(0, row), ImVec2(windowSize.x, row), RGBA2ImColor(r, g, b, a), t);

			interval_y = (interval_y + 1) % interval;
		}

		for (int col = x_off; col < windowSize.x; col += spacing) {

			float t = interval_x == 0 ? thickness * intervalScale : thickness;

			if (t > 0)
				drawList->AddLine(ImVec2(col, 0), ImVec2(col, windowSize.y), RGBA2ImColor(r, g, b, a), t);

			interval_x = (interval_x + 1) % interval;
		}


	}

	ScriptCreationUtilities::ScriptInput::ScriptInput(::Components::AttributesType type) : attrType(type)
	{
		serialized = false;
		alwaysSerialize = false;

		attrValue = ::Components::AttributeValue();

		attrValue.value.valueFloat = 0;

		this->type = Node::Input;

		switch (attrType) {

		case ::Components::AttributesType::NONE:
			outputStr = "";
			break;
		case ::Components::AttributesType::FLOAT:
			outputStr = "float";
			break;
		case ::Components::AttributesType::VECTOR2:
			outputStr = "Vector2D";
			break;
		case ::Components::AttributesType::STRING:
			outputStr = "string";
			break;
		case ::Components::AttributesType::BOOL:
			outputStr = "bool";
			break;
		case ::Components::AttributesType::COLOR:
			outputStr = "color";
			break;
		case ::Components::AttributesType::GAMEOBJECT:
			outputStr = "Entity";
			alwaysSerialize = true;
			break;
		}

	}

	ScriptCreationUtilities::ScriptNode::Node ScriptCreationUtilities::ScriptNode::GetType()
	{
		return type;
	}

}

namespace ShyEditor {

	void ScriptCreationUtilities::ScriptInput::updateAndRender()
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


		if (!alwaysSerialize) {
			ImGui::Checkbox("Serialize", &serialized);
		}

		if (alwaysSerialize || serialized) {

			ImGui::InputText("Name", serializedName, 256);
		}
	}

	std::string ScriptCreationUtilities::ScriptInput::GetStringId()
	{
		return "Input - " + outputStr + " (" + std::to_string(id) + ")";
	}




	void ScriptCreationUtilities::ScriptMenuBar::Close()
	{
		nameBuffer[0] = '\0';
		creator->ClearScript();
		Editor::getInstance()->changeEditorState(Editor::EDITOR_STATE::EDITOR_WINDOW);;
	}

	ScriptCreationUtilities::ScriptMenuBar::ScriptMenuBar(ScriptCreation* creator) : creator(creator)
	{
		showClosePopup = false;
	}

	void ScriptCreationUtilities::ScriptMenuBar::SetName(const std::string& name)
	{
		std::memcpy(nameBuffer, name.c_str(), 256);
	}

	char* ScriptCreationUtilities::ScriptMenuBar::GetName()
	{
		return nameBuffer;
	}

	void ScriptCreationUtilities::ScriptMenuBar::UpdateAndRender()
	{
		int scrollx, scrolly;
		ScriptCreation::GetScrollPosition(&scrollx, &scrolly);
		auto windowSize = ImGui::GetWindowSize();


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(20, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(1, 0));
		if (ImGui::BeginMenuBar()) {


			ImGui::Text("Name the script");

			ImGui::PushItemWidth(300);
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
				if (ImGui::MenuItem("Entity")) {

					type = ::Components::AttributesType::GAMEOBJECT;
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

				bool change = false;
				ScriptFork::Fork type;

				if (ImGui::MenuItem("If")) {

					type = ScriptFork::Fork::If;
					change = true;
				}

				if (ImGui::MenuItem("While")) {

					type = ScriptFork::Fork::While;
					change = true;
				}

				if (ImGui::MenuItem("Loop")) {

					type = ScriptFork::Fork::For;
					change = true;
				}

				if (change) {
					ScriptNode* node = new ScriptFork(type);

					node->SetPosition((windowSize.x - node->GetW()) * 0.5f - scrollx, (windowSize.y - node->GetH()) * 0.5f - scrolly);

					creator->AddNode(node);
					ScriptCreation::SetFileModified();
				}


				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Events")) {

				std::vector<std::string> events = {

					"start", "update", "init", "onCollisionEnter"
				};

				for (auto& name : events) {

					if (ImGui::MenuItem(ScriptEvent::StyleName(name).c_str())) {

						ScriptEvent* selectedEvent = creator->ContainsEvent(name);

						if (selectedEvent == nullptr) {
							ScriptEvent* node = new ScriptEvent(name);

							node->SetPosition((windowSize.x - node->GetW()) * 0.5f - scrollx, (windowSize.y - node->GetH()) * 0.5f - scrolly);

							creator->AddEvent(name, node);
							ScriptCreation::SetFileModified();
						}
						else
						{
							creator->Lerp(
								selectedEvent->GetX() + selectedEvent->GetW() * 0.5f,
								selectedEvent->GetY() + selectedEvent->GetH() * 0.5f,
								1);

						}
					}
				}

				ImGui::EndMenu();
			}



			if (ImGui::Button("Save script")) {

				Save();
			}

			if (ImGui::Button("Close")) {


				if (creator->IsFileModified())
					showClosePopup = true;
				else Close();
			}

			ImGui::PushItemWidth(300);
			if (ImGui::InputText("Node search", nameSearch, CharBufferSize)) {

				showNodeSearch = std::string(nameSearch).length() > 0;
			}

			if (showNodeSearch) {

				if (ImGui::BeginMenu("Node search result")) {

					ShowFoundMethods(Components::ComponentManager::GetAllComponents(), windowSize.x, windowSize.y);
					ShowFoundMethods(Components::ComponentManager::GetAllManagers(), windowSize.x, windowSize.y);

					ImGui::EndMenu();
				}
			}


			if (ImGui::BeginMenu("Find existing node")) {


				for (auto& node : creator->GetNodes()) {

					if (ImGui::MenuItem(node->GetStringId().c_str())) {

						creator->Lerp(node->GetX() + node->GetW() * 0.5f, node->GetY() + node->GetH() * 0.5f, 1);
					}
				}
				ImGui::EndMenu();
			}


			if (ImGui::Button("Add comment")) {


				creator->AddNode(new ScriptComment("Write comment here"));
			}


			if (ImGui::Button("Open console")) {

				Editor::getInstance()->getConsole()->Show();
			}


			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(3);



		if (showClosePopup) {

			ImGui::OpenPopup("Close without saving");
			auto size = ImGui::GetWindowSize();

			auto drawList = ImGui::GetWindowDrawList();

			drawList->AddRectFilled(ImVec2(0, 0), windowSize, ColorPalette2ImColor(ColorPalette::GetCurrentPalette().scriptBackground), 0, 0);


			ImGui::SetNextWindowPos(ImVec2(size.x * 0.5f, size.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Centrar el pop-up

		}
		if (ImGui::BeginPopup("Close without saving")) {

			ImGui::Text("Esto es un mensaje emergente");
			bool closePopup = false;
			if (ImGui::Button("Cancel")) {
				closePopup = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("Close without saving")) {
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

				showClosePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}


	void ScriptCreationUtilities::ScriptMenuBar::ShowFoundMethods(std::unordered_map<std::string, Components::Component>& v, int windowW, int windowH)
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

	Components::Method& ScriptCreationUtilities::ScriptMenuBar::GetMethodReference(const std::string& name)
	{

		return Components::ComponentManager::GetAllComponents()["a"].getMethod("a");

	}


	void ScriptCreationUtilities::ScriptMenuBar::Save()
	{
		creator->Save();

	}


	json ScriptCreationUtilities::ScriptNode::ToJson()
	{
		json root;

		root["index"] = id;
		root["x"] = x;
		root["y"] = y;

		return root;
	}

	void ScriptCreationUtilities::ScriptNode::FromJson(nlohmann::json& root)
	{
		id = root["index"].get<int>();

		x = root.contains("x") ? root["x"].get<float>() : 0;
		y = root.contains("y") ? root["y"].get<float>() : 0;
	}



	nlohmann::json ScriptCreationUtilities::ScriptMethod::ToJson()
	{
		json root = ScriptNode::ToJson();

		if (method.getComponent() != method.getName())
			root["function"] = method.getComponent() + "_" + method.getName();
		else
			root["function"] = method.getComponent();

		root["input"] = json::array();

		for (auto& in : input) {

			if (in != nullptr)
				root["input"].push_back(in->GetId());
			else root["input"].push_back(-1);
		}

		if (flow->GetNext() != nullptr)
			root["next"] = flow->GetNext()->GetId();

		return root;
	}

	void ScriptCreationUtilities::ScriptMethod::SetInput(int idx, ScriptNode* node)
	{
		input[idx] = node;
		node->AddOutput(this);
	}

	void ScriptCreationUtilities::ScriptMethod::SetNext(ScriptFlow* flow)
	{
		this->flow->SetNext(flow);
	}


	void ScriptCreationUtilities::ScriptMethod::OnRemoved()
	{
		ScriptNode::OnRemoved();
		flow->OnRemoved();
		delete flow;
		flow = nullptr;

		for (auto in : input) {

			if (in != nullptr) {

				in->RemoveOutput(this);
			}
		}
	}

	void ScriptCreationUtilities::ScriptMethod::RemoveInput(ScriptNode* node)
	{
		for (int i = 0; i < input.size(); i++) {

			if (input[i] == node)
				input[i] = nullptr;
		}

	}

	ScriptCreationUtilities::ScriptFlow* ScriptCreationUtilities::ScriptMethod::GetScriptFlow()
	{
		return flow;
	}


	nlohmann::json ScriptCreationUtilities::ScriptInput::ToJson()
	{
		json root = ScriptNode::ToJson();

		//TODO: serialised values
		root["serialized"] = serialized;

		if (serialized)
			root["name"] = serializedName;


		if (attrType == Components::AttributesType::NONE) {
			root["type"] = "null";
			root["value"] = nullptr;
		}
		else if (attrType == Components::AttributesType::FLOAT) {
			root["type"] = "float";
			root["value"] = attrValue.value.valueFloat;
		}
		else if (attrType == Components::AttributesType::VECTOR2) {
			root["type"] = "Vector2D";
			root["value"] = std::to_string(attrValue.value.valueVector2.x) + "," + std::to_string(attrValue.value.valueVector2.y);
		}
		else if (attrType == Components::AttributesType::STRING) {
			root["type"] = "string";
			root["value"] = attrValue.valueString;
		}
		else if (attrType == Components::AttributesType::BOOL) {
			root["type"] = "bool";
			root["value"] = attrValue.value.valueBool;
		}
		else if (attrType == Components::AttributesType::COLOR) {
			root["type"] = "color";
			root["value"] = std::to_string(attrValue.value.valueColor.r) + "," + std::to_string(attrValue.value.valueColor.g) + ", " + std::to_string(attrValue.value.valueColor.b);
		}
		else if (attrType == Components::AttributesType::GAMEOBJECT) {
			root["type"] = "Entity";
			root["value"] = attrValue.value.entityIdx;
		}

		return root;
	}

	void ScriptCreationUtilities::ScriptInput::SetValue(::Components::AttributeValue const& val)
	{
		attrValue = val;
	}

	bool ScriptCreationUtilities::ScriptInput::IsSerialized()
	{
		return serialized || alwaysSerialize;
	}

	std::string ScriptCreationUtilities::ScriptInput::GetName()
	{
		return serializedName;
	}

	void ScriptCreationUtilities::ScriptInput::SetSerialized(bool value, const std::string& str)
	{
		serialized = value;
		std::memcpy(serializedName, str.c_str(), 256);
	}

	ScriptCreationUtilities::ScriptFork::ScriptFork(Fork type) : forkType(type)
	{
		condition = nullptr;

		ScriptNode::type = Node::Fork;

		ignoreOutput = true;

		A = new ScriptFlow(this, true);
		B = new ScriptFlow(this);

		A->SetOffset(0, -50);

		w = 100;
		h = 150;


		switch (type)
		{
		case ScriptCreationUtilities::ScriptFork::Fork::If:
			outputStr = "If";
			a_tooltip = "On true";
			b_tooltip = "On false";
			break;
		case ScriptCreationUtilities::ScriptFork::Fork::While:
			outputStr = "While";
			a_tooltip = "Node to loop";
			b_tooltip = "On Completed";
			break;
		case ScriptCreationUtilities::ScriptFork::Fork::For:
			outputStr = "Loop";
			a_tooltip = "Node to loop";
			b_tooltip = "On Completed";
			break;
		default:
			break;
		}

	}



	nlohmann::json ScriptCreationUtilities::ScriptFork::ToJson()
	{
		json root = ScriptNode::ToJson();

		root["type"] = (int)forkType;
		root["condition"] = condition != nullptr ? condition->GetId() : -1;
		root["A"] = A->GetNext() != nullptr ? A->GetNext()->GetId() : -1;
		root["B"] = B->GetNext() != nullptr ? B->GetNext()->GetId() : -1;

		return root;
	}

	void ScriptCreationUtilities::ScriptFork::OnRemoved()
	{
		ScriptNode::OnRemoved();

		A->OnRemoved();
		B->OnRemoved();

		delete A;
		delete B;

		A = B = nullptr;

		if (condition != nullptr)
			condition->RemoveOutput(this);
	}

	void ScriptCreationUtilities::ScriptFork::RemoveInput(ScriptNode* node)
	{
		if (condition == node)
			condition = nullptr;
	}

	void ScriptCreationUtilities::ScriptFork::SetCondition(ScriptNode* condition)
	{
		this->condition = condition;
		condition->AddOutput(this);
	}

	void ScriptCreationUtilities::ScriptFork::SetA(ScriptFlow* flow)
	{
		A->SetNext(flow);
	}

	void ScriptCreationUtilities::ScriptFork::SetB(ScriptFlow* flow)
	{
		B->SetNext(flow);
	}

	ScriptCreationUtilities::ScriptFlow* ScriptCreationUtilities::ScriptFork::GetScriptFlow()
	{
		return B;
	}

	void ScriptCreationUtilities::ScriptFork::updateAndRender()
	{
		float x, y;
		B->GetNextNodePosition(&x, &y);
		B->ManageNextNode(x, y, b_tooltip);

		A->GetNextNodePosition(&x, &y);
		A->ManageNextNode(x, y, a_tooltip);


		ImVec2 a = ImVec2(x, y);
		A->GetPreviousNodePosition(&a.x, NULL);

		//Size: 5x10
		int size = 5;

		auto b = ImVec2(a.x + size, a.y - size);
		auto c = ImVec2(a.x + size, a.y + size);

		bool drawInputFilled = condition != nullptr;

		if (ImGui::IsMouseHoveringRect(ImVec2(a.x, b.y), c)) {

			if (ImGui::IsMouseDoubleClicked(0)) {

				condition->RemoveOutput(this);
				condition = nullptr;
			}

			if (ScriptNode::currentlySelectedOutput != nullptr)
			{
				if (ImGui::IsMouseReleased(0))
				{
					condition = currentlySelectedOutput;
					ScriptNode::currentlySelectedOutput->AddOutput(this);
					currentlySelectedOutput = nullptr;
				}
				drawInputFilled = true;
			}
		}


		auto drawList = ImGui::GetWindowDrawList();

		auto& colorPalette = ColorPalette::GetCurrentPalette();
		ImColor nodeColor = ColorPaletteAlpha2ImColor(colorPalette.line, colorPalette.lineAlpha);

		if (drawInputFilled)
			drawList->AddTriangleFilled(a, b, c, nodeColor);
		else
			drawList->AddTriangle(a, b, c, nodeColor, colorPalette.buttonThickness);

		if (condition != nullptr) {

			float outx, outy;
			condition->GetOutputNodePosition(&outx, &outy);
			Bezier::Draw(outx, outy, a.x, a.y);
		}

	}



	std::string ScriptCreationUtilities::ScriptFork::GetStringId()
	{
		return outputStr + " (" + std::to_string(id) + ")";
	}



	ScriptCreationUtilities::ScriptEvent::ScriptEvent(const std::string& ev)
	{
		type = Node::Event;
		ignoreOutput = true;

		eventname = ev;

		if (eventname.size() > 0)
		{
			stylisedName = ScriptEvent::StyleName(eventname);
		}


		h = 100;

		flow = new ScriptFlow(this, true);
	}

	void ScriptCreationUtilities::ScriptEvent::updateAndRender()
	{
		float x, y;
		flow->GetNextNodePosition(&x, &y);
		flow->ManageNextNode(x, y);
	}

	std::string ScriptCreationUtilities::ScriptEvent::StyleName(const std::string& str)
	{
		std::string styled = std::string(1, (char)std::toupper(str[0]));

		for (int i = 1; i < str.size(); i++) {

			if (std::isupper(str[i])) {
				styled.push_back(' ');
			}

			styled.push_back(std::tolower(str[i]));
		}

		return styled;
	}

	void ScriptCreationUtilities::ScriptEvent::OnRemoved()
	{
		ScriptCreation::RemoveEvent(eventname);

		flow->RemoveNext();
		delete flow;
		flow = nullptr;
	}

	std::string ScriptCreationUtilities::ScriptEvent::GetStringId()
	{
		return stylisedName;
	}

	nlohmann::json ScriptCreationUtilities::ScriptEvent::ToJson()
	{
		json root = ScriptNode::ToJson();

		root["type"] = eventname;

		return root;
	}

	std::string ScriptCreationUtilities::ScriptEvent::GetEventName()
	{
		return eventname;
	}

	ScriptCreationUtilities::ScriptFlow* ScriptCreationUtilities::ScriptEvent::GetScriptFlow()
	{
		return flow;
	}

	ScriptCreationUtilities::ScriptComment::ScriptComment(const std::string commentStr)
	{
		type = Node::Comment;
		resizable = true;
		ignoreOutput = true;

		memcpy(comment, commentStr.c_str(), 256);
	}

	std::string ScriptCreationUtilities::ScriptComment::GetStringId()
	{
		return "Comment";
	}

	void ScriptCreationUtilities::ScriptComment::updateAndRender()
	{
		const int margin = 10;

		float encabezado = ImGui::GetFrameHeight();

		ImVec2 textSize = ImVec2(w - 2 * margin, h - 50 - margin - encabezado);
		ImGui::SetCursorPos(ImVec2(margin, margin + encabezado));
		ImGui::InputTextMultiline("##",
			comment, 256, textSize,
			ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll
		);
	}

	void ScriptCreationUtilities::ScriptComment::SetSize(float w, float h)
	{
		this->w = w;
		this->h = h;
	}

	nlohmann::json ScriptCreationUtilities::ScriptComment::ToJson()
	{
		json root = ScriptNode::ToJson();

		root["comment"] = comment;
		root["w"] = w;
		root["h"] = h;

		return root;
	}

}