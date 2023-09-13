#include "ComponentFactory.h"

#include "Components/Animation.h"
#include "Components/BoxBody.h"
#include "Components/CircleBody.h"
#include "Components/EdgeBody.h"
#include "Components/Image.h"
#include "Components/MusicEmitter.h"
#include "Components/Overlay.h"
#include "Components/OverlayButton.h"
#include "Components/OverlayImage.h"
#include "Components/OverlayText.h"
#include "Components/ParticleSystem.h"
#include "Components/PlatformController.h"
#include "Components/SoundEmitter.h"
#include "Components/TopDownController.h"
#include "Components/Transform.h"
#include "Scripting/Script.h"



ComponentFactory::ComponentFactory(){

	components["Animation"] = &ComponentFactory::CreateAnimation;
	components["BoxBody"] = &ComponentFactory::CreateBoxBody;
	components["CircleBody"] = &ComponentFactory::CreateCircleBody;
	components["EdgeBody"] = &ComponentFactory::CreateEdgeBody;
	components["Image"] = &ComponentFactory::CreateImage;
	components["MusicEmitter"] = &ComponentFactory::CreateMusicEmitter;
	components["Overlay"] = &ComponentFactory::CreateOverlay;
	components["OverlayButton"] = &ComponentFactory::CreateOverlayButton;
	components["OverlayImage"] = &ComponentFactory::CreateOverlayImage;
	components["OverlayText"] = &ComponentFactory::CreateOverlayText;
	components["ParticleSystem"] = &ComponentFactory::CreateParticleSystem;
	components["PlatformController"] = &ComponentFactory::CreatePlatformController;
	components["SoundEmitter"] = &ComponentFactory::CreateSoundEmitter;
	components["TopDownController"] = &ComponentFactory::CreateTopDownController;
	components["Transform"] = &ComponentFactory::CreateTransform;
	components["Script"] = &ComponentFactory::CreateScript;

}ComponentFactory::~ComponentFactory(){

}


	ECS::Component* ComponentFactory::CreateAnimation(){
		return new ECS::Animation();

}
	ECS::Component* ComponentFactory::CreateBoxBody(){
		return new ECS::BoxBody();

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
	ECS::Component* ComponentFactory::CreateMusicEmitter(){
		return new ECS::MusicEmitter();

}
	ECS::Component* ComponentFactory::CreateOverlay(){
		return new ECS::Overlay();

}
	ECS::Component* ComponentFactory::CreateOverlayButton(){
		return new ECS::OverlayButton();

}
	ECS::Component* ComponentFactory::CreateOverlayImage(){
		return new ECS::OverlayImage();

}
	ECS::Component* ComponentFactory::CreateOverlayText(){
		return new ECS::OverlayText();

}
	ECS::Component* ComponentFactory::CreateParticleSystem(){
		return new ECS::ParticleSystem();

}
	ECS::Component* ComponentFactory::CreatePlatformController(){
		return new ECS::PlatformController();

}
	ECS::Component* ComponentFactory::CreateSoundEmitter(){
		return new ECS::SoundEmitter();

}
	ECS::Component* ComponentFactory::CreateTopDownController(){
		return new ECS::TopDownController();

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
