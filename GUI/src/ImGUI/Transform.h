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
			float rotation = 0;
			ImGui::Text("Position");
			ImGui::DragFloat2("##position", (float*)getPosition(), 0.1f);

			ImGui::Text("Rotation");
			ImGui::DragFloat2("##rotation", &rotation, 0.1f);

			ImGui::Text("Scale");
			ImGui::DragFloat2("##scale", (float*)getSize(), 0.1f);
		}
	}
};

