
#include "FunctionManager.h"
#include "Entity.h"
#include "ConsoleManager.h"

//Creation time: Sat Apr  8 00:02:37 2023

#define _Console(info, value) Console::Output::PrintError( info , value )
#define _ErrorInfo(entity, script, function, title) entity + ": " + script + ": " + function + ": " + title + ": "
#define _DebugError(entity, script, function, title, error) _Console(_ErrorInfo(entity, script, function, title), error)

#define _ComponentErrorMessage(entityError, missingComponent) "The entity <" + entityError + "> does not contain <" + missingComponent + "> component"
#define _NullErrorMessage(i, expected) "Empty input given to input(" + i + "). Expected type: <" + expected + ">"
#define _InvalidInputErrorMessage(i, expected, given) "Expected <" + expected + "> but received <" + given + "> for input(" + i + ")"

#define DebugComponentError(entity, script, function, entityError, missingComponent) _DebugError(entity, script, function, "Entity error", _ComponentErrorMessage(entityError, #missingComponent))
#define DebugNullError(entity, script, function, i, expected) _DebugError(entity, script, function, "Empty value", _NullErrorMessage(i, expected))
#define DebugInvalidInputError(entity, script, function, i, expected, given) _DebugError(entity, script, function, "Invalid input", _InvalidInputErrorMessage(i, expected, given))
#include <Components/BoxBody.h>
#include <Components/ChainBody.h>
#include <Components/CircleBody.h>
#include <Components/EdgeBody.h>
#include <Components/Image.h>
#include <Components/Overlay.h>
#include <Components/OverlayButton.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>
#include <Components/PhysicBody.h>
#include <Components/Transform.h>
#include <Save.h>
#include <SceneManager.h>
#include <ScriptFunctionality.h>
#include <InputManager.h>
#include <PhysicsManager.h>
#include <EngineTime.h>


using namespace ECS;
using namespace Physics;
using namespace Input;
using namespace Scripting;



void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

	map.emplace("Image_getTextureWidth",Image_getTextureWidth);
	map.emplace("Image_getTextureHeight",Image_getTextureHeight);
	map.emplace("Image_setSrcRect",Image_setSrcRect);
	map.emplace("Image_setFlipX",Image_setFlipX);
	map.emplace("Image_setFlipY",Image_setFlipY);
	map.emplace("Image_setRotaionPoint",Image_setRotaionPoint);
	map.emplace("Image_scaledSize",Image_scaledSize);
	map.emplace("Overlay_GetPlacement",Overlay_GetPlacement);
	map.emplace("Overlay_SetPositioned",Overlay_SetPositioned);
	map.emplace("Overlay_SetStreched",Overlay_SetStreched);
	map.emplace("Overlay_Move",Overlay_Move);
	map.emplace("Overlay_MoveTo",Overlay_MoveTo);
	map.emplace("Overlay_SetSize",Overlay_SetSize);
	map.emplace("Overlay_GetPosition",Overlay_GetPosition);
	map.emplace("Overlay_GetSize",Overlay_GetSize);
	map.emplace("Overlay_GetTop",Overlay_GetTop);
	map.emplace("Overlay_GetLeft",Overlay_GetLeft);
	map.emplace("Overlay_GetRight",Overlay_GetRight);
	map.emplace("Overlay_GetBottom",Overlay_GetBottom);
	map.emplace("Overlay_SetTop",Overlay_SetTop);
	map.emplace("Overlay_SetLeft",Overlay_SetLeft);
	map.emplace("Overlay_SetRight",Overlay_SetRight);
	map.emplace("Overlay_SetBottom",Overlay_SetBottom);
	map.emplace("Overlay_GetAnchor",Overlay_GetAnchor);
	map.emplace("Overlay_SetAnchor",Overlay_SetAnchor);
	map.emplace("Overlay_SetAnchorCenter",Overlay_SetAnchorCenter);
	map.emplace("Overlay_SetAnchorTopLeft",Overlay_SetAnchorTopLeft);
	map.emplace("Overlay_SetAnchorTopRight",Overlay_SetAnchorTopRight);
	map.emplace("Overlay_SetAnchorBottomLeft",Overlay_SetAnchorBottomLeft);
	map.emplace("Overlay_SetAnchorBottomRight",Overlay_SetAnchorBottomRight);
	map.emplace("Overlay_GetColor",Overlay_GetColor);
	map.emplace("Overlay_SetColor",Overlay_SetColor);
	map.emplace("Overlay_GetRenderScale",Overlay_GetRenderScale);
	map.emplace("Overlay_SetRenderScale",Overlay_SetRenderScale);
	map.emplace("Overlay_ResetRenderScale",Overlay_ResetRenderScale);
	map.emplace("Overlay_SetInteractable",Overlay_SetInteractable);
	map.emplace("Overlay_IsInteractable",Overlay_IsInteractable);
	map.emplace("OverlayImage_SetTexture",OverlayImage_SetTexture);
	map.emplace("OverlayImage_GetTexture",OverlayImage_GetTexture);
	map.emplace("OverlayText_GetFit",OverlayText_GetFit);
	map.emplace("OverlayText_SetFit",OverlayText_SetFit);
	map.emplace("OverlayText_GetVerticalAlignment",OverlayText_GetVerticalAlignment);
	map.emplace("OverlayText_GetHorizontalAlignment",OverlayText_GetHorizontalAlignment);
	map.emplace("OverlayText_SetVerticalAlignment",OverlayText_SetVerticalAlignment);
	map.emplace("OverlayText_SetHorizontalAlignment",OverlayText_SetHorizontalAlignment);
	map.emplace("OverlayText_GetFont",OverlayText_GetFont);
	map.emplace("OverlayText_SetFont",OverlayText_SetFont);
	map.emplace("OverlayText_GetText",OverlayText_GetText);
	map.emplace("OverlayText_SetText",OverlayText_SetText);
	map.emplace("OverlayText_GetPointSize",OverlayText_GetPointSize);
	map.emplace("OverlayText_SetPointSize",OverlayText_SetPointSize);
	map.emplace("PhysicBody_setTrigger",PhysicBody_setTrigger);
	map.emplace("PhysicBody_isTrigger",PhysicBody_isTrigger);
	map.emplace("PhysicBody_setFriction",PhysicBody_setFriction);
	map.emplace("PhysicBody_getFriction",PhysicBody_getFriction);
	map.emplace("PhysicBody_setBounciness",PhysicBody_setBounciness);
	map.emplace("PhysicBody_getBounciness",PhysicBody_getBounciness);
	map.emplace("PhysicBody_setLinearVelocity",PhysicBody_setLinearVelocity);
	map.emplace("PhysicBody_getLinearVelocity",PhysicBody_getLinearVelocity);
	map.emplace("Transform_getPosition",Transform_getPosition);
	map.emplace("Transform_getScale",Transform_getScale);
	map.emplace("Transform_getRotation",Transform_getRotation);
	map.emplace("Transform_setPosition",Transform_setPosition);
	map.emplace("Transform_setPositionX",Transform_setPositionX);
	map.emplace("Transform_setPositionY",Transform_setPositionY);
	map.emplace("Transform_setScale",Transform_setScale);
	map.emplace("Transform_setScaleX",Transform_setScaleX);
	map.emplace("Transform_setScaleY",Transform_setScaleY);
	map.emplace("Transform_setRotation",Transform_setRotation);
	map.emplace("Transform_translate",Transform_translate);
	map.emplace("Transform_translateX",Transform_translateX);
	map.emplace("Transform_translateY",Transform_translateY);
	map.emplace("Transform_rotate",Transform_rotate);
	map.emplace("Transform_scale",Transform_scale);
	map.emplace("InputManager_keyDownEvent",InputManager_keyDownEvent);
	map.emplace("InputManager_keyUpEvent",InputManager_keyUpEvent);
	map.emplace("PhysicsManager_debugDraw",PhysicsManager_debugDraw);
	map.emplace("PhysicsManager_enableDebugDraw",PhysicsManager_enableDebugDraw);
	map.emplace("PhysicsManager_handleBodies",PhysicsManager_handleBodies);
	map.emplace("SaveManager_SaveAll",SaveManager_SaveAll);
	map.emplace("SaveManager_Save",SaveManager_Save);
	map.emplace("SaveManager_Load",SaveManager_Load);
	map.emplace("SaveManager_SetSlot",SaveManager_SetSlot);
	map.emplace("SaveManager_GetSlot",SaveManager_GetSlot);
	map.emplace("SaveManager_Set",SaveManager_Set);
	map.emplace("SaveManager_Get",SaveManager_Get);
	map.emplace("SaveManager_ClearSlot",SaveManager_ClearSlot);
	map.emplace("SaveManager_Exists",SaveManager_Exists);
	map.emplace("SaveManager_DeleteSave",SaveManager_DeleteSave);
	map.emplace("SceneManager_ChangeScene",SceneManager_ChangeScene);
	map.emplace("SceneManager_ResetScene",SceneManager_ResetScene);
	map.emplace("SceneManager_EndGame",SceneManager_EndGame);
	map.emplace("SceneManager_getNumberOfScenes",SceneManager_getNumberOfScenes);
	map.emplace("SceneManager_GetCurrentScenePath",SceneManager_GetCurrentScenePath);
	map.emplace("Entity",ScriptFunctionality_Entity);
	map.emplace("EntityName",ScriptFunctionality_EntityName);
	map.emplace("Script",ScriptFunctionality_Script);
	map.emplace("Print",ScriptFunctionality_Print);
	map.emplace("Math_Add",ScriptFunctionality_Math_Add);
	map.emplace("Math_Subtract",ScriptFunctionality_Math_Subtract);
	map.emplace("Math_Multiply",ScriptFunctionality_Math_Multiply);
	map.emplace("Math_Divide",ScriptFunctionality_Math_Divide);
	map.emplace("Math_Power",ScriptFunctionality_Math_Power);
	map.emplace("Math_Root",ScriptFunctionality_Math_Root);
	map.emplace("Logic_Equals",ScriptFunctionality_Logic_Equals);
	map.emplace("Logic_NotEquals",ScriptFunctionality_Logic_NotEquals);
	map.emplace("Logic_Lesser",ScriptFunctionality_Logic_Lesser);
	map.emplace("Logic_Greater",ScriptFunctionality_Logic_Greater);
	map.emplace("Logic_LesserOrEqual",ScriptFunctionality_Logic_LesserOrEqual);
	map.emplace("Logic_GreaterOrEqual",ScriptFunctionality_Logic_GreaterOrEqual);
	map.emplace("Logic_And",ScriptFunctionality_Logic_And);
	map.emplace("Logic_Or",ScriptFunctionality_Logic_Or);
	map.emplace("Logic_Negate",ScriptFunctionality_Logic_Negate);
	map.emplace("Vector2D_Create",ScriptFunctionality_Vector2D_Create);
	map.emplace("Vector2D_Magnitude",ScriptFunctionality_Vector2D_Magnitude);
	map.emplace("Vector2D_X",ScriptFunctionality_Vector2D_X);
	map.emplace("Vector2D_Y",ScriptFunctionality_Vector2D_Y);
	map.emplace("Vector2D_Normalize",ScriptFunctionality_Vector2D_Normalize);
	map.emplace("Vector2D_Angle",ScriptFunctionality_Vector2D_Angle);
	map.emplace("Vector2D_AngleWithVector",ScriptFunctionality_Vector2D_AngleWithVector);
	map.emplace("Vector2D_Rotate",ScriptFunctionality_Vector2D_Rotate);
	map.emplace("Vector2D_Add",ScriptFunctionality_Vector2D_Add);
	map.emplace("Vector2D_Subtract",ScriptFunctionality_Vector2D_Subtract);
	map.emplace("Vector2D_Dot",ScriptFunctionality_Vector2D_Dot);
	map.emplace("Vector2D_Cross",ScriptFunctionality_Vector2D_Cross);
	map.emplace("Vector2D_Mult",ScriptFunctionality_Vector2D_Mult);
	map.emplace("String_Equals",ScriptFunctionality_String_Equals);
	map.emplace("String_Concatenate",ScriptFunctionality_String_Concatenate);
	map.emplace("String_Substring",ScriptFunctionality_String_Substring);
	map.emplace("String_Begining",ScriptFunctionality_String_Begining);
	map.emplace("String_End",ScriptFunctionality_String_End);
	map.emplace("String_Trim",ScriptFunctionality_String_Trim);
	map.emplace("String_TrimBlanks",ScriptFunctionality_String_TrimBlanks);
	map.emplace("String_GetLetter",ScriptFunctionality_String_GetLetter);
	map.emplace("String_Find",ScriptFunctionality_String_Find);
	map.emplace("String_ToString",ScriptFunctionality_String_ToString);
	map.emplace("String_LeadingZeros",ScriptFunctionality_String_LeadingZeros);
	map.emplace("Set",ScriptFunctionality_Set);
	map.emplace("Get",ScriptFunctionality_Get);
	map.emplace("SetLocal",ScriptFunctionality_SetLocal);
	map.emplace("GetLocal",ScriptFunctionality_GetLocal);
	map.emplace("SetGlobal",ScriptFunctionality_SetGlobal);
	map.emplace("GetGlobal",ScriptFunctionality_GetGlobal);
	map.emplace("Time_Now",ScriptFunctionality_Time_Now);
	map.emplace("Time_WeekDay",ScriptFunctionality_Time_WeekDay);
	map.emplace("Time_ShortWeekDay",ScriptFunctionality_Time_ShortWeekDay);
	map.emplace("Time_Month",ScriptFunctionality_Time_Month);
	map.emplace("Time_ShortMonth",ScriptFunctionality_Time_ShortMonth);
	map.emplace("Time_DayOfWeekIndex",ScriptFunctionality_Time_DayOfWeekIndex);
	map.emplace("Time_MonthIndex",ScriptFunctionality_Time_MonthIndex);
	map.emplace("Time_MonthDay",ScriptFunctionality_Time_MonthDay);
	map.emplace("Time_Year",ScriptFunctionality_Time_Year);
	map.emplace("Time_Hours",ScriptFunctionality_Time_Hours);
	map.emplace("Time_Minutes",ScriptFunctionality_Time_Minutes);
	map.emplace("Time_Seconds",ScriptFunctionality_Time_Seconds);
	map.emplace("Time_TimeHHMM",ScriptFunctionality_Time_TimeHHMM);
	map.emplace("Time_TimeHHMMSS",ScriptFunctionality_Time_TimeHHMMSS);
	map.emplace("Time_TimeStamp",ScriptFunctionality_Time_TimeStamp);
	map.emplace("Time_DDMMYY",ScriptFunctionality_Time_DDMMYY);
	map.emplace("Open_URL",ScriptFunctionality_Open_URL);
	map.emplace("Time_GetTimeSinceBegining",Time_GetTimeSinceBegining);
	map.emplace("Time_GetTimeSinceBeginingMilliseconds",Time_GetTimeSinceBeginingMilliseconds);
	map.emplace("Time_GetDeltaTime",Time_GetDeltaTime);
	map.emplace("Time_GetFrameRate",Time_GetFrameRate);
	map.emplace("Time_GetPhysicsDeltaTime",Time_GetPhysicsDeltaTime);

};
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_getTextureWidth", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	int ret = self->getTextureWidth();
	return ret;
}
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_getTextureHeight", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	int ret = self->getTextureHeight();
	return ret;
}
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_setSrcRect", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setSrcRect(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_setFlipX", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setFlipX(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_setFlipY", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setFlipY(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_setRotaionPoint", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setRotaionPoint(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_scaledSize(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Image_scaledSize", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->scaledSize();
	return ret;
}
Scripting::Variable Overlay_GetPlacement(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetPlacement", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetPlacement();
	return ret;
}
Scripting::Variable Overlay_SetPositioned(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetPositioned", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetPositioned(vec[1].vector, vec[2].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetStreched(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetStreched", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetStreched(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_Move(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_Move", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->Move(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_MoveTo(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_MoveTo", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->MoveTo(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetSize(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetSize", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetSize(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetPosition(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetPosition", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetPosition();
	return ret;
}
Scripting::Variable Overlay_GetSize(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetSize", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetSize();
	return ret;
}
Scripting::Variable Overlay_GetTop(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetTop", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetTop();
	return ret;
}
Scripting::Variable Overlay_GetLeft(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetLeft();
	return ret;
}
Scripting::Variable Overlay_GetRight(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetRight();
	return ret;
}
Scripting::Variable Overlay_GetBottom(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetBottom", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetBottom();
	return ret;
}
Scripting::Variable Overlay_SetTop(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetTop", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetTop(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetLeft(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetLeft(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetRight(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetRight(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetBottom(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetBottom", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetBottom(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetAnchor(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetAnchor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetAnchor();
	return ret;
}
Scripting::Variable Overlay_SetAnchor(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchor(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorCenter(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchorCenter", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorCenter();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorTopLeft(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchorTopLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorTopLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorTopRight(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchorTopRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorTopRight();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorBottomLeft(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchorBottomLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorBottomLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorBottomRight(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetAnchorBottomRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorBottomRight();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetColor(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetColor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Color ret = self->GetColor();
	return ret;
}
Scripting::Variable Overlay_SetColor(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetColor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetColor(vec[1].value.color);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetRenderScale(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_GetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	float ret = self->GetRenderScale();
	return ret;
}
Scripting::Variable Overlay_SetRenderScale(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetRenderScale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_ResetRenderScale(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_ResetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->ResetRenderScale();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetInteractable(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_SetInteractable", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetInteractable(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_IsInteractable(std::vector<Scripting::Variable>const& vec){
	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Overlay_IsInteractable", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	bool ret = self->IsInteractable();
	return ret;
}
Scripting::Variable OverlayImage_SetTexture(std::vector<Scripting::Variable>const& vec){
	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayImage_SetTexture", vec[0].value.entity->getEntityName(), OverlayImage);
		return Scripting::Variable::Null();
	}
	self->SetTexture(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayImage_GetTexture(std::vector<Scripting::Variable>const& vec){
	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayImage_GetTexture", vec[0].value.entity->getEntityName(), OverlayImage);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetTexture();
	return ret;
}
Scripting::Variable OverlayText_GetFit(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetFit", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	int ret = self->GetFit();
	return ret;
}
Scripting::Variable OverlayText_SetFit(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetFit", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetFit(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetVerticalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetVerticalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	int ret = self->GetVerticalAlignment();
	return ret;
}
Scripting::Variable OverlayText_GetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetHorizontalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	int ret = self->GetHorizontalAlignment();
	return ret;
}
Scripting::Variable OverlayText_SetVerticalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetVerticalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetVerticalAlignment(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_SetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetHorizontalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetHorizontalAlignment(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetFont(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetFont", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetFont();
	return ret;
}
Scripting::Variable OverlayText_SetFont(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetFont", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetFont(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetText(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetText", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetText();
	return ret;
}
Scripting::Variable OverlayText_SetText(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetText", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetText(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetPointSize(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_GetPointSize", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	int ret = self->GetPointSize();
	return ret;
}
Scripting::Variable OverlayText_SetPointSize(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "OverlayText_SetPointSize", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetPointSize(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_setTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_setTrigger", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setTrigger(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_isTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_isTrigger", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	bool ret = self->isTrigger();
	return ret;
}
Scripting::Variable PhysicBody_setFriction(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_setFriction", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setFriction(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getFriction(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_getFriction", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getFriction();
	return ret;
}
Scripting::Variable PhysicBody_setBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_setBounciness", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setBounciness(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_getBounciness", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getBounciness();
	return ret;
}
Scripting::Variable PhysicBody_setLinearVelocity(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_setLinearVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setLinearVelocity(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getLinearVelocity(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "PhysicBody_getLinearVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getLinearVelocity();
	return ret;
}
Scripting::Variable Transform_getPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_getPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->getPosition();
	return ret;
}
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_getScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->getScale();
	return ret;
}
Scripting::Variable Transform_getRotation(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_getRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	float ret = self->getRotation();
	return ret;
}
Scripting::Variable Transform_setPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setPosition(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setPositionX(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setPositionX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setPositionX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setPositionY(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setPositionY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setPositionY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setScale(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setScaleX(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setScaleX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setScaleX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setScaleY(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setScaleY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setScaleY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setRotation(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_setRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->setRotation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_translate(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_translate", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->translate(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_translateX(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_translateX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->translateX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_translateY(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_translateY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->translateY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_rotate(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_rotate", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->rotate(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_scale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_EntityName({}).str, ScriptFunctionality_Script({}).str, "Transform_scale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->scale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable InputManager_keyDownEvent(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->keyDownEvent();
	return ret;
}
Scripting::Variable InputManager_keyUpEvent(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->keyUpEvent();
	return ret;
}
Scripting::Variable PhysicsManager_debugDraw(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->debugDraw();
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_enableDebugDraw(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->enableDebugDraw(vec[0].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_handleBodies(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->handleBodies();
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_SaveAll(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->SaveAll();
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Save(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Save(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Load(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Load(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_SetSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->SetSlot(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_GetSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	Scripting::Variable ret = manager->GetSlot(vec[0].str);
	return ret;
}
Scripting::Variable SaveManager_Set(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Set(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Get(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	Scripting::Variable ret = manager->Get(vec[0].str);
	return ret;
}
Scripting::Variable SaveManager_ClearSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->ClearSlot();
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Exists(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	bool ret = manager->Exists(vec[0].value.Float);
	return ret;
}
Scripting::Variable SaveManager_DeleteSave(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->DeleteSave(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_ChangeScene(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->ChangeScene(vec[0].str, vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_ResetScene(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->ResetScene();
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_EndGame(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->EndGame();
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_getNumberOfScenes(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	int ret = manager->getNumberOfScenes();
	return ret;
}
Scripting::Variable SceneManager_GetCurrentScenePath(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	std::string ret = manager->GetCurrentScenePath();
	return ret;
}
Scripting::Variable ScriptFunctionality_Entity(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	ECS::Entity* ret = manager->Entity();
	return ret;
}
Scripting::Variable ScriptFunctionality_EntityName(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->EntityName();
	return ret;
}
Scripting::Variable ScriptFunctionality_Script(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Script();
	return ret;
}
Scripting::Variable ScriptFunctionality_Print(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Print(vec[0]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Math_Add(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Add(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Subtract(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Subtract(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Multiply(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Multiply(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Divide(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Divide(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Power(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Power(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Root(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Root(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Equals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Equals(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_NotEquals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_NotEquals(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Lesser(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Lesser(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Greater(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Greater(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_LesserOrEqual(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_LesserOrEqual(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_GreaterOrEqual(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_GreaterOrEqual(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_And(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_And(vec[0].value.Bool, vec[1].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Or(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Or(vec[0].value.Bool, vec[1].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Negate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Negate(vec[0].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Create(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Create(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Magnitude(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Magnitude(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_X(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_X(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Y(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Y(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Normalize(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Normalize(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Angle(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Angle(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_AngleWithVector(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_AngleWithVector(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Rotate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Rotate(vec[0].vector, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Add(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Add(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Subtract(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Subtract(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Dot(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Dot(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Cross(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Cross(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Mult(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Mult(vec[0].vector, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Equals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->String_Equals(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Concatenate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_Concatenate(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Substring(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_Substring(vec[0].str, vec[1].value.Float, vec[2].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Begining(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_Begining(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_End(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_End(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Trim(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_Trim(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_TrimBlanks(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_TrimBlanks(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_GetLetter(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	char ret = manager->String_GetLetter(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_Find(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->String_Find(vec[0].str, vec[1].value.Char);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_ToString(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_ToString(vec[0]);
	return ret;
}
Scripting::Variable ScriptFunctionality_String_LeadingZeros(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->String_LeadingZeros(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Set(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Set(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Get(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->Get(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_SetLocal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->SetLocal(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_GetLocal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->GetLocal(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_SetGlobal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->SetGlobal(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_GetGlobal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->GetGlobal(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Now(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_Now();
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_WeekDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_WeekDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_ShortWeekDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_ShortWeekDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Month(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_Month(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_ShortMonth(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_ShortMonth(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_DayOfWeekIndex(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_DayOfWeekIndex(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_MonthIndex(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_MonthIndex(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_MonthDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_MonthDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Year(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_Year(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Hours(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_Hours(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Minutes(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_Minutes(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_Seconds(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Time_Seconds(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_TimeHHMM(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_TimeHHMM(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_TimeHHMMSS(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_TimeHHMMSS(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_TimeStamp(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_TimeStamp(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Time_DDMMYY(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Time_DDMMYY(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Open_URL(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Open_URL(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable Time_GetTimeSinceBegining(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetTimeSinceBegining();
	return ret;
}
Scripting::Variable Time_GetTimeSinceBeginingMilliseconds(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetTimeSinceBeginingMilliseconds();
	return ret;
}
Scripting::Variable Time_GetDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetDeltaTime();
	return ret;
}
Scripting::Variable Time_GetFrameRate(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetFrameRate();
	return ret;
}
Scripting::Variable Time_GetPhysicsDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetPhysicsDeltaTime();
	return ret;
}
