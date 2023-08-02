#pragma once
#include <iostream>
#include <unordered_map>

struct ImVec2;

enum AttributesType {
	INT,
	FLOAT,
	VECTOR2,
	STRING,
	BOOL,
	//COLOR,
};

class Attribute {

private:
	AttributesType type;
	std::string name;

public:
	Attribute(std::string name, AttributesType type);

	AttributesType getType();
	std::string getName();

	union value
	{
		int valueInt = 0;
		float valueFloat;
		ImVec2* valueVector2;
		bool valueBool;
		//color
	} value;

	std::string valueString;
};

class Component
{
public:
	std::string name;

	std::unordered_map<std::string, Attribute> attributes;

	virtual void drawEditor();
	virtual std::string toJson();


private:

	void drawInt(std::string attrName, Attribute* attr);
	void drawFloat(std::string attrName, Attribute* attr);
	void drawVector2(std::string attrName, Attribute* attr);
	void drawString(std::string attrName, Attribute* attr);
	void drawBool(std::string attrName, Attribute* attr);
	//void drawColor(std::string attrName, Attribute* attr);
};

