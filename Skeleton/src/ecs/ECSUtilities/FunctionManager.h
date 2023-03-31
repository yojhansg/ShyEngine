#pragma once

#include <map>
#include <string>
#include <vector>
#include "Scripting/Variable.h"



#define ECSfunc_Version 1.0
//Creation time : Fri Mar 31 03:15:28 2023

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
Scripting::Variable OverlayElement_GetPlacement(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetPositioned(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetStreched(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_Move(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_MoveTo(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetTop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetBottom(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetTop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetBottom(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_GetAnchor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchorCenter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchorTopLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchorTopRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchorBottomLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayElement_SetAnchorBottomRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayImage_SetTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayImage_GetTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetFit(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetFit(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetVerticalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetHorizontalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetVerticalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetHorizontalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetFont(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetFont(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetText(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetText(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetPointSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetPointSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_isTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_setLinearVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsBody_getLinearVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_Print_GameObject_Name(std::vector<Scripting::Variable>const& vec);
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
Scripting::Variable PhysicsManager_debugDraw(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_enableDebugDraw(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_handleBodies(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_resetScene(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_GameObject(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Print(std::vector<Scripting::Variable>const& vec);
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
Scripting::Variable ScriptFunctionality_String_Concatenate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_Substring(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_Begining(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_End(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_Trim(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_TrimBlanks(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_GetLetter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_String_Find(std::vector<Scripting::Variable>const& vec);
