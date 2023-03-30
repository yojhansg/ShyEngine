#include "ClassReflection.h"

#include "Components/Image.h"
#include "Components/PhysicBody.h"
#include "Components/Transform.h"






	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("width"))
			self->width= std::stoi(map.at("width"));
		if(map.contains("height"))
			self->height= std::stoi(map.at("height"));

}
	void ClassReflection::ReflectTransform(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Transform* self = static_cast<Transform*>(selfComp);
		if(map.contains("x"))
			self->x= std::stoi(map.at("x"));
		if(map.contains("tonto"))
			self->tonto= map.at("tonto") == "true" ? true : false;
		if(map.contains("hmmmm"))
			self->hmmmm= std::stof(map.at("hmmmm"));
		if(map.contains("miVector2d"))
			self->miVector2d= map.at("miVector2d");

}
