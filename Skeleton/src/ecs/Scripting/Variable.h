#pragma once

#include "Vector2D.h"

namespace ECS {
	class Entity;
}

//TODO: hacer que en vez de guardar un componente como variable se guarde directamente el objeto
//TODO: meter el tipo vector2d
namespace Scripting {


	struct Variable {

		enum class Type {
			Null, Int, Float, Bool, Char, Entity
		};

		union Value {
			int Int;
			float Float;
			bool Bool;
			char Char;
			ECS::Entity* entity;
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
		static Variable Entity(ECS::Entity* entity);

		Variable(int value);
		Variable(float value);
		Variable(bool value);
		Variable(char value);
		Variable(ECS::Entity* entity);


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
