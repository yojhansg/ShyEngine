#pragma once

#include "Singleton.h"
#include <string>
#include <unordered_map>
namespace ECS { class Component; }

//Creation time : Sat Aug 19 17:34:31 2023

#define ECSreflection_Version 1.0

using namespace ECS;
class ComponentFactory: public Utilities::Singleton<ComponentFactory>{

	 friend Singleton<ComponentFactory>;
public: 
	~ComponentFactory();
private: 
typedef ECS::Component*(ComponentFactory::*ComponentCreation)();

	ComponentFactory();
	std::unordered_map<std::string, ComponentCreation> components;

	ECS::Component* CreateAnimation();
	ECS::Component* CreateBallComponent();
	ECS::Component* CreateBoxBody();
	ECS::Component* CreateChainBody();
	ECS::Component* CreateCircleBody();
	ECS::Component* CreateEdgeBody();
	ECS::Component* CreateImage();
	ECS::Component* CreateLinkComponent();
	ECS::Component* CreateMusicEmitter();
	ECS::Component* CreateOverlay();
	ECS::Component* CreateOverlayButton();
	ECS::Component* CreateOverlayImage();
	ECS::Component* CreateOverlayText();
	ECS::Component* CreateParticleSystem();
	ECS::Component* CreateSoundEmitter();
	ECS::Component* CreateTransform();
	ECS::Component* CreateScript();
public: 

	ECS::Component* CreateComponent(std::string const& comp);
};
