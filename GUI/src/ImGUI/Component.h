#pragma once
#include <iostream>
#include "imgui.h"

enum AttributesType {
	INT,
	FLOAT,
	VECTOR2,
	STRING,
	BOOL,
	//COLOR,
};

class Attribute {
	AttributesType type;
	std::string name;

	union value
	{
		int valueInt;
		float valueFloat;
		ImVec2 valueVector2;
		char* valueString;
		bool valueBool;
		//color
	};

};

class Component
{
public:
	std::string name;

	std::unordered_map<std::string, Attribute> attributes;

	virtual void drawEditor();
	virtual std::string toJson();
};

