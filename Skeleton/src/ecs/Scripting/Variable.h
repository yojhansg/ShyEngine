#pragma once


namespace ECS {
	class Component;
}

//TODO: hacer que en vez de guardar un componente como variable se guarde directamente el objeto
//TODO: meter el tipo vector2d
namespace Scripting {


	struct Variable {

		enum class Type {
			Null, Int, Float, Bool, Char, Component
		};

		union Value {
			int Int;
			float Float;
			bool Bool;
			char Char;

			ECS::Component* pointer;
		};


		Type type;
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

		Variable(int value);
		Variable(float value);
		Variable(bool value);
		Variable(char value);
		Variable(ECS::Component* comp);


		Variable operator +(Variable const& other) const;
		Variable operator -(Variable const& other) const;
		Variable operator *(Variable const& other) const;
		Variable operator /(Variable const& other) const;
		Variable operator %(Variable const& other) const;

		Variable Pow(Variable const& other) const;


		/*
			Arithmetic grid

			-------   Null Int Float   Char Component

			Int        x    o    o      o       x  
			Float      x    o    o      x       x
			Char       x    o    x      o       x
		
		*/

		static bool ArithmeticCheck(Variable const& a, Variable const& b);
	};




	using Input = Variable;
	using Output = Variable;
}
