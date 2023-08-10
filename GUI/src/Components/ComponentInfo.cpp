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

	const Attribute& Components::Component::getAttribute(cstring name)
	{
		return attributes[name];
	}

	const Method& Components::Component::getMethod(cstring name)
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
		methods.emplace(method.getName(), method);
	}

	Attribute::Attribute()
	{
		name = "";
		type = AttributesType::NONE;
	}

	Attribute::Attribute(const std::string& name, const std::string& typeString)
	{
		typeStr = typeString;
		this->name = name;

		if (typeString == "int") {
			value.value.valueInt = 0;
			type = AttributesType::INT;
		}
		else if (typeString == "float") {
			value.value.valueFloat = 0.0f;
			type = AttributesType::FLOAT;
		}
		else if (typeString == "Utilities::Vector2D") {
			value.value.valueVector2 = { 0.0f, 0.0f };
			type = AttributesType::VECTOR2;
		}
		else if (typeString == "std::string") {
			value.valueString = "";
			type = AttributesType::STRING;
		}
		else if (typeString == "bool") {
			value.value.valueBool = false;
			type = AttributesType::BOOL;
		}
		else if (typeString == "Utilities::Color") {
			value.value.valueColor = { 0.0f, 0.0f, 0.0f };
			type = AttributesType::COLOR;
		}
		else {
			type = AttributesType::NONE;
		}
	}

	AttributesType Attribute::getType() const
	{
		return type;
	}

	std::string Attribute::getName() const {
		return name;
	}

	std::string Attribute::getTypeStr() const
	{
		return typeStr;
	}

	std::string Attribute::toJson()
	{
		nlohmann::ordered_json j;
		nlohmann::ordered_json vectorJson;
		nlohmann::ordered_json colorJson;

		switch (type)
		{
		case AttributesType::INT:
			j[name] = value.value.valueInt;
			break;
		case AttributesType::FLOAT:
			j[name] = value.value.valueFloat;
			break;
		case AttributesType::VECTOR2:
			vectorJson.push_back(value.value.valueVector2.x);
			vectorJson.push_back(value.value.valueVector2.y);

			j[name] = vectorJson;
			break;
		case AttributesType::STRING:
			j[name] = value.valueString;
			break;
		case AttributesType::BOOL:
			j[name] = value.value.valueBool;
			break;
		case AttributesType::COLOR:
			colorJson.push_back(value.value.valueColor.r);
			colorJson.push_back(value.value.valueColor.g);
			colorJson.push_back(value.value.valueColor.b);

			j[name] = colorJson;
			break;
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
	Method::Method()
	{
	}

	Method::Method(const std::string& name, const std::string& className)
	{
		this->name = name;
		this->component = className;
	}

	void Method::SetReturn(const Variable& ret)
	{
		returnType = ret;
	}

	void Method::AddInput(const Variable& input)
	{
		this->input.push_back(input);
	}

	std::string Method::getName() const
	{
		return name;
	}

	std::string Method::getComponent() const
	{
		return component;
	}

	Variable Method::getReturn() const
	{
		return returnType;
	}

	const std::vector<Variable>& Method::getInput() const {

		return input;
	}

}
