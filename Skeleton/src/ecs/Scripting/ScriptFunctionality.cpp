#include "ScriptFunctionality.h"
#include <math.h>
#include <iostream>
#include "Entity.h"
#include <Script.h>
#include "StringTrim.h"

#define DELTA 0.001f

Scripting::ScriptFunctionality::~ScriptFunctionality()
{
}

Scripting::ScriptFunctionality::ScriptFunctionality()
{
}


ECS::Entity* Scripting::ScriptFunctionality::GameObject()
{
	return ECS::Script::currentScript->getEntity();
}


void Scripting::ScriptFunctionality::Print(Variable val)
{
	switch (val.type)
	{
	case Scripting::Variable::Type::Bool:
		std::cout << val.value.Bool ? "True" : "False";
		break;
	case Scripting::Variable::Type::Float:
		std::cout << val.value.Float;
		break;
	case Scripting::Variable::Type::Char:
		std::cout << val.value.Char;
		break;
	case Scripting::Variable::Type::Entity:
		std::cout << val.value.entity->getEntityName();
		break;
	case Scripting::Variable::Type::Vector2D:
		std::cout << val.vector;
		break;
	case Scripting::Variable::Type::String:
		std::cout << val.str;
		break;
	case Scripting::Variable::Type::Null:
		std::cout << "null";
		break;
	default:
		break;
	}

	std::cout << "\n";
}


float Scripting::ScriptFunctionality::Math_Add(float a, float b)
{
	return a + b;
}

float Scripting::ScriptFunctionality::Math_Subtract(float a, float b)
{
	return a - b;
}

float Scripting::ScriptFunctionality::Math_Multiply(float a, float b)
{
	return a * b;
}

float Scripting::ScriptFunctionality::Math_Divide(float a, float b)
{
	return a / b;
}

float Scripting::ScriptFunctionality::Math_Power(float a, float b)
{
	return powf(a, b);
}

float Scripting::ScriptFunctionality::Math_Root(float a, float b)
{
	return powf(a , 1.0f / b);
}

bool Scripting::ScriptFunctionality::Logic_Equals(float a, float b)
{
	return abs(a - b) <= DELTA;
}

bool Scripting::ScriptFunctionality::Logic_NotEquals(float a, float b)
{
	return abs(a - b) > DELTA;

}

bool Scripting::ScriptFunctionality::Logic_Lesser(float a, float b)
{
	return a < b;
}

bool Scripting::ScriptFunctionality::Logic_Greater(float a, float b)
{
	return a > b;
}

bool Scripting::ScriptFunctionality::Logic_LesserOrEqual(float a, float b)
{
	if (Logic_Equals(a, b))
		return true;

	return Logic_Lesser(a, b);
}

bool Scripting::ScriptFunctionality::Logic_GreaterOrEqual(float a, float b)
{
	if (Logic_Equals(a, b))
		return true;

	return Logic_Greater(a, b);
}

bool Scripting::ScriptFunctionality::Logic_And(bool a, bool b)
{
	return a && b;
}

bool Scripting::ScriptFunctionality::Logic_Or(bool a, bool b)
{
	return a || b;
}

bool Scripting::ScriptFunctionality::Logic_Negate(bool b)
{
	return !b;
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Create(float x, float y)
{
	return Vector2D(x, y);
}

float Scripting::ScriptFunctionality::Vector2D_Magnitude(Vector2D vec)
{
	return vec.magnitude();
}

float Scripting::ScriptFunctionality::Vector2D_X(Vector2D vec)
{
	return vec.getX();
}

float Scripting::ScriptFunctionality::Vector2D_Y(Vector2D vec)
{
	return vec.getY();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Normalize(Vector2D vec)
{
	return Vector2D(vec).normalize();
}

float Scripting::ScriptFunctionality::Vector2D_Angle(Vector2D vec)
{
	Vector2D norm(vec);
	norm.normalize();

	return norm.angle(Vector2D(1, 0));
}


float Scripting::ScriptFunctionality::Vector2D_AngleWithVector(Vector2D vec, Vector2D other)
{
	return vec.angle(other);
}


std::string Scripting::ScriptFunctionality::String_Concatenate(std::string a, std::string b)
{
	return a + b;
}

std::string Scripting::ScriptFunctionality::String_Substring(std::string a, int begin, int end)
{
	return a.substr(begin, end - begin);
}

std::string Scripting::ScriptFunctionality::String_Begining(std::string a, int b) {
	return a.substr(0, b);
}
std::string Scripting::ScriptFunctionality::String_End(std::string a, int b) {
	return a.substr(a.size() - b);
}
std::string Scripting::ScriptFunctionality::String_Trim(std::string a, std::string values) {
	return Utilities::trim(a, values);
}
std::string Scripting::ScriptFunctionality::String_TrimBlanks(std::string a) {
	return Utilities::trim(a);

}
char Scripting::ScriptFunctionality::String_GetLetter(std::string a, int b) {
	return a[b];
}
int Scripting::ScriptFunctionality::String_Find(std::string a, char c) {
	return a.find(c);
}