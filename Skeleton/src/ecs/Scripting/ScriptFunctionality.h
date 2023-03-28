#pragma once

#include "Singleton.h"
#include "EditorExport.h"
#include "Vector2D.h"
#include "Scripting/Variable.h"

namespace ECS {

	class Entity;
}

using Utilities::Vector2D;


namespace Scripting {

	EditorManager ScriptFunctionality : public Utilities::Singleton<ScriptFunctionality>{

		friend Singleton<ScriptFunctionality>;

public:

	~ScriptFunctionality();


publish:

	ECS::Entity* GameObject();
	//print
	void Print(Scripting::Variable val);
	void Print_Number(float n);
	void Print_Vector2D(Vector2D n);


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
	Vector2D Vector2D_Create(float x, float y);
	float Vector2D_Magnitude(Vector2D vec);
	float Vector2D_X(Vector2D vec);
	float Vector2D_Y(Vector2D vec);
	Vector2D Vector2D_Normalize(Vector2D vec);
	float Vector2D_Angle(Vector2D vec);
	float Vector2D_AngleWithVector(Vector2D vec, Vector2D other);
private:

	ScriptFunctionality();

	};
}





