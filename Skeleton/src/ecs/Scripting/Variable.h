#pragma once


namespace ECS {
	class Component;
}

//TODO: meter el tipo vector2d
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


		operator int(){
			return 3;
		}

		Variable(int value);
		Variable(float value);
		Variable(bool value);
		Variable(char value);
		Variable(ECS::Component* comp);

	};




	using Input = Variable;
	using Output = Variable;
}
