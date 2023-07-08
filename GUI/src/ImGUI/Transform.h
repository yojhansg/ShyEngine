#pragma once
#include "Component.h"
#include "imgui.h"

class Transform : public Component
{
public:
	Transform() {};
	ImVec2 size;
	ImVec2 pos;

	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
	}

	void setSize(float x, float y) {
		size.x = x;
		size.y = y;
	}

	ImVec2* getPosition() { return &pos; }
	ImVec2* getSize() { return &size; }

	virtual void drawEditor() {
		if (ImGui::CollapsingHeader("Transform"))
		{

			ImGui::Text("Position");
			ImGui::DragFloat2("##position_drag", (float*)getPosition(), 0.3f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Scale");
			ImGui::DragFloat2("##scale_drag", (float*)getSize(), 0.1f, 0.0f, 0.0f, "%.2f");
		}
	}
};

