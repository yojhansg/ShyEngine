#pragma once


namespace ECS {
	class Component;
}

namespace Scripting {


	struct Variable {

		enum class Type {
			Null, Int, Float, Bool, Char, Component
		};
		Type type;

		union Value {
			int Int;
			float Float;
			bool Bool;
			char Char;

			ECS::Component* pointer;
		};
		Value value;


		Variable();
		Variable(Type type);

		/*static constructors*/
		static Variable Null();
		static Variable Int(int value);
		static Variable Float(float value);
		static Variable Bool(bool value);
		static Variable Char(char value);
		static Variable Component(ECS::Component* comp);
	};


	using Input = Variable;
	using Output = Variable;
}
