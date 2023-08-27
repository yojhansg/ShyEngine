#include "ComponentInfo.h"

#include "ComponentManager.h"

#include "nlohmann/json.hpp"

#include "CheckML.h"

namespace Components {

	Component::Component()
	{
		name = "";
	}

	Component::Component(const Component& other)
	{
		name = other.name;

		functions = other.functions;
		attributes = other.attributes;

		for (auto& m : other.orderedFunctions) {

			orderedFunctions.push_back(&functions.at(m->getName()));
		}

		for (auto& attr : other.orderedAttributes) {

			orderedAttributes.push_back(&attributes.at(attr->GetName()));
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

	Attribute& Component::GetAttribute(cstring name)
	{
		return attributes[name];
	}

	Function& Component::GetFunction(cstring name)
	{
		return functions[name];
	}

	std::unordered_map<std::string, Attribute>& Component::GetAllAttributes()
	{
		return attributes;
	}

	std::unordered_map<std::string, Function>& Component::GetAllFunctions()
	{
		return functions;
	}

	std::vector<Attribute*>& Component::GetAttributesOrdered()
	{
		return orderedAttributes;
	}

	std::vector<Function*>& Component::GetFunctionsOrdered()
	{
		return orderedFunctions;
	}

	void Component::AddAttribute(const Attribute& attribute)
	{
		const auto& name = attribute.GetName();

		attributes.emplace(name, attribute);


		auto& ref = attributes.at(name);
		orderedAttributes.push_back(&ref);
	}

	void Component::AddFunction(const Function& function)
	{
		const auto& name = function.getName();


		functions.emplace(name, function);

		auto& ref = functions.at(name);
		orderedFunctions.push_back(&ref);
	}

	void Attribute::SetDefaultValues()
	{
		if (type == AttributesType::FLOAT) {
			value.value.valueFloat = 0.0f;
		}
		else if (type == AttributesType::VECTOR2) {
			value.value.valueVector2 = { 0.0f, 0.0f };
		}
		else if (type == AttributesType::STRING) {
			value.valueString = "";
		}
		else if (type == AttributesType::BOOL) {
			value.value.valueBool = false;
		}
		else if (type == AttributesType::COLOR) {
			value.value.valueColor = { 0.0f, 0.0f, 0.0f };
		}
		else if (type == AttributesType::CHAR) {
			value.value.valueChar = ' ';
		}
		else if (type == AttributesType::ENTITY) {
			value.value.entityIdx = 0;
		}
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

		type = Attribute::GetAttributeTypeFromTypeStr(typeString);

		SetDefaultValues();
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

	void Attribute::SetValueStr(const std::string& str)
	{
		switch (type) {
		case Components::AttributesType::BOOL:
			value.value.valueBool = str == "true" ? true : false;
			break;
		case Components::AttributesType::FLOAT:
			value.value.valueFloat = std::stof(str);
			break;
		case Components::AttributesType::CHAR:
			value.value.valueChar = str[0];
			break;
		case Components::AttributesType::STRING:
			value.valueString = str;
			break;
		case Components::AttributesType::VECTOR2:
			sscanf_s(str.c_str(), "%f, %f", &value.value.valueVector2.x, &value.value.valueVector2.y);
			break;
		case Components::AttributesType::COLOR:
			sscanf_s(str.c_str(), "%f, %f, %f", &value.value.valueColor.r, &value.value.valueColor.g, &value.value.valueColor.b);
			break;
		case Components::AttributesType::ENTITY:
			value.value.entityIdx = std::stoi(str);
			break;
		}
	}

	AttributesType Attribute::GetType() const
	{
		return type;
	}

	const std::string& Attribute::GetName() const {
		return name;
	}

	const std::string& Attribute::GetTypeStr() const
	{
		return typeStr;
	}

	std::string Attribute::GetTypeStrFromAttributeType(AttributesType type) {
		std::string returnValue = "";

		if (type == AttributesType::FLOAT) {
			returnValue = "float";
		}
		else if (type == AttributesType::VECTOR2) {
			returnValue = "Vector2D";
		}
		else if (type == AttributesType::STRING) {
			returnValue = "string";
		}
		else if (type == AttributesType::BOOL) {
			returnValue = "bool";
		}
		else if (type == AttributesType::COLOR) {
			returnValue = "color";
		}
		else if (type == AttributesType::CHAR) {
			returnValue = "char";
		}
		else if (type == AttributesType::ENTITY) {
			returnValue = "Entity";
		}
		else if (type == AttributesType::NONE) {
			returnValue = "null";
		}
		
		return returnValue;
	}

	AttributesType Attribute::GetAttributeTypeFromTypeStr(const std::string& typeStr) {

		if (typeStr == "float") {
			return AttributesType::FLOAT;
		}
		else if (typeStr == "Vector2D") {
			return AttributesType::VECTOR2;
		}
		else if (typeStr == "string") {
			return AttributesType::STRING;
		}
		else if (typeStr == "bool") {
			return AttributesType::BOOL;
		}
		else if (typeStr == "color") {
			return AttributesType::COLOR;
		}
		else if (typeStr == "char") {
			return AttributesType::CHAR;
		}
		else if (typeStr == "Entity") {
			return AttributesType::ENTITY;
		}
		else {
			return AttributesType::NONE;
		}
	}

	std::string Attribute::ToJson()
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


	nlohmann::ordered_json Component::ToJson() {
		nlohmann::ordered_json j;

		j["component"] = name;

		nlohmann::ordered_json attributesJson;
		for (auto& attr : orderedAttributes) {

			attributesJson[attr->GetName()] = attr->ToJson();
		}


		j["attributes"] = attributesJson;

		return j;
	}

	Component Component::FromJson(std::string json) {


		nlohmann::ordered_json jsonData = nlohmann::json::parse(json);

		std::string name = jsonData["component"];
		Component component = Components::ComponentManager::GetAllComponents()[name];


		nlohmann::json attributesJson = jsonData["attributes"];
		for (const auto& attrJson : attributesJson.items()) {

			auto& attr = component.attributes[attrJson.key()];

			attr.SetValueStr(attrJson.value());
		}

		return component;
	}

	Function::Function() {}

	Function::Function(Function const& other)
	{
		name = other.name;
		component = other.component;
		returnType = other.returnType;
		input = other.input;
	}

	Function::Function(const std::string& name, const std::string& className)
	{
		this->name = name;
		this->component = className;
	}

	void Function::SetReturn(const Variable& ret)
	{
		returnType = ret;
	}

	void Function::AddInput(const Variable& input)
	{
		this->input.push_back(input);
	}

	std::string Function::getName() const
	{
		return name;
	}

	std::string Function::getComponent() const
	{
		return component;
	}

	Variable Function::getReturn() const
	{
		return returnType;
	}

	const std::vector<Variable>& Function::getInput() const {

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

	std::unordered_map<std::string, Attribute>& Script::GetAllAttributes() {
		return attributes;
	}

	std::string Script::ToJson() {

		nlohmann::json root;

		for (auto& attr : attributes) {

			nlohmann::json value;


			value["type"] = attr.second.GetTypeStr();

			switch (attr.second.GetType())
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
				value["value"] = std::to_string(attr.second.value.value.valueVector2.x) + ", " + std::to_string(attr.second.value.value.valueVector2.y);
				break;
			case Components::AttributesType::COLOR:
				value["value"] = std::to_string(attr.second.value.value.valueColor.r) + ", " + std::to_string(attr.second.value.value.valueColor.g) + ", " + std::to_string(attr.second.value.value.valueColor.b);
				break;
			case Components::AttributesType::ENTITY:
				value["value"] = attr.second.value.value.entityIdx;
				break;
			case Components::AttributesType::NONE:
				value["value"] = "null";
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

			switch (attribute.GetType()) {
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
				sscanf_s(attributeJson["value"].get<std::string>().c_str(), "%f, %f", &attribute.value.value.valueVector2.x, &attribute.value.value.valueVector2.y);
				break;
			case Components::AttributesType::COLOR:
				sscanf_s(attributeJson["value"].get<std::string>().c_str(), "%f, %f, %f", &attribute.value.value.valueColor.r, &attribute.value.value.valueColor.g, &attribute.value.value.valueColor.b);
				break;
			case Components::AttributesType::ENTITY:
				attribute.value.value.entityIdx = attributeJson["value"].get<int>();
				break;
			}

			script.AddAttribute(attributeName, attribute);
		}

		return script;
	}
}
