#pragma once

#include "Singleton.h"
#include <string>
#include <unordered_map>
namespace ECS { class Component; }

//Creation time : Fri Aug 18 15:21:18 2023

#define ECSreflection_Version 1.0

using namespace ECS;
class ClassReflection: public Utilities::Singleton<ClassReflection>{

	 friend Singleton<ClassReflection>;
typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::unordered_map<std::string, std::string> const&);

private: 
	std::unordered_map<std::string, ReflectionMethod> reflectionMethods;

	ClassReflection();
public: 
	~ClassReflection();
	void ReflectComponent(std::string const& component, ECS::Component* pointer, std::unordered_map<std::string, std::string> const& map);
	void ReflectAnimation(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectBoxBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectChainBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectCircleBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectEdgeBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectMusicEmitter(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectOverlay(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectOverlayButton(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectOverlayImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectOverlayText(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectParticleSystem(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectPhysicBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectSoundEmitter(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

	void ReflectTransform(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map);

};
