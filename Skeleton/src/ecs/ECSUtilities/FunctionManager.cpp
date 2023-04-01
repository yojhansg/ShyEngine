
#include "FunctionManager.h"
#include "Entity.h"

//Creation time: Sat Apr  1 20:23:04 2023
#include <Components/BoxBody.h>
#include <Components/ChainBody.h>
#include <Components/CircleBody.h>
#include <Components/EdgeBody.h>
#include <Components/Image.h>
#include <Components/OverlayElement.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>
#include <Components/PhysicBody.h>
#include <Components/Transform.h>
#include <SceneManager.h>
#include <ScriptFunctionality.h>
#include <InputManager.h>
#include <PhysicsManager.h>


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
	map.emplace("OverlayElement_GetPlacement",OverlayElement_GetPlacement);
	map.emplace("OverlayElement_SetPositioned",OverlayElement_SetPositioned);
	map.emplace("OverlayElement_SetStreched",OverlayElement_SetStreched);
	map.emplace("OverlayElement_Move",OverlayElement_Move);
	map.emplace("OverlayElement_MoveTo",OverlayElement_MoveTo);
	map.emplace("OverlayElement_SetSize",OverlayElement_SetSize);
	map.emplace("OverlayElement_GetPosition",OverlayElement_GetPosition);
	map.emplace("OverlayElement_GetSize",OverlayElement_GetSize);
	map.emplace("OverlayElement_GetTop",OverlayElement_GetTop);
	map.emplace("OverlayElement_GetLeft",OverlayElement_GetLeft);
	map.emplace("OverlayElement_GetRight",OverlayElement_GetRight);
	map.emplace("OverlayElement_GetBottom",OverlayElement_GetBottom);
	map.emplace("OverlayElement_SetTop",OverlayElement_SetTop);
	map.emplace("OverlayElement_SetLeft",OverlayElement_SetLeft);
	map.emplace("OverlayElement_SetRight",OverlayElement_SetRight);
	map.emplace("OverlayElement_SetBottom",OverlayElement_SetBottom);
	map.emplace("OverlayElement_GetAnchor",OverlayElement_GetAnchor);
	map.emplace("OverlayElement_SetAnchor",OverlayElement_SetAnchor);
	map.emplace("OverlayElement_SetAnchorCenter",OverlayElement_SetAnchorCenter);
	map.emplace("OverlayElement_SetAnchorTopLeft",OverlayElement_SetAnchorTopLeft);
	map.emplace("OverlayElement_SetAnchorTopRight",OverlayElement_SetAnchorTopRight);
	map.emplace("OverlayElement_SetAnchorBottomLeft",OverlayElement_SetAnchorBottomLeft);
	map.emplace("OverlayElement_SetAnchorBottomRight",OverlayElement_SetAnchorBottomRight);
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
	map.emplace("Transform_setPosition",Transform_setPosition);
	map.emplace("Transform_setScale",Transform_setScale);
	map.emplace("Transform_setRotation",Transform_setRotation);
	map.emplace("Transform_translate",Transform_translate);
	map.emplace("Transform_rotate",Transform_rotate);
	map.emplace("Transform_scale",Transform_scale);
	map.emplace("InputManager_keyDownEvent",InputManager_keyDownEvent);
	map.emplace("InputManager_keyUpEvent",InputManager_keyUpEvent);
	map.emplace("PhysicsManager_debugDraw",PhysicsManager_debugDraw);
	map.emplace("PhysicsManager_enableDebugDraw",PhysicsManager_enableDebugDraw);
	map.emplace("PhysicsManager_handleBodies",PhysicsManager_handleBodies);
	map.emplace("SceneManager_resetScene",SceneManager_resetScene);
	map.emplace("ScriptFunctionality_GameObject",ScriptFunctionality_GameObject);
	map.emplace("ScriptFunctionality_Print",ScriptFunctionality_Print);
	map.emplace("ScriptFunctionality_Math_Add",ScriptFunctionality_Math_Add);
	map.emplace("ScriptFunctionality_Math_Subtract",ScriptFunctionality_Math_Subtract);
	map.emplace("ScriptFunctionality_Math_Multiply",ScriptFunctionality_Math_Multiply);
	map.emplace("ScriptFunctionality_Math_Divide",ScriptFunctionality_Math_Divide);
	map.emplace("ScriptFunctionality_Math_Power",ScriptFunctionality_Math_Power);
	map.emplace("ScriptFunctionality_Math_Root",ScriptFunctionality_Math_Root);
	map.emplace("ScriptFunctionality_Logic_Equals",ScriptFunctionality_Logic_Equals);
	map.emplace("ScriptFunctionality_Logic_NotEquals",ScriptFunctionality_Logic_NotEquals);
	map.emplace("ScriptFunctionality_Logic_Lesser",ScriptFunctionality_Logic_Lesser);
	map.emplace("ScriptFunctionality_Logic_Greater",ScriptFunctionality_Logic_Greater);
	map.emplace("ScriptFunctionality_Logic_LesserOrEqual",ScriptFunctionality_Logic_LesserOrEqual);
	map.emplace("ScriptFunctionality_Logic_GreaterOrEqual",ScriptFunctionality_Logic_GreaterOrEqual);
	map.emplace("ScriptFunctionality_Logic_And",ScriptFunctionality_Logic_And);
	map.emplace("ScriptFunctionality_Logic_Or",ScriptFunctionality_Logic_Or);
	map.emplace("ScriptFunctionality_Logic_Negate",ScriptFunctionality_Logic_Negate);
	map.emplace("ScriptFunctionality_Vector2D_Create",ScriptFunctionality_Vector2D_Create);
	map.emplace("ScriptFunctionality_Vector2D_Magnitude",ScriptFunctionality_Vector2D_Magnitude);
	map.emplace("ScriptFunctionality_Vector2D_X",ScriptFunctionality_Vector2D_X);
	map.emplace("ScriptFunctionality_Vector2D_Y",ScriptFunctionality_Vector2D_Y);
	map.emplace("ScriptFunctionality_Vector2D_Normalize",ScriptFunctionality_Vector2D_Normalize);
	map.emplace("ScriptFunctionality_Vector2D_Angle",ScriptFunctionality_Vector2D_Angle);
	map.emplace("ScriptFunctionality_Vector2D_AngleWithVector",ScriptFunctionality_Vector2D_AngleWithVector);
	map.emplace("ScriptFunctionality_String_Concatenate",ScriptFunctionality_String_Concatenate);
	map.emplace("ScriptFunctionality_String_Substring",ScriptFunctionality_String_Substring);
	map.emplace("ScriptFunctionality_String_Begining",ScriptFunctionality_String_Begining);
	map.emplace("ScriptFunctionality_String_End",ScriptFunctionality_String_End);
	map.emplace("ScriptFunctionality_String_Trim",ScriptFunctionality_String_Trim);
	map.emplace("ScriptFunctionality_String_TrimBlanks",ScriptFunctionality_String_TrimBlanks);
	map.emplace("ScriptFunctionality_String_GetLetter",ScriptFunctionality_String_GetLetter);
	map.emplace("ScriptFunctionality_String_Find",ScriptFunctionality_String_Find);

};
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	int ret = self->getTextureWidth();
	return ret;
}
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	int ret = self->getTextureHeight();
	return ret;
}
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	self->setSrcRect(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	self->setFlipX(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	self->setFlipY(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec){
	Image* self = vec[0].value.entity->getComponent<Image>();
	self->setRotaionPoint(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_GetPlacement(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	int ret = self->GetPlacement();
	return ret;
}
Scripting::Variable OverlayElement_SetPositioned(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetPositioned(vec[1].vector, vec[2].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetStreched(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetStreched(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_Move(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->Move(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_MoveTo(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->MoveTo(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetSize(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetSize(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_GetPosition(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	Utilities::Vector2D ret = self->GetPosition();
	return ret;
}
Scripting::Variable OverlayElement_GetSize(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	Utilities::Vector2D ret = self->GetSize();
	return ret;
}
Scripting::Variable OverlayElement_GetTop(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	int ret = self->GetTop();
	return ret;
}
Scripting::Variable OverlayElement_GetLeft(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	int ret = self->GetLeft();
	return ret;
}
Scripting::Variable OverlayElement_GetRight(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	int ret = self->GetRight();
	return ret;
}
Scripting::Variable OverlayElement_GetBottom(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	int ret = self->GetBottom();
	return ret;
}
Scripting::Variable OverlayElement_SetTop(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetTop(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetLeft(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetLeft(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetRight(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetRight(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetBottom(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetBottom(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_GetAnchor(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	Utilities::Vector2D ret = self->GetAnchor();
	return ret;
}
Scripting::Variable OverlayElement_SetAnchor(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchor(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetAnchorCenter(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchorCenter();
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetAnchorTopLeft(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchorTopLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetAnchorTopRight(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchorTopRight();
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetAnchorBottomLeft(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchorBottomLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayElement_SetAnchorBottomRight(std::vector<Scripting::Variable>const& vec){
	OverlayElement* self = vec[0].value.entity->getComponent<OverlayElement>();
	self->SetAnchorBottomRight();
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayImage_SetTexture(std::vector<Scripting::Variable>const& vec){
	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	self->SetTexture(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayImage_GetTexture(std::vector<Scripting::Variable>const& vec){
	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	std::string ret = self->GetTexture();
	return ret;
}
Scripting::Variable OverlayText_GetFit(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	int ret = self->GetFit();
	return ret;
}
Scripting::Variable OverlayText_SetFit(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetFit(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetVerticalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	int ret = self->GetVerticalAlignment();
	return ret;
}
Scripting::Variable OverlayText_GetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	int ret = self->GetHorizontalAlignment();
	return ret;
}
Scripting::Variable OverlayText_SetVerticalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetVerticalAlignment(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_SetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetHorizontalAlignment(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetFont(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	std::string ret = self->GetFont();
	return ret;
}
Scripting::Variable OverlayText_SetFont(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetFont(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetText(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	std::string ret = self->GetText();
	return ret;
}
Scripting::Variable OverlayText_SetText(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetText(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetPointSize(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	int ret = self->GetPointSize();
	return ret;
}
Scripting::Variable OverlayText_SetPointSize(std::vector<Scripting::Variable>const& vec){
	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	self->SetPointSize(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_setTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	self->setTrigger(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_isTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	bool ret = self->isTrigger();
	return ret;
}
Scripting::Variable PhysicBody_setFriction(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	self->setFriction(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getFriction(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	float ret = self->getFriction();
	return ret;
}
Scripting::Variable PhysicBody_setBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	self->setBounciness(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	float ret = self->getBounciness();
	return ret;
}
Scripting::Variable PhysicBody_setLinearVelocity(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	self->setLinearVelocity(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getLinearVelocity(std::vector<Scripting::Variable>const& vec){
	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	Vector2D ret = self->getLinearVelocity();
	return ret;
}
Scripting::Variable Transform_getPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	Utilities::Vector2D ret = self->getPosition();
	return ret;
}
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	Utilities::Vector2D ret = self->getScale();
	return ret;
}
Scripting::Variable Transform_setPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	self->setPosition(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	self->setScale(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setRotation(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	self->setRotation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_translate(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	self->translate(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_rotate(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	self->rotate(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_scale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
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
Scripting::Variable SceneManager_resetScene(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->resetScene();
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_GameObject(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	ECS::Entity* ret = manager->GameObject();
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
