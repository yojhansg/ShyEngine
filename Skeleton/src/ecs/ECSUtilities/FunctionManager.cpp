
#include "FunctionManager.h"
#include "Entity.h"

//Creation time: Sat Mar 25 17:27:25 2023
#include <Components/Image.h>
#include <Components/PhysicBody.h>
#include <Components/Transform.h>
#include <PhysicsManager.h>


using namespace ECS;
using namespace Physics;



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
	map.emplace("PhysicsManager_debugDraw",PhysicsManager_debugDraw);
	map.emplace("PhysicsManager_enableDebugDraw",PhysicsManager_enableDebugDraw);

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
	Utilities::Vector2D* ret = self->getPosition();
	return ret;
}
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = vec[0].value.entity->getComponent<Transform>();
	Utilities::Vector2D* ret = self->getScale();
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
