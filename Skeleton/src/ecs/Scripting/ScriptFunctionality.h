#pragma once

#include "Singleton.h"
#include "EditorExport.h"
#include "Vector2D.h"
#include "Scripting/Variable.h"

namespace ECS {

	class Entity;
}

using Utilities::Vector2D;

//TODO: Creo que para las funciones es mejor quitar el nombre script functionality

namespace Scripting {

	EditorManager ScriptFunctionality: public Utilities::Singleton<ScriptFunctionality>{

		friend Singleton<ScriptFunctionality>;

public:

	~ScriptFunctionality();


publish:

	ECS::Entity* Entity();

	//TODO: esto deberia depender de la entidad no dar la del propio objeto
	std::string EntityName();
	std::string Script();


	void Print(Scripting::Variable val);


	//Math
	float Math_Add(float a, float b);
	float Math_Subtract(float a, float b);
	float Math_Multiply(float a, float b);
	float Math_Divide(float a, float b);
	float Math_Power(float a, float b);
	float Math_Root(float a, float b);

	//Logic
	//TODO: logica con mas tipos diferentes ademas de numeros
	bool Logic_Equals(float a, float b);
	bool Logic_NotEquals(float a, float b);
	bool Logic_Lesser(float a, float b);
	bool Logic_Greater(float a, float b);
	bool Logic_LesserOrEqual(float a, float b);
	bool Logic_GreaterOrEqual(float a, float b);
	bool Logic_And(bool a, bool b);
	bool Logic_Or(bool a, bool b);
	bool Logic_Negate(bool b);

	//Vector2D
	//TODO: constructoras predeterminadas vector2d: left, up, one, zero...
	Vector2D Vector2D_Create(float x, float y);
	float Vector2D_Magnitude(Vector2D vec);
	float Vector2D_X(Vector2D vec);
	float Vector2D_Y(Vector2D vec);
	Vector2D Vector2D_Normalize(Vector2D vec);
	float Vector2D_Angle(Vector2D vec);
	float Vector2D_AngleWithVector(Vector2D vec, Vector2D other);
	Vector2D Vector2D_Rotate(Vector2D vec, float angle);
	Vector2D Vector2D_Add(Vector2D a, Vector2D b);
	Vector2D Vector2D_Subtract(Vector2D a, Vector2D b);
	Vector2D Vector2D_Dot(Vector2D a, Vector2D b);
	Vector2D Vector2D_Cross(Vector2D a, Vector2D b);
	Vector2D Vector2D_Mult(Vector2D a, float b);


	//String
	bool String_Equals(std::string a, std::string b);
	std::string String_Concatenate(std::string a, std::string b);
	std::string String_Substring(std::string a, int begin, int end);
	std::string String_Begining(std::string a, int b);
	std::string String_End(std::string a, int b);
	std::string String_Trim(std::string a, std::string values);
	std::string String_TrimBlanks(std::string a);
	char String_GetLetter(std::string a, int b);
	int String_Find(std::string a, char c);


	//Misc
	void Open_URL(std::string url);

private:

	ScriptFunctionality();

	};
}





