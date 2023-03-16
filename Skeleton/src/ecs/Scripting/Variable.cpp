#include "Variable.h"

namespace Scripting {

	Variable::Variable()
	{
		type = Type::Null;
		value.Int = 0;
	}

	Variable::Variable(Type t)
	{
		type = t;
		value.Int = 0;
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

	Variable Variable::Component(ECS::Component* comp)
	{
		return comp;
	}

	Variable::Variable(int val)
	{
		type = Type::Int;
		value.Int = val;
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

	Variable::Variable(ECS::Component* comp)
	{
		type = Type::Component;
		value.pointer = comp;
	}

}