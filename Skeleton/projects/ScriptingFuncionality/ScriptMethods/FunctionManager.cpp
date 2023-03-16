#include "FunctionManager.h"

//Creation time: Thu Mar 16 12:49:13 2023


void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

	map.emplace("getTextureWidth",Image_getTextureWidth);
	map.emplace("getTextureHeight",Image_getTextureHeight);
	map.emplace("setSrcRect",Image_setSrcRect);
	map.emplace("setFlipX",Image_setFlipX);
	map.emplace("setFlipY",Image_setFlipY);
	map.emplace("setRotaionPoint",Image_setRotaionPoint);

};
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	int* ret = self->getTextureWidth();
	return ret;
}
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	int* ret = self->getTextureHeight();
	return ret;
}
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	self->setSrcRect(vec[0].value.Int, vec[1].value.Int, vec[2].value.Int, vec[3].value.Int);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipX(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	self->setFlipX(vec[0].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipY(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	self->setFlipY(vec[0].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>& vec);{
	Image* self = static_cast<Image*>(self);
	self->setRotaionPoint(vec[0].value.Int, vec[1].value.Int);
	return Scripting::Variable::Null();
}
