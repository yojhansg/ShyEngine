#pragma once

#include "Vector2D.h"
#include <string>

namespace ECS {
	class Entity;
}

//TODO: hacer que en vez de guardar un componente como variable se guarde directamente el objeto
//TODO: meter el tipo string
namespace Scripting {


	struct Variable {

		enum class Type {
			Null, Float, Bool, Char, Entity, Vector2D, String
		};

		union Value {
			float Float;
			bool Bool;
			char Char;
			ECS::Entity* entity;
		};


		Type type;
		Value value;

		std::string str;
		Utilities::Vector2D vector;

		Variable();
		Variable(Type type);

		/*static constructors*/
		static Variable Null();
		static Variable Int(int value);
		static Variable Float(float value);
		static Variable Bool(bool value);
		static Variable Char(char value);
		static Variable Entity(ECS::Entity* entity);
		static Variable Vector2D(Utilities::Vector2D vector);
		static Variable String(std::string const& str);

		Variable(int value);
		Variable(float value);
		Variable(bool value);
		Variable(char value);
		Variable(ECS::Entity* entity);
		Variable(Utilities::Vector2D vector);
		Variable(std::string const& str);


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
