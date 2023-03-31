#include "ClassReflection.h"

#include "Components/Image.h"
#include "Components/PhysicBody.h"
#include "Components/Transform.h"



ClassReflection::ClassReflection(){

	reflectionMethods["Image"] = &ClassReflection::ReflectImage;
	reflectionMethods["Transform"] = &ClassReflection::ReflectTransform;

}ClassReflection::~ClassReflection(){

}


	void ClassReflection::ReflectComponent(std::string const& component, ECS::Component* pointer, std::map<std::string, std::string> const& map){
	if(reflectionMethods.contains(component))
		(this->*reflectionMethods[component])(pointer, map);

}
	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");

}
	void ClassReflection::ReflectTransform(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Transform* self = static_cast<Transform*>(selfComp);
		if(map.contains("position_"))
			self->position_ = map.at("position_");
		if(map.contains("scale_"))
			self->scale_ = map.at("scale_");
		if(map.contains("rotation_"))
			self->rotation_ = std::stod(map.at("rotation_"));

}
