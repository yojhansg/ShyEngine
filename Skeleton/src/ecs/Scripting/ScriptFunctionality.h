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

	EditorManager ScriptFunctionality : public Utilities::Singleton<ScriptFunctionality>{

		friend Singleton<ScriptFunctionality>;

public:

	~ScriptFunctionality();

publish:

	void Print(cVariable val);

	ECS::Entity* Entity();

	//TODO: entity_Destroy, entity_Create
	std::string Entity_Name(ECS::Entity* ent);
	std::string Entity_CurrentName();

	//TODO: no me convence el nombre
	void Entity_Event(ECS::Entity* ent, cstring name);
	void GlobalEvent(cstring name);

	std::string Graph();


	//TODO: plus one, minue one
	//Math
	float Math_Add(float a, float b);
	float Math_Subtract(float a, float b);
	float Math_Multiply(float a, float b);
	float Math_Divide(float a, float b);
	float Math_Power(float a, float b);
	float Math_Root(float a, float b);
	float Math_Max(float a, float b);
	float Math_Min(float a, float b);
	float Math_PlusOne(float a);
	float Math_MinusOne(float a);

	//Logic
	//TODO: cambiar equals para que funciones con una variable
	bool Logic_Equals(float a, float b);
	bool Logic_NotEquals(float a, float b);
	bool Logic_Lesser(float a, float b);
	bool Logic_Greater(float a, float b);
	bool Logic_LesserOrEqual(float a, float b);
	bool Logic_GreaterOrEqual(float a, float b);
	bool Logic_And(bool a, bool b);
	bool Logic_Or(bool a, bool b);
	bool Logic_Negate(bool b);

	//TODO: constructoras predeterminadas vector2d: left, up, one, zero...
	Vector2D Vector2D_Create(float x, float y);
	float Vector2D_Magnitude(cVector2D vec);
	float Vector2D_X(cVector2D vec);
	float Vector2D_Y(cVector2D vec);
	Vector2D Vector2D_Normalize(cVector2D vec);
	float Vector2D_Angle(cVector2D vec);
	float Vector2D_AngleWithVector(cVector2D vec, cVector2D other);
	Vector2D Vector2D_Rotate(cVector2D vec, float angle);
	Vector2D Vector2D_Add(cVector2D a, cVector2D b);
	Vector2D Vector2D_Subtract(cVector2D a, cVector2D b);
	Vector2D Vector2D_Dot(cVector2D a, cVector2D b);
	Vector2D Vector2D_Cross(cVector2D a, cVector2D b);
	Vector2D Vector2D_Multiply(cVector2D a, float b);


	//Todo: cambiar el nombre de string a text
	bool String_Equals(cstring a, cstring b);
	std::string String_Concatenate(cstring a, cstring b);
	std::string String_Substring(cstring a, int begin, int end);
	std::string String_Begining(cstring a, int b);
	std::string String_End(cstring a, int b);
	std::string String_Trim(cstring a, cstring values);
	std::string String_TrimBlanks(cstring a);
	char String_GetLetter(cstring a, int b);
	int String_Find(cstring a, char c);
	std::string String_ToString(cVariable variable);
	std::string String_LeadingZeros(int number, int zeroCount);
	std::string String_RemoveDecimals(float number);
	//Attributes

	void Attribute_Set(cstring name, cVariable val);
	Scripting::Variable Attribute_Get(cstring name);

	void Attribute_SetLocal(cstring name, cVariable val);
	Scripting::Variable Attribute_GetLocal(cstring name);

	void Attribute_SetGlobal(cstring name, cVariable val);
	Scripting::Variable Attribute_GetGlobal(cstring name);

	//TODO: set local -> Poder acceder a otra entidad
	//TODO: englobal en espacio de nombres Set
	
	//Current time
	int RealTime_Now();
	std::string RealTime_WeekDay(int time);
	std::string RealTime_ShortWeekDay(int time);

	std::string RealTime_Month(int time);
	std::string RealTime_ShortMonth(int time);

	int RealTime_DayOfWeekIndex(int time);
	int RealTime_MonthIndex(int time);

	int RealTime_MonthDay(int time);
	int RealTime_Year(int time);

	int RealTime_Hours(int time);
	int RealTime_Minutes(int time);
	int RealTime_Seconds(int time);

	std::string RealTime_Since(int time, int now);
	std::string RealTime_HourTime(int time);
	std::string RealTime_PreciseHourTime(int time);
	std::string RealTime_TimeStamp(int time);
	std::string RealTime_Date(int time);

	Utilities::Vector2D Camera_GetPosition();
	void Camera_SetPosition(cVector2D newPosition);
	float Camera_GetScale();
	void Camera_SetScale(float newScale);

	//Misc
	void OpenURL(cstring url);

	//TODO: Collections

	float Random_UnitValue();
	float Random_Between(float a, float b);
	Utilities::Vector2D Random_UnitVector();
	Utilities::Vector2D Random_ScaledVector(float val);

private:

	int GetRealTime(int time);

	ScriptFunctionality();

	};
}





