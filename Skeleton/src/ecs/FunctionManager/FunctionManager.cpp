#include "FunctionManager.h"

//Creation time: Thu Mar 16 13:13:08 2023
#include <C:\Users\sryoj\Documents\TFG\Skeleton\src\ecs\Components\Image.h>


using namespace ECS;

void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

	map.emplace("getTextureWidth",Image_getTextureWidth);
	map.emplace("getTextureHeight",Image_getTextureHeight);
	map.emplace("setSrcRect",Image_setSrcRect);
	map.emplace("setFlipX",Image_setFlipX);
	map.emplace("setFlipY",Image_setFlipY);
	map.emplace("setRotaionPoint",Image_setRotaionPoint);

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
