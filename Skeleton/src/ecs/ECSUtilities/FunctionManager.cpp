
#include "FunctionManager.h"
#include "Entity.h"

//Creation time: Mon Mar 27 22:32:54 2023
#include <Components/Image.h>
#include <Components/PhysicBody.h>
#include <Components/Transform.h>
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
	map.emplace("PhysicsBody_setTrigger",PhysicsBody_setTrigger);
	map.emplace("PhysicsBody_isTrigger",PhysicsBody_isTrigger);
	map.emplace("PhysicsBody_setFriction",PhysicsBody_setFriction);
	map.emplace("PhysicsBody_getFriction",PhysicsBody_getFriction);
	map.emplace("PhysicsBody_setBounciness",PhysicsBody_setBounciness);
	map.emplace("PhysicsBody_getBounciness",PhysicsBody_getBounciness);
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
	map.emplace("InputManager_print",InputManager_print);
	map.emplace("PhysicsManager_debugDraw",PhysicsManager_debugDraw);
	map.emplace("PhysicsManager_enableDebugDraw",PhysicsManager_enableDebugDraw);
	map.emplace("ScriptFunctionality_Print_Number",ScriptFunctionality_Print_Number);
	map.emplace("ScriptFunctionality_Print_Vector2D",ScriptFunctionality_Print_Vector2D);
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
	self->setSrcRect(vec[1].value.Int, vec[2].value.Int, vec[3].value.Int, vec[4].value.Int);
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
	self->setRotaionPoint(vec[1].value.Int, vec[2].value.Int);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsBody_setTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	self->setTrigger(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsBody_isTrigger(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	bool ret = self->isTrigger();
	return ret;
}
Scripting::Variable PhysicsBody_setFriction(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	self->setFriction(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsBody_getFriction(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	float ret = self->getFriction();
	return ret;
}
Scripting::Variable PhysicsBody_setBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	self->setBounciness(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsBody_getBounciness(std::vector<Scripting::Variable>const& vec){
	PhysicsBody* self = vec[0].value.entity->getComponent<PhysicsBody>();
	float ret = self->getBounciness();
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
Scripting::Variable InputManager_print(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	manager->print();
	return Scripting::Variable::Null();
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
Scripting::Variable ScriptFunctionality_Print_Number(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Print_Number(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Print_Vector2D(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Print_Vector2D(vec[0].vector);
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
