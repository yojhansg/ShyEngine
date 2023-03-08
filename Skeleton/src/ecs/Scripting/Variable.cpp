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
		Variable val;
		val.type = Type::Int;
		val.value.Int = value;
		return val;
	}

	Variable Scripting::Variable::Float(float value)
	{
		Variable val;
		val.type = Type::Float;
		val.value.Float = value;
		return val;
	}

	Variable Scripting::Variable::Bool(bool value)
	{
		Variable val;
		val.type = Type::Bool;
		val.value.Bool = value;
		return val;
	}

	Variable Scripting::Variable::Char(char value)
	{
		Variable val;
		val.type = Type::Char;
		val.value.Char = value;
		return val;
	}

	Variable Variable::Component(ECS::Component* comp)
	{
		Variable val;
		val.type = Type::Component;
		val.value.pointer = comp;
		return val;
	}

}