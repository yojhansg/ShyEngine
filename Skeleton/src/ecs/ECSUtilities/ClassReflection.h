#pragma once

#include <string>
#include <map>
#include <Components/Image.h>
#include <Components/PhysicBody.h>
#include <Components/Transform.h>
#include <PhysicsManager.h>

//Creation time : Wed Mar 22 23:46:56 2023
#define ECSreflection_Version 1.0

using namespace ECS;
typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::map<std::string, std::string> const&);

class ClassReflection{

	std::map<std::string, ReflectionMethod> reflectionMethods;

public: 
	static void ReflectImage(Component* selfComp, std::map<std::string, std::string> const& map);

	static void ReflectTransform(Component* selfComp, std::map<std::string, std::string> const& map);

};
