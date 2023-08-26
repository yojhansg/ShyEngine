#include "ComponentInfo.h"
#include "nlohmann/json.hpp"

#include "CheckML.h"

namespace Components {

	std::unordered_map<std::string, AttributesType> Attribute::attributeTypes;

	Component::Component()
	{
		name = "";
	}

	Component::Component(const Component& other)
	{
		name = other.name;

		methods = other.methods;
		attributes = other.attributes;

		for (auto& m : other.orderedMethods) {

			orderedMethods.push_back(&methods.at(m->getName()));
		}

		for (auto& attr : other.orderedAttributes) {

			orderedAttributes.push_back(&attributes.at(attr->getName()));
		}
	}

	Component::Component(cstring name)
	{
		this->name = name;
	}

	cstring Component::GetName()
	{
		return name;
	}

	Attribute& Component::getAttribute(cstring name)
	{
		return attributes[name];
	}

	Method& Component::getMethod(cstring name)
	{
		return methods[name];
	}

	std::unordered_map<std::string, Attribute>& Component::getAllAttributes()
	{
		return attributes;
	}

	std::unordered_map<std::string, Method>& Component::getAllMethods()
	{
		return methods;
	}

	std::vector<Attribute*>& Component::GetAttributesOrdered()
	{
		return orderedAttributes;
	}

	std::vector<Method*>& Component::GetMethodsOrdered()
	{
		return orderedMethods;
	}

	void Component::addAttribute(const Attribute& attribute)
	{
		const auto& name = attribute.getName();

		attributes.emplace(name, attribute);


		auto& ref = attributes.at(name);
		orderedAttributes.push_back(&ref);
	}

	void Component::addMethod(const Method& method)
	{
		const auto& name = method.getName();


		methods.emplace(name, method);

		auto& ref = methods.at(name);
		orderedMethods.push_back(&ref);
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

		if (typeString == "float") {
			value.value.valueFloat = 0.0f;
			type = AttributesType::FLOAT;
		}
		else if (typeString == "Vector2D") {
			value.value.valueVector2 = { 0.0f, 0.0f };
			type = AttributesType::VECTOR2;
		}
		else if (typeString == "string") {
			value.valueString = "";
			type = AttributesType::STRING;
		}
		else if (typeString == "bool") {
			value.value.valueBool = false;
			type = AttributesType::BOOL;
		}
		else if (typeString == "color") {
			value.value.valueColor = { 0.0f, 0.0f, 0.0f };
			type = AttributesType::COLOR;
		}
		else if (typeString == "char") {
			value.value.valueChar = ' ';
			type = AttributesType::CHAR;
		}
		else if (typeString == "Entity") {
			value.value.entityIdx = 0;
			type = AttributesType::ENTITY;
		}
		else {
			type = AttributesType::NONE;
		}

		Attribute::attributeTypes[this->name] = this->type;
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

	const std::string& Attribute::getName() const {
		return name;
	}

	const std::string& Attribute::getTypeStr() const
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
			stringValue = std::to_string(value.value.valueVector2.x) + ", " + std::to_string(value.value.valueVector2.y);
			return stringValue;
			break;
		case AttributesType::STRING:
			return value.valueString;
			break;
		case AttributesType::BOOL:
			return value.value.valueBool ? "true" : "false";
			break;
		case AttributesType::COLOR:
			stringValue = std::to_string(value.value.valueColor.r) + ", " + std::to_string(value.value.valueColor.g) + ", " + std::to_string(value.value.valueColor.b);
			return stringValue;
			break;
		case AttributesType::CHAR:
			if (value.value.valueChar == '\0') return "";
			return std::string(1, value.value.valueChar);
		case AttributesType::ENTITY:
			return std::to_string(value.value.entityIdx);
		default:
			return "";
			break;
		}
	}

	Attribute Attribute::fromJson(std::string name, std::string json)
	{
		Attribute attribute;

		attribute.name = name;
		attribute.type = Attribute::attributeTypes[name];

		nlohmann::ordered_json jsonData = nlohmann::json::parse(json);

		switch (attribute.type) {
		case AttributesType::FLOAT:
			attribute.value.value.valueFloat = std::stof(jsonData.get<std::string>());
			break;
		case AttributesType::VECTOR2:
			sscanf_s(jsonData.get<std::string>().c_str(), "%f, %f", &attribute.value.value.valueVector2.x, &attribute.value.value.valueVector2.y);
			break;
		case AttributesType::STRING:
			attribute.value.valueString = jsonData.get<std::string>();
			break;
		case AttributesType::BOOL:
			attribute.value.value.valueBool = jsonData.get<std::string>() == "true" ? true : false;
			break;
		case AttributesType::COLOR:
			sscanf_s(jsonData.get<std::string>().c_str(), "%f, %f, %f", &attribute.value.value.valueColor.r, &attribute.value.value.valueColor.g, &attribute.value.value.valueColor.b);
			break;
		case AttributesType::CHAR:
			attribute.value.value.valueChar = jsonData.get<std::string>()[0];
			break;
		case AttributesType::ENTITY:
			attribute.value.value.entityIdx = std::stoi(jsonData.get<std::string>());
			break;
		default:
			break;
		}

		return attribute;
	}

	nlohmann::ordered_json Component::toJson() {
		nlohmann::ordered_json j;

		j["component"] = name;

		nlohmann::ordered_json attributesJson;
		for (auto& attr : orderedAttributes) {

			attributesJson[attr->getName()] = attr->toJson();
		}


		j["attributes"] = attributesJson;

		return j;
	}

	Component Component::fromJson(std::string json) {

		Component component;

		nlohmann::ordered_json jsonData = nlohmann::json::parse(json);

		component.name = jsonData["component"];

		nlohmann::json attributesJson = jsonData["attributes"];
		for (const auto& attrJson : attributesJson.items()) {
			::Components::Attribute attr = ::Components::Attribute::fromJson(attrJson.key(), attrJson.value().dump());
			component.attributes[attr.getName()] = attr;

			//Se hace un at para obtener una referencia al valor guardado dentro del mapa
			component.orderedAttributes.push_back(&component.attributes.at(attr.getName()));
		}

		return component;
	}

	Method::Method() {}

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


	Script::Script(cstring name) : name(name) {}

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
			case Components::AttributesType::ENTITY:
				value["value"] = attr.second.value.value.entityIdx;
				break;

			default:
				break;
			}

			root[attr.first] = value;
		}


		return root.dump(2);
	}

	Script Script::fromJson(std::string name, std::string json) {
		Script script(name);

		nlohmann::json jsonData = nlohmann::json::parse(json);


		for (const auto& jsonEntry : jsonData.items()) {
			const std::string attributeName = jsonEntry.key();
			const nlohmann::json& attributeJson = jsonEntry.value();

			const std::string attributeType = attributeJson["type"];

			Attribute attribute(attributeName, attributeType);

			switch (attribute.getType()) {
			case Components::AttributesType::BOOL:

				attribute.value.value.valueBool = attributeJson["value"].get<bool>();
				break;
			case Components::AttributesType::FLOAT:
				attribute.value.value.valueFloat = attributeJson["value"].get<float>();
				break;
			case Components::AttributesType::CHAR:
				attribute.value.value.valueChar = attributeJson["value"].get<char>();
				break;
			case Components::AttributesType::STRING:
				attribute.value.valueString = attributeJson["value"].get<std::string>();
				break;
			case Components::AttributesType::VECTOR2:
				//value["value"] = attr.second.value.value.valueBool;
				break;
			case Components::AttributesType::COLOR:
				//value["value"] = attr.second.value.value.valueBool;
				break;
			case Components::AttributesType::ENTITY:
				attribute.value.value.entityIdx = attributeJson["value"].get<int>();
				break;
			}

			//if (attributeJson["value"].is_boolean()) {
			//	attribute.value.value.valueBool = attributeJson["value"];
			//}
			//else if (attributeJson["value"].is_number_float()) {
			//	attribute.value.value.valueFloat = attributeJson["value"];
			//}
			//else if (attributeJson["value"].is_string()) {
			//	std::string valueStr = attributeJson["value"];
			//	if (valueStr.size() == 1) {
			//		attribute.value.value.valueChar = valueStr[0];
			//	}
			//	else {
			//		attribute.value.valueString = valueStr;
			//	}
			//}

			//ADD REMAINING TYPES

			script.AddAttribute(attributeName, attribute);
		}

		return script;
	}
}
