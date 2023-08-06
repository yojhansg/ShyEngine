#include "ComponentInfo.h"

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
			type = INT;
		}
		else if (typeString == "float") {
			type = FLOAT;
		}
		else if (typeString == "Utilities::Vector2D") {
			type = VECTOR2;
		}
		else if (typeString == "std::string") {
			type = STRING;
		}
		else if (typeString == "bool") {
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

}
