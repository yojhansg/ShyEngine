#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace Components {

	using cstring = std::string const&;

	enum AttributesType {
		NONE,
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
		Attribute();
		Attribute(std::string name, std::string typeString);

		AttributesType getType() const;
		std::string getName() const;

		std::string toJson();

		union value
		{
			int valueInt;
			float valueFloat;
			struct {
				float x;
				float y;
			} valueVector2;
			bool valueBool;
			//color
		} value;

		std::string valueString;
	};

	struct Method {

		std::string name;

		Attribute returnType;
		std::vector<Attribute> input;
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

		std::string  toJson();
	};

}
