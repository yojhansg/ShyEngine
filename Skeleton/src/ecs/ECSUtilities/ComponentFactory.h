#pragma once

#include "Singleton.h"
#include <string>
#include <map>
namespace ECS { class Component; }

//Creation time : Fri Apr  7 16:29:02 2023

#define ECSreflection_Version 1.0

using namespace ECS;
class ComponentFactory: public Utilities::Singleton<ComponentFactory>{

	 friend Singleton<ComponentFactory>;
public: 
	~ComponentFactory();
private: 
typedef ECS::Component*(ComponentFactory::*ComponentCreation)();

	ComponentFactory();
	std::map<std::string, ComponentCreation> components;

	ECS::Component* CreateBoxBody();
	ECS::Component* CreateCamera();
	ECS::Component* CreateChainBody();
	ECS::Component* CreateCircleBody();
	ECS::Component* CreateEdgeBody();
	ECS::Component* CreateImage();
	ECS::Component* CreateOverlay();
	ECS::Component* CreateOverlayButton();
	ECS::Component* CreateOverlayImage();
	ECS::Component* CreateOverlayText();
	ECS::Component* CreateTestComponent();
	ECS::Component* CreateTransform();
	ECS::Component* CreateScript();
public: 

	ECS::Component* CreateComponent(std::string const& comp);
};
