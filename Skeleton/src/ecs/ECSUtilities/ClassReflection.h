#pragma once

#include <string>
#include <map>
#include <Components/Image.h>
#include <Components/Transform.h>

//Creation time : Sun Mar 19 13:59:41 2023
#define ECSreflection_Version 1.0

using namespace ECS;
class ClassReflection{
	void hola( );
public: 
	static void ReflectImage(Image* self, std::map<std::string, std::string> const& map);
	static void ReflectTransform(Transform* self, std::map<std::string, std::string> const& map);
};
