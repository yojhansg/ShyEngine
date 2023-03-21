


#include "ClassReflection.h"
	void ClassReflection::ReflectImage(Component* selfComp, std::map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("width"))
			self->width= std::stoi(map.at("width"));
		if(map.contains("height"))
			self->height= std::stoi(map.at("height"));

}
	void ClassReflection::ReflectTransform(Component* selfComp, std::map<std::string, std::string> const& map){
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
