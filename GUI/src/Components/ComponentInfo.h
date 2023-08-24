#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_map>

namespace Components {

	using cstring = std::string const&;

	//TODO: implementar las cosas para leer chars
	enum class AttributesType {
		NONE,
		FLOAT,
		VECTOR2,
		STRING,
		BOOL,
		COLOR,
		CHAR,
		GAMEOBJECT
	};


	struct AttributeValue {

		union value
		{
			float valueFloat;
			struct {
				float x;
				float y;
			} valueVector2;
			bool valueBool;
			struct {
				float r;
				float g;
				float b;
			} valueColor;
			char valueChar;
			int entityIdx;
		} value;

		std::string valueString;
	};


	class Attribute {

	private:
		AttributesType type;
		std::string name;
		std::string typeStr;

		static std::unordered_map<std::string, AttributesType> attributeTypes;

	public:

		Attribute();
		Attribute(const std::string& name, const std::string& typeString);
		Attribute(const std::string& name);

		void SetType(const std::string& str, const AttributesType& type);
		void SetValue(const AttributeValue& value);
		AttributesType getType() const;

		std::string getName() const;
		std::string getTypeStr() const;
		std::string toJson();

		static Attribute fromJson(std::string name, std::string json);

		AttributeValue value;
	};

	using Variable = Attribute;

	class Method {

	private:

		std::string component;
		std::string name;

		Variable returnType;
		std::vector<Variable> input;
	public:

		Method();
		Method(Method const& other);
		Method(const std::string& name, const std::string& className);

		void SetReturn(const Variable& ret);
		void AddInput(const Variable& input);

		std::string getName() const;
		std::string getComponent() const;
		Variable getReturn() const;

		const std::vector<Variable>& getInput() const;
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

		cstring GetName();

		Attribute& getAttribute(cstring name);
		Method& getMethod(cstring name);

		std::unordered_map<std::string, Attribute>& getAllAttributes();
		std::unordered_map<std::string, Method>& getAllMethods();

		void addAttribute(const Attribute& attribute);
		void addMethod(const Method& method);

		std::string toJson();

		static Component fromJson(std::string json);
	};


	class Script {

	private:

		std::string name;
		std::unordered_map<std::string, Attribute> attributes;

	public:

		Script(cstring name);

		std::string GetName();

		void AddAttribute(const std::string& name, Attribute attr);
		std::unordered_map<std::string, Attribute>& getAllAttributes();

		std::string ToJson();

		static Script fromJson(std::string name, std::string json);
	};

}
