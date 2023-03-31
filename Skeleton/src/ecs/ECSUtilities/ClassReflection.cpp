#include "ClassReflection.h"

#include "Components/BoxBody.h"
#include "Components/ChainBody.h"
#include "Components/CircleBody.h"
#include "Components/EdgeBody.h"
#include "Components/Image.h"
#include "Components/PhysicBody.h"
#include "Components/Transform.h"



ClassReflection::ClassReflection(){

	reflectionMethods["BoxBody"] = &ClassReflection::ReflectBoxBody;
	reflectionMethods["ChainBody"] = &ClassReflection::ReflectChainBody;
	reflectionMethods["CircleBody"] = &ClassReflection::ReflectCircleBody;
	reflectionMethods["EdgeBody"] = &ClassReflection::ReflectEdgeBody;
	reflectionMethods["Image"] = &ClassReflection::ReflectImage;
	reflectionMethods["PhysicsBody"] = &ClassReflection::ReflectPhysicsBody;
	reflectionMethods["Transform"] = &ClassReflection::ReflectTransform;

}ClassReflection::~ClassReflection(){

}


	void ClassReflection::ReflectComponent(std::string const& component, ECS::Component* pointer, std::map<std::string, std::string> const& map){
	if(reflectionMethods.contains(component))
		(this->*reflectionMethods[component])(pointer, map);

}
	void ClassReflection::ReflectBoxBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		BoxBody* self = static_cast<BoxBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectChainBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		ChainBody* self = static_cast<ChainBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectCircleBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		CircleBody* self = static_cast<CircleBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectEdgeBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		EdgeBody* self = static_cast<EdgeBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");

}
	void ClassReflection::ReflectPhysicsBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		PhysicsBody* self = static_cast<PhysicsBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

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
