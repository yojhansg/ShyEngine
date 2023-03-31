#pragma once

#include "Singleton.h"
#include <string>
#include <map>
namespace ECS { class Component; }

//Creation time : Thu Mar 30 13:18:38 2023
#define ECSreflection_Version 1.0

using namespace ECS;
class ClassReflection: public Utilities::Singleton<ClassReflection>{

	 friend Singleton<ClassReflection>;
typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::map<std::string, std::string> const&);

private: 
	std::map<std::string, ReflectionMethod> reflectionMethods;

	ClassReflection();
public: 
	~ClassReflection();
	void ReflectComponent(std::string const& component, ECS::Component* pointer, std::map<std::string, std::string> const& map);
private: 
	void ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map);

	void ReflectTransform(ECS::Component* selfComp, std::map<std::string, std::string> const& map);

};
