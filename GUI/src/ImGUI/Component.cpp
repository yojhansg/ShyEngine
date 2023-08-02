#include "Component.h"
#include "imgui.h"

void Component::drawEditor()
{
    for (auto& attribute : attributes) {
        std::string attributeName = attribute.first;
		Attribute* attr = &attribute.second;

		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::Text(attributeName.c_str());

			switch (attr->getType())
			{
			case INT:
				drawInt(attributeName, attr);
				break;
			case FLOAT:
				drawFloat(attributeName, attr);
				break;
			case VECTOR2:
				drawVector2(attributeName, attr);
				break;
			case STRING:
				drawString(attributeName, attr);
				break;
			case BOOL:
				drawBool(attributeName, attr);
				break;
				/*	case COLOR:
						break;*/
			default:
				break;
			}
		}
    }
}

std::string Component::toJson()
{
	return std::string();
}

void Component::drawInt(std::string attrName, Attribute* attr)
{
	ImGui::DragInt(("##" + attrName).c_str(), &attr->value.valueInt, 0.3f, 0.0f, 0.0f, "%.2f");
}

void Component::drawFloat(std::string attrName, Attribute* attr)
{
	ImGui::DragFloat(("##" + attrName).c_str(), &attr->value.valueFloat, 0.3f, 0.0f, 0.0f, "%.2f");
}

void Component::drawVector2(std::string attrName, Attribute* attr)
{
	ImGui::DragFloat2(("##" + attrName).c_str(), (float*) attr->value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
}

void Component::drawString(std::string attrName, Attribute* attr)
{
	char inputBuffer[256];
	strncpy_s(inputBuffer, attr->valueString.c_str(), sizeof(inputBuffer));

	if (ImGui::InputText(("##" + attr->getName()).c_str(), inputBuffer, sizeof(inputBuffer))) {
		attr->valueString = inputBuffer;
	}
}

void Component::drawBool(std::string attrName, Attribute* attr)
{
	ImGui::Checkbox(("##" + attrName).c_str(), &attr->value.valueBool);
}

Attribute::Attribute(std::string name, AttributesType type)
{
	this->name = name;
	this->type = type;
}

AttributesType Attribute::getType()
{
	return type;
}

std::string Attribute::getName()
{
	return name;
}
