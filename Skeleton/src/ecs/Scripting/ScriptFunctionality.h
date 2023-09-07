#pragma once

#include "Singleton.h"
#include "EditorExport.h"
#include "Vector2D.h"
#include "Scripting/Variable.h"

namespace ECS {

	class Entity;
}

using Utilities::Vector2D;
using Utilities::Color;

// TODO: Creo que para las funciones es mejor quitar el nombre script functionality

namespace Scripting {

	EditorManager ScriptFunctionality : public Utilities::Singleton<ScriptFunctionality> {

		friend Singleton<ScriptFunctionality>;

public:

	~ScriptFunctionality();

publish:

	void Print(cVariable val);


	// --------------------- Entity -------------------------

	ECS::Entity* Entity();

	// TODO: entity_Destroy, entity_Create

	void Create_EntityWithTransform(cstring entName);
	void Create_EntityWithOverlay(cstring entName);

	void Destroy_Entity(ECS::Entity* ent);

	//TODO: Name y current name no se entiende lo que es
	std::string Entity_Name(ECS::Entity* ent);
	std::string Entity_CurrentName();

	ECS::Entity* Entity_Collided();

	// TODO: No me convence el nombre
	void Event_EntityEvent(ECS::Entity* ent, cstring name);
	void Event_GlobalEvent(cstring name);

	std::string Graph();



	// ------------------------ Math ------------------------

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
	float Math_Lerp(float a, float b, float t);
	float Math_CubicLerp(float a, float b, float y);

	
	// ----------------------- Logic -----------------------

	// TODO: cambiar equals para que funciones con una variable

	bool Logic_Equals(float a, float b);
	bool Logic_NotEquals(float a, float b);
	bool Logic_Lesser(float a, float b);
	bool Logic_Greater(float a, float b);
	bool Logic_LesserOrEqual(float a, float b);
	bool Logic_GreaterOrEqual(float a, float b);
	bool Logic_And(bool a, bool b);
	bool Logic_Or(bool a, bool b);
	bool Logic_Negate(bool b);



	// ---------------------- Vector2 -----------------------

	float Vector2D_Magnitude(cVector2D vec);
	float Vector2D_X(cVector2D vec);
	float Vector2D_Y(cVector2D vec);
	float Vector2D_Angle(cVector2D vec);
	float Vector2D_AngleWithVector(cVector2D vec, cVector2D other);

	Vector2D Vector2D_Create(float x, float y);
	Vector2D Vector2D_Normalize(cVector2D vec);
	Vector2D Vector2D_Rotate(cVector2D vec, float angle);
	Vector2D Vector2D_Add(cVector2D a, cVector2D b);
	Vector2D Vector2D_Subtract(cVector2D a, cVector2D b);
	Vector2D Vector2D_Dot(cVector2D a, cVector2D b);
	Vector2D Vector2D_Cross(cVector2D a, cVector2D b);
	Vector2D Vector2D_Multiply(cVector2D a, float b);
	Vector2D Vector2D_Lerp(cVector2D a, cVector2D b, float t);
	Vector2D Vector2D_CubicLerp(cVector2D a, cVector2D b, float t);


	Vector2D Vector2D_Up();
	Vector2D Vector2D_Left();
	Vector2D Vector2D_Right();
	Vector2D Vector2D_Down();
	Vector2D Vector2D_One();
	Vector2D Vector2D_Zero();



	// ----------------------- Text --------------------------

	bool Text_Equals(cstring a, cstring b);
	std::string Text_Concatenate(cstring a, cstring b);
	std::string Text_Substring(cstring a, int begin, int end);
	std::string Text_Begining(cstring a, int b);
	std::string Text_End(cstring a, int b);
	std::string Text_Trim(cstring a, cstring values);
	std::string Text_TrimBlanks(cstring a);
	char Text_GetLetter(cstring a, int b);
	int Text_Find(cstring a, char c);
	std::string Text_ToString(cVariable variable);
	std::string Text_LeadingZeros(int number, int zeroCount);
	std::string Text_RemoveDecimals(float number);



	// --------------------- Attributes -----------------------

	void Attribute_Set(cstring name, cVariable val);
	Scripting::Variable Attribute_Get(cstring name);

	void Attribute_SetLocal(cstring name, cVariable val);
	Scripting::Variable Attribute_GetLocal(cstring name);

	void Attribute_SetGlobal(cstring name, cVariable val);
	Scripting::Variable Attribute_GetGlobal(cstring name);

	// TODO: set local -> Poder acceder a otra entidad
	// TODO: englobal en espacio de nombres Set
	


	// ---------------------- Time ---------------------------

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



	// ---------------------- Camera ---------------------

	Utilities::Vector2D Camera_GetPosition();
	void Camera_SetPosition(cVector2D newPosition);
	float Camera_GetScale();
	void Camera_SetScale(float newScale);


	// TODO: Collections
	// void Collection_Create(int size);
	// void Collection_Modify(int idx, cVariable value);
	// cVariable Collection_Peek(int idx);


	
	// -------------------- Random ----------------------

	float Random_UnitValue();
	float Random_NumberBetween(int min, int max);
	float Random_Between(float a, float b);
	float DegreesTo_Radians(float angle);
	float RadiansTo_Degrees(float angle); 
	float Random_AngleBetween(float min, float max);
	Vector2D Random_UnitVector();
	Vector2D Random_ScaledVector(float val);
	Vector2D Random_VectorBetween(cVector2D v1, cVector2D v2);
	Color Random_Color();
	Color Random_ColorBetween(cColor c1, cColor c2);




	// ---------------------- Misc ---------------------

	void OpenURL(cstring url);


	// Mouse position
	Vector2D InputManager_GetMouseWorldPosition();

private:

	int GetRealTime(int time);

	ScriptFunctionality();

	};

}





