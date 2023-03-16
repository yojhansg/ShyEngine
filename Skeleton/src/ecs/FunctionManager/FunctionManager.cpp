#include "FunctionManager.h"

//Creation time: Thu Mar 16 17:37:25 2023
#include <Components/Image.h>
#include <Components/Transform.h>


using namespace ECS;

void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

	map.emplace("Image_getTextureWidth",Image_getTextureWidth);
	map.emplace("Image_getTextureHeight",Image_getTextureHeight);
	map.emplace("Image_setSrcRect",Image_setSrcRect);
	map.emplace("Image_setFlipX",Image_setFlipX);
	map.emplace("Image_setFlipY",Image_setFlipY);
	map.emplace("Image_setRotaionPoint",Image_setRotaionPoint);
	map.emplace("Transform_getPosition",Transform_getPosition);
	map.emplace("Transform_getScale",Transform_getScale);
	map.emplace("Transform_setPosition",Transform_setPosition);
	map.emplace("Transform_setScale",Transform_setScale);
	map.emplace("Transform_setRotation",Transform_setRotation);
	map.emplace("Transform_translate",Transform_translate);
	map.emplace("Transform_rotate",Transform_rotate);
	map.emplace("Transform_scale",Transform_scale);

};
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	int ret = self->getTextureWidth();
	return ret;
}
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	int ret = self->getTextureHeight();
	return ret;
}
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	self->setSrcRect(vec[1].value.Int, vec[2].value.Int, vec[3].value.Int, vec[4].value.Int);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	self->setFlipX(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	self->setFlipY(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec){
	Image* self = static_cast<Image*>(vec[0].value.pointer);
	self->setRotaionPoint(vec[1].value.Int, vec[2].value.Int);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_getPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	Utilities::Vector2D* ret = self->getPosition();
	return ret;
}
Scripting::Variable Transform_getScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	Utilities::Vector2D* ret = self->getScale();
	return ret;
}
Scripting::Variable Transform_setPosition(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->setPosition(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setScale(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->setScale(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_setRotation(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->setRotation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_translate(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->translate(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_rotate(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->rotate(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_scale(std::vector<Scripting::Variable>const& vec){
	Transform* self = static_cast<Transform*>(vec[0].value.pointer);
	self->scale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
