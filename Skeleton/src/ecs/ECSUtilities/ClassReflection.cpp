#include "ClassReflection.h"
	void ClassReflection::hola( ){

}
	void ClassReflection::ReflectImage(Image* self, std::map<std::string, std::string> const& map){
	if(map.contains("width"))
		self->width= std::stoi(map.at("width"));
	if(map.contains("height"))
		self->height= std::stoi(map.at("height"));

}
	void ClassReflection::ReflectTransform(Transform* self, std::map<std::string, std::string> const& map){
	if(map.contains("x"))
		self->x= std::stoi(map.at("x"));
	if(map.contains("tonto"))
		self->tonto= map.at("tonto") == "true" ? true : false;
	if(map.contains("hmmmm"))
		self->hmmmm= std::stof(map.at("hmmmm"));
	if(map.contains("miVector2d"))
		self->miVector2d= map.at("miVector2d");

}
