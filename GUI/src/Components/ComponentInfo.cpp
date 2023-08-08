#include "ComponentInfo.h"
#include "nlohmann/json.hpp"

namespace Components {
	Component::Component()
	{
		name = "";
	}
	Component::Component(const Component& other)
	{
		name = other.name;

		methods = other.methods;
		attributes = other.attributes;
	}
	Components::Component::Component(cstring name)
	{
		this->name = name;
	}

	cstring Components::Component::getName()
	{
		return name;
	}

	Attribute& Components::Component::getAttribute(cstring name)
	{
		return attributes[name];
	}

	Method& Components::Component::getMethod(cstring name)
	{
		return methods[name];
	}

	std::unordered_map<std::string, Attribute>& Components::Component::getAllAttributes()
	{
		return attributes;
	}

	std::unordered_map<std::string, Method>& Components::Component::getAllMethods()
	{
		return methods;
	}

	void Components::Component::addAttribute(const Attribute& attribute)
	{
		attributes.emplace(attribute.getName(), attribute);
	}

	void Components::Component::addMethod(const Method& method)
	{
		methods.emplace(method.name, method);
	}

	Attribute::Attribute()
	{
		name = "";
		type = NONE;
	}

	Attribute::Attribute(std::string name, std::string typeString)
	{
		this->name = name;

		if (typeString == "int") {
			value.valueInt = 0;
			type = INT;
		}
		else if (typeString == "float") {
			value.valueFloat = 0.0f;
			type = FLOAT;
		}
		else if (typeString == "Utilities::Vector2D") {
			value.valueVector2 = { 0.0f, 0.0f };
			type = VECTOR2;
		}
		else if (typeString == "std::string") {
			valueString = "";
			type = STRING;
		}
		else if (typeString == "bool") {
			value.valueBool = false;
			type = BOOL;
		}
		/*else if (typeString == "int") {
			type = COLOR;
		}*/
		else {
			type = NONE;
		}
	}

	AttributesType Attribute::getType() const
	{
		return type;
	}

	std::string Attribute::getName() const {
		return name;
	}

	std::string Attribute::toJson()
	{
		nlohmann::ordered_json j;
		nlohmann::ordered_json vectorJson;

		switch (type)
		{
		case INT:
			j[name] = value.valueInt;
			break;
		case FLOAT:
			j[name] = value.valueFloat;
			break;
		case VECTOR2:
			vectorJson.push_back(value.valueVector2.x);
			vectorJson.push_back(value.valueVector2.y);

			j[name] = vectorJson;
			break;
		case STRING:
			j[name] = valueString;
			break;
		case BOOL:
			j[name] = value.valueBool;
			break;
		/*case COLOR:
		* j[name] = value.valueColor;
			break;*/
		default:
			break;
		}
		return j.dump(2);
	}

	std::string Component::toJson() {
		nlohmann::ordered_json j;

		j["component"] = name;

		nlohmann::ordered_json attributesJson;
		for (auto it = attributes.begin(); it != attributes.end(); it++) {
			attributesJson.push_back(j.parse(it->second.toJson()));
		}

		j["attributes"] = attributesJson;

		return j.dump(2);
	}
}
