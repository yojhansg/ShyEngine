#include "Variable.h"
#include <iostream>

namespace Scripting {

	Variable::Variable()
	{
		type = Type::Null;
		value.Float = 0;
	}

	Variable::Variable(Type t)
	{
		type = t;
		value.Float = 0;
	}



	Variable Scripting::Variable::Null()
	{
		return Variable();
	}

	Variable Scripting::Variable::Int(int value)
	{
		return value;
	}

	Variable Scripting::Variable::Float(float value)
	{
		return value;
	}

	Variable Scripting::Variable::Bool(bool value)
	{
		return value;
	}

	Variable Scripting::Variable::Char(char value)
	{
		return value;
	}

	Variable Variable::Entity(ECS::Entity* comp)
	{
		return comp;
	}

	Variable Variable::Vector2D(Utilities::Vector2D vector)
	{
		return vector;
	}

	Variable Variable::String(std::string const& str)
	{
		return str;
	}

	Variable::Variable(int val)
	{
		type = Type::Float;
		value.Float = val;
	}

	Variable::Variable(float val)
	{
		type = Type::Float;
		value.Float = val;
	}

	
	Variable::Variable(bool val)
	{
		type = Type::Bool;
		value.Bool = val;
	}

	
	Variable::Variable(char val)
	{
		type = Type::Char;
		value.Char = val;
	}

	Variable::Variable(ECS::Entity* entity)
	{
		type = Type::Entity;
		value.entity = entity;
	}

	Variable::Variable(Utilities::Vector2D vector)
	{
		type = Type::Vector2D;
		this->vector = vector;
		value.Float = 0;
	}

	Variable::Variable(std::string const& str)
	{
		type = Type::String;
		this->str = str;
		value.Float = 0;
	}
}