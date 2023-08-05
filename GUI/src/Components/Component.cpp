#include "Component.h"

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
		attributes.emplace(attribute.name, attribute);
	}

	void Components::Component::addMethod(const Method& method)
	{
		methods.emplace(method.name, method);
	}

}
