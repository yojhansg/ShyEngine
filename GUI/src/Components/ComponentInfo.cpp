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

		if (typeString == "int" || typeString == "float") {
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
		else if (typeString == "char") {
			value.value.valueChar = ' ';
		}
		else {
			type = AttributesType::NONE;
		}
	}

	Attribute::Attribute(const std::string& name)
	{
		this->name = name;
	}

	void Attribute::SetType(const std::string& str, const AttributesType& type)
	{
		typeStr = str;
		this->type = type;
	}

	void Attribute::SetValue(const AttributeValue& value)
	{
		this->value = value;
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
		std::string stringValue;

		switch (type)
		{
		case AttributesType::FLOAT:
			return std::to_string(value.value.valueFloat);
			break;
		case AttributesType::VECTOR2:
			stringValue = std::to_string(value.value.valueVector2.x) + "," + std::to_string(value.value.valueVector2.y);
			return stringValue;
			break;
		case AttributesType::STRING:
			return value.valueString;
			break;
		case AttributesType::BOOL:
			return std::to_string(value.value.valueBool);
			break;
		case AttributesType::COLOR:
			stringValue = std::to_string(value.value.valueColor.r) + "," + std::to_string(value.value.valueColor.g) + "," + std::to_string(value.value.valueColor.b);
			return stringValue;
			break;
		case AttributesType::CHAR:
			if (value.value.valueChar == '\0') return "";
			return std::string(1, value.value.valueChar);
		default:
			return "";
			break;
		}
	}

	std::string Component::toJson() {
		nlohmann::ordered_json j;

		j["component"] = name;

		nlohmann::ordered_json attributesJson;
		for (auto it = attributes.begin(); it != attributes.end(); it++) {
			attributesJson[it->first] = it->second.toJson();
		}

		j["attributes"] = attributesJson;

		return j.dump(2);
	}

	Method::Method()
	{
	}

	Method::Method(Method const& other)
	{
		name = other.name;
		component = other.component;
		returnType = other.returnType;
		input = other.input;
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


	Script::Script(cstring name) : name(name)
	{

	}

	std::string Script::GetName()
	{
		return name;
	}
	void Script::AddAttribute(const std::string& name, Attribute attr)
	{
		attributes.emplace(name, attr);
	}

	std::unordered_map<std::string, Attribute>& Script::getAllAttributes() {
		return attributes;
	}



	std::string Script::ToJson() {

		nlohmann::json root;

		for (auto& attr : attributes) {

			nlohmann::json value;


			value["type"] = attr.second.getTypeStr();

			switch (attr.second.getType())
			{
				case Components::AttributesType::BOOL:
					value["value"] = attr.second.value.value.valueBool;
				break;
				case Components::AttributesType::FLOAT:
					value["value"] = attr.second.value.value.valueFloat;
					break;
				case Components::AttributesType::CHAR:
					value["value"] = attr.second.value.value.valueChar;
					break;
				case Components::AttributesType::STRING:
					value["value"] = attr.second.value.valueString;
					break;
				case Components::AttributesType::VECTOR2:
					//value["value"] = attr.second.value.value.valueBool;
					break;
				case Components::AttributesType::COLOR:
					//value["value"] = attr.second.value.value.valueBool;
					break;

			default:
				break;
			}


			root[attr.first] = value;
		}


		return root.dump(2);
	}
}
