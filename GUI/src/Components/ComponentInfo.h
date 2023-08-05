#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace Components {

	using cstring = std::string const&;

	struct Variable {

		std::string name;
		std::string type;
	};

	using Attribute = Variable;

	struct Method {

		std::string name;

		Variable returnType;
		std::vector<Variable> input;
	};


	class Component {

	private:

		std::string name;

		std::unordered_map<std::string, Attribute> attributes;
		std::unordered_map<std::string, Method> methods;

	public:

		Component();
		Component(const Component& other);
		Component(cstring name);

		cstring getName();

		Attribute& getAttribute(cstring name);
		Method& getMethod(cstring name);

		std::unordered_map<std::string, Attribute>& getAllAttributes();
		std::unordered_map<std::string, Method>& getAllMethods();

		void addAttribute(const Attribute& attribute);
		void addMethod(const Method& method);

	};

}
