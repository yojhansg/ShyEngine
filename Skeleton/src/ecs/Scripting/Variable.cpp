#include "Variable.h"

#include <iostream>

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

	//Todo esto se puede mejorar probablemente con preprocesadores
	bool Variable::ArithmeticCheck(Variable const& a, Variable const& b)
	{
		if (a.type == Type::Null || b.type == Type::Null) {

			return false;
		}
		if (a.type == Type::Bool || b.type == Type::Bool) {

			return false;
		}
		if (a.type == Type::Component || b.type == Type::Component) {

			return false;
		}



		if (a.type == Type::Char && b.type == Type::Float) {

			return false;
		}
		if (a.type == Type::Float && b.type == Type::Char) {

			return false;
		}

		return true;
	}

	Variable Variable::operator+(Variable const& other) const
	{
		if (!ArithmeticCheck(*this, other)) {

			std::cout << "Error: No se puede realizar esa operacion" << std::endl;
			return Null();
		}

		if (type != other.type) {

			std::cout << "Error: Aun no se han implementado las operaciones entre distintos tipos" << std::endl;
			return Null();
		}

		Variable ret(type);

		switch (type)
		{
		case Scripting::Variable::Type::Int:
			ret.value.Int = value.Int + other.value.Int;
			break;
		case Scripting::Variable::Type::Float:
			ret.value.Float = value.Float + other.value.Float;
			break;
		case Scripting::Variable::Type::Char:
			ret.value.Char = value.Char + other.value.Char;
			break;
		}

		return ret;
	}

	Variable Variable::operator-(Variable const& other)const
	{
		if (!ArithmeticCheck(this, other)) {

			std::cout << "Error: No se puede realizar esa operacion" << std::endl;
			return Null();
		}

		if (type != other.type) {

			std::cout << "Error: Aun no se han implementado las operaciones entre distintos tipos" << std::endl;
			return Null();
		}

		Variable ret(type);

		switch (type)
		{
		case Scripting::Variable::Type::Int:
			ret.value.Float = value.Int - other.value.Int;
			break;
		case Scripting::Variable::Type::Float:
			ret.value.Float = value.Float - other.value.Float;
			break;
		case Scripting::Variable::Type::Char:
			ret.value.Char = value.Char - other.value.Char;
			break;
		}

		return ret;
	}

	Variable Variable::operator*(Variable const& other)const
	{
		if (!ArithmeticCheck(this, other)) {

			std::cout << "Error: No se puede realizar esa operacion" << std::endl;
			return Null();
		}

		if (type != other.type) {

			std::cout << "Error: Aun no se han implementado las operaciones entre distintos tipos" << std::endl;
			return Null();
		}

		Variable ret(type);

		switch (type)
		{
		case Scripting::Variable::Type::Int:
			ret.value.Float = value.Int * other.value.Int;
			break;
		case Scripting::Variable::Type::Float:
			ret.value.Float = value.Float * other.value.Float;
			break;
		case Scripting::Variable::Type::Char:
			ret.value.Char = value.Char * other.value.Char;
			break;
		}

		return ret;
	}

	Variable Variable::operator/(Variable const& other)const
	{
		if (!ArithmeticCheck(this, other)) {

			std::cout << "Error: No se puede realizar esa operacion" << std::endl;
			return Null();
		}

		if (type != other.type) {

			std::cout << "Error: Aun no se han implementado las operaciones entre distintos tipos" << std::endl;
			return Null();
		}

		Variable ret(type);

		switch (type)
		{
		case Scripting::Variable::Type::Int:
			ret.value.Float = value.Int / other.value.Int;
			break;
		case Scripting::Variable::Type::Float:
			ret.value.Float = value.Float / other.value.Float;
			break;
		case Scripting::Variable::Type::Char:
			ret.value.Char = value.Char / other.value.Char;
			break;
		}

		return ret;
	}

	Variable Variable::operator%(Variable const& other)const
	{
		std::cout << "Error: operacion no implementada" << std::endl;
		return Null();
	}

	Variable Variable::Pow(Variable const& other) const
	{
		std::cout << "Error: operacion no implementada" << std::endl;
		return Null();
	}


}