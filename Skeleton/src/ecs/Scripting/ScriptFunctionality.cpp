#include "ScriptFunctionality.h"
#include <math.h>
#include "Entity.h"
#include <Script.h>
#include "StringTrim.h"

#include "ConsoleManager.h"

#include <Windows.h>
#include <shellapi.h>


#define DELTA 0.001f

Scripting::ScriptFunctionality::~ScriptFunctionality()
{
}

Scripting::ScriptFunctionality::ScriptFunctionality()
{
}


ECS::Entity* Scripting::ScriptFunctionality::Entity()
{
	return ECS::Script::currentScript->getEntity();
}

std::string Scripting::ScriptFunctionality::EntityName()
{
	return Entity()->getEntityName();
}

std::string Scripting::ScriptFunctionality::Script()
{
	return ECS::Script::currentScript->GetName();
}


void Scripting::ScriptFunctionality::Print(Variable val)
{
	std::string objectName = ECS::Script::currentScript->getEntity()->getEntityName();

	std::string scriptName = ECS::Script::currentScript->GetName();

	switch (val.type)
	{
	case Scripting::Variable::Type::Bool:
		Console::Output::Print( objectName + ": " + scriptName, val.value.Bool ? "Yes" : "No");
		break;
	case Scripting::Variable::Type::Float:
		Console::Output::Print(objectName + ": " + scriptName, std::to_string(val.value.Float));
		break;
	case Scripting::Variable::Type::Char:
		Console::Output::Print(objectName + ": " + scriptName, std::to_string(val.value.Char));
		break;
	case Scripting::Variable::Type::Entity:
		Console::Output::Print(objectName + ": " + scriptName, val.value.entity->getEntityName());
		break;
	case Scripting::Variable::Type::Vector2D:
		Console::Output::Print(objectName + ": " + scriptName, val.vector);
		break;
	case Scripting::Variable::Type::String:
		Console::Output::Print(objectName + ": " + scriptName, val.str);
		break;
	case Scripting::Variable::Type::Null:
		Console::Output::Print(objectName + ": " + scriptName, "null");
		break;
	default:
		Console::Output::PrintWarning(objectName + ": " + scriptName, "Unknown value");
		break;
	}
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

Vector2D Scripting::ScriptFunctionality::Vector2D_Rotate(Vector2D vec, float angle)
{
	return vec.rotate(angle);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Add(Vector2D a, Vector2D b)
{
	return a + b;
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Subtract(Vector2D a, Vector2D b)
{
	return a - b;
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Dot(Vector2D a, Vector2D b)
{
	//TODO: Dot product
	Console::Output::PrintWarning("Not implemented", "Vector2D_Dot operation is not implemented yet");
	return Vector2D();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Cross(Vector2D a, Vector2D b)
{
	//TODO: Dot product
	Console::Output::PrintWarning("Not implemented", "Vector2D_Cross operation is not implemented yet");
	return Vector2D();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Mult(Vector2D a, float b)
{
	return a * b;
}

bool Scripting::ScriptFunctionality::String_Equals(std::string a, std::string b)
{
	return a == b;
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


void Scripting::ScriptFunctionality::Open_URL(std::string url) {

	ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}

void Scripting::ScriptFunctionality::Set(std::string name, Scripting::Variable val)
{
	ECS::Script::currentScript->Set(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::Get(std::string name)
{
	return ECS::Script::currentScript->Get(name);
}

void Scripting::ScriptFunctionality::SetLocal(std::string name, Scripting::Variable val)
{
	Entity()->SetAttribute(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::GetLocal(std::string name)
{
	return Entity()->GetAttribute(name);
}

void Scripting::ScriptFunctionality::SetGlobal(std::string name, Scripting::Variable val)
{
	ScriptManager::instance()->SetGlobal(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::GetGlobal(std::string name)
{
	return ScriptManager::instance()->GetGlobal(name);
}
