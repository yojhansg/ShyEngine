#include "ComponentFactory.h"

#include "Components/BoxBody.h"
#include "Components/Camera.h"
#include "Components/ChainBody.h"
#include "Components/CircleBody.h"
#include "Components/EdgeBody.h"
#include "Components/Image.h"
#include "Components/OverlayElement.h"
#include "Components/OverlayImage.h"
#include "Components/OverlayText.h"
#include "Components/TestComponent.h"
#include "Components/Transform.h"
#include "Scripting/Script.h"



ComponentFactory::ComponentFactory(){

	components["BoxBody"] = &ComponentFactory::CreateBoxBody;
	components["Camera"] = &ComponentFactory::CreateCamera;
	components["ChainBody"] = &ComponentFactory::CreateChainBody;
	components["CircleBody"] = &ComponentFactory::CreateCircleBody;
	components["EdgeBody"] = &ComponentFactory::CreateEdgeBody;
	components["Image"] = &ComponentFactory::CreateImage;
	components["OverlayElement"] = &ComponentFactory::CreateOverlayElement;
	components["OverlayImage"] = &ComponentFactory::CreateOverlayImage;
	components["OverlayText"] = &ComponentFactory::CreateOverlayText;
	components["TestComponent"] = &ComponentFactory::CreateTestComponent;
	components["Transform"] = &ComponentFactory::CreateTransform;
	components["Script"] = &ComponentFactory::CreateScript;

}ComponentFactory::~ComponentFactory(){

}


	ECS::Component* ComponentFactory::CreateBoxBody(){
		return new ECS::BoxBody();

}
	ECS::Component* ComponentFactory::CreateCamera(){
		return new ECS::Camera();

}
	ECS::Component* ComponentFactory::CreateChainBody(){
		return new ECS::ChainBody();

}
	ECS::Component* ComponentFactory::CreateCircleBody(){
		return new ECS::CircleBody();

}
	ECS::Component* ComponentFactory::CreateEdgeBody(){
		return new ECS::EdgeBody();

}
	ECS::Component* ComponentFactory::CreateImage(){
		return new ECS::Image();

}
	ECS::Component* ComponentFactory::CreateOverlayElement(){
		return new ECS::OverlayElement();

}
	ECS::Component* ComponentFactory::CreateOverlayImage(){
		return new ECS::OverlayImage();

}
	ECS::Component* ComponentFactory::CreateOverlayText(){
		return new ECS::OverlayText();

}
	ECS::Component* ComponentFactory::CreateTestComponent(){
		return new ECS::TestComponent();

}
	ECS::Component* ComponentFactory::CreateTransform(){
		return new ECS::Transform();

}
	ECS::Component* ComponentFactory::CreateScript(){
		return new ECS::Script();

}
	ECS::Component* ComponentFactory::CreateComponent(std::string const& comp){
	if(components.contains(comp))
		return (this->*components[comp])();
	return nullptr;
}
