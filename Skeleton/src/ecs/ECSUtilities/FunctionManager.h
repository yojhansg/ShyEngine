#pragma once

#include <map>
#include <string>
#include <vector>
#include "Scripting/Variable.h"



#define ECSfunc_Version 1.0
//Creation time : Mon Mar 27 22:32:54 2023

typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager{
public: 
	static void CreateFunctionMap(std::map<std::string, CallableFunction>& map);
};



Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_isTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_getPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_setRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_translate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_rotate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_scale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_keyDownEvent(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_keyUpEvent(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_print(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_debugDraw(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_enableDebugDraw(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Print_Number(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Print_Vector2D(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Add(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Subtract(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Multiply(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Divide(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Power(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Root(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Equals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_NotEquals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Lesser(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Greater(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_LesserOrEqual(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_GreaterOrEqual(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_And(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Or(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Negate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Create(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Magnitude(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_X(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Y(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Normalize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Angle(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_AngleWithVector(std::vector<Scripting::Variable>const& vec);
