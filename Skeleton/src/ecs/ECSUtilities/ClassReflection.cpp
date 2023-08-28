#include "ClassReflection.h"

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
#include "Components/PhysicBody.h"
#include "Components/SoundEmitter.h"
#include "Components/Transform.h"



ClassReflection::ClassReflection(){

	reflectionMethods["Animation"] = &ClassReflection::ReflectAnimation;
	reflectionMethods["BoxBody"] = &ClassReflection::ReflectBoxBody;
	reflectionMethods["CircleBody"] = &ClassReflection::ReflectCircleBody;
	reflectionMethods["EdgeBody"] = &ClassReflection::ReflectEdgeBody;
	reflectionMethods["Image"] = &ClassReflection::ReflectImage;
	reflectionMethods["MusicEmitter"] = &ClassReflection::ReflectMusicEmitter;
	reflectionMethods["Overlay"] = &ClassReflection::ReflectOverlay;
	reflectionMethods["OverlayButton"] = &ClassReflection::ReflectOverlayButton;
	reflectionMethods["OverlayImage"] = &ClassReflection::ReflectOverlayImage;
	reflectionMethods["OverlayText"] = &ClassReflection::ReflectOverlayText;
	reflectionMethods["ParticleSystem"] = &ClassReflection::ReflectParticleSystem;
	reflectionMethods["PhysicBody"] = &ClassReflection::ReflectPhysicBody;
	reflectionMethods["SoundEmitter"] = &ClassReflection::ReflectSoundEmitter;
	reflectionMethods["Transform"] = &ClassReflection::ReflectTransform;

}ClassReflection::~ClassReflection(){

}


	void ClassReflection::ReflectComponent(std::string const& component, ECS::Component* pointer, std::unordered_map<std::string, std::string> const& map){
	if(reflectionMethods.contains(component))
		(this->*reflectionMethods[component])(pointer, map);

}
	void ClassReflection::ReflectAnimation(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Animation* self = static_cast<Animation*>(selfComp);
		if(map.contains("speed"))
			self->speed = std::stof(map.at("speed"));
		if(map.contains("currentFrame"))
			self->currentFrame = std::stoi(map.at("currentFrame"));
		if(map.contains("currentAnimation"))
			self->currentAnimation = std::stoi(map.at("currentAnimation"));
		if(map.contains("animationColumns"))
			self->animationColumns = std::stoi(map.at("animationColumns"));
		if(map.contains("animationRows"))
			self->animationRows = std::stoi(map.at("animationRows"));
		if(map.contains("animationSheetStyle"))
			self->animationSheetStyle = std::stoi(map.at("animationSheetStyle"));
		if(map.contains("animationLenght"))
			self->animationLenght = std::stoi(map.at("animationLenght"));

}
	void ClassReflection::ReflectBoxBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		BoxBody* self = static_cast<BoxBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;
		if(map.contains("freezeRotation"))
			self->freezeRotation = map.at("freezeRotation") == "true" ? true : false;
		if(map.contains("layerName"))
			self->layerName = map.at("layerName");

}
	void ClassReflection::ReflectCircleBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		CircleBody* self = static_cast<CircleBody*>(selfComp);
		if(map.contains("radius"))
			self->radius = std::stof(map.at("radius"));
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;
		if(map.contains("freezeRotation"))
			self->freezeRotation = map.at("freezeRotation") == "true" ? true : false;
		if(map.contains("layerName"))
			self->layerName = map.at("layerName");

}
	void ClassReflection::ReflectEdgeBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		EdgeBody* self = static_cast<EdgeBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;
		if(map.contains("freezeRotation"))
			self->freezeRotation = map.at("freezeRotation") == "true" ? true : false;
		if(map.contains("layerName"))
			self->layerName = map.at("layerName");

}
	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");

}
	void ClassReflection::ReflectMusicEmitter(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		MusicEmitter* self = static_cast<MusicEmitter*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");
		if(map.contains("loop"))
			self->loop = map.at("loop") == "true" ? true : false;
		if(map.contains("playOnStart"))
			self->playOnStart = map.at("playOnStart") == "true" ? true : false;
		if(map.contains("volume"))
			self->volume = std::stof(map.at("volume"));

}
	void ClassReflection::ReflectOverlay(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Overlay* self = static_cast<Overlay*>(selfComp);
		if(map.contains("placement"))
			self->placement = std::stoi(map.at("placement"));
		if(map.contains("position"))
			self->position = map.at("position");
		if(map.contains("size"))
			self->size = map.at("size");
		if(map.contains("anchor"))
			self->anchor = map.at("anchor");
		if(map.contains("left"))
			self->left = std::stoi(map.at("left"));
		if(map.contains("top"))
			self->top = std::stoi(map.at("top"));
		if(map.contains("right"))
			self->right = std::stoi(map.at("right"));
		if(map.contains("bottom"))
			self->bottom = std::stoi(map.at("bottom"));
		if(map.contains("color"))
			self->color = Utilities::Color::CreateColor(map.at("color"));
		if(map.contains("scale"))
			self->scale = std::stof(map.at("scale"));
		if(map.contains("interactable"))
			self->interactable = map.at("interactable") == "true" ? true : false;

}
	void ClassReflection::ReflectOverlayButton(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayButton* self = static_cast<OverlayButton*>(selfComp);
		if(map.contains("defaultColor"))
			self->defaultColor = Utilities::Color::CreateColor(map.at("defaultColor"));
		if(map.contains("hoverColor"))
			self->hoverColor = Utilities::Color::CreateColor(map.at("hoverColor"));
		if(map.contains("downColor"))
			self->downColor = Utilities::Color::CreateColor(map.at("downColor"));
		if(map.contains("scale"))
			self->scale = std::stof(map.at("scale"));

}
	void ClassReflection::ReflectOverlayImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayImage* self = static_cast<OverlayImage*>(selfComp);
		if(map.contains("path"))
			self->path = map.at("path");

}
	void ClassReflection::ReflectOverlayText(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayText* self = static_cast<OverlayText*>(selfComp);
		if(map.contains("text"))
			self->text = map.at("text");
		if(map.contains("font"))
			self->font = map.at("font");
		if(map.contains("fontSize"))
			self->fontSize = std::stoi(map.at("fontSize"));
		if(map.contains("horizontalAlignment"))
			self->horizontalAlignment = std::stoi(map.at("horizontalAlignment"));
		if(map.contains("verticalAlignment"))
			self->verticalAlignment = std::stoi(map.at("verticalAlignment"));
		if(map.contains("fit"))
			self->fit = std::stoi(map.at("fit"));
		if(map.contains("lineSpacing"))
			self->lineSpacing = std::stoi(map.at("lineSpacing"));

}
	void ClassReflection::ReflectParticleSystem(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		ParticleSystem* self = static_cast<ParticleSystem*>(selfComp);
		if(map.contains("duration"))
			self->duration = std::stof(map.at("duration"));
		if(map.contains("maxParticles"))
			self->maxParticles = std::stoi(map.at("maxParticles"));
		if(map.contains("loop"))
			self->loop = map.at("loop") == "true" ? true : false;
		if(map.contains("delay"))
			self->delay = std::stof(map.at("delay"));
		if(map.contains("playOnStart"))
			self->playOnStart = map.at("playOnStart") == "true" ? true : false;
		if(map.contains("lifeTime"))
			self->lifeTime = std::stof(map.at("lifeTime"));
		if(map.contains("randomLifeTimeBetweenTwoValues"))
			self->randomLifeTimeBetweenTwoValues = map.at("randomLifeTimeBetweenTwoValues") == "true" ? true : false;
		if(map.contains("lifeTimeFirstValue"))
			self->lifeTimeFirstValue = std::stof(map.at("lifeTimeFirstValue"));
		if(map.contains("lifeTimeSecondValue"))
			self->lifeTimeSecondValue = std::stof(map.at("lifeTimeSecondValue"));
		if(map.contains("speed"))
			self->speed = std::stof(map.at("speed"));
		if(map.contains("randomSpeedBetweenTwoValues"))
			self->randomSpeedBetweenTwoValues = map.at("randomSpeedBetweenTwoValues") == "true" ? true : false;
		if(map.contains("speedFirstValue"))
			self->speedFirstValue = std::stof(map.at("speedFirstValue"));
		if(map.contains("speedSecondValue"))
			self->speedSecondValue = std::stof(map.at("speedSecondValue"));
		if(map.contains("size"))
			self->size = map.at("size");
		if(map.contains("randomSizeBetweenTwoValues"))
			self->randomSizeBetweenTwoValues = map.at("randomSizeBetweenTwoValues") == "true" ? true : false;
		if(map.contains("sizeFirstValue"))
			self->sizeFirstValue = map.at("sizeFirstValue");
		if(map.contains("sizeSecondValue"))
			self->sizeSecondValue = map.at("sizeSecondValue");
		if(map.contains("angle"))
			self->angle = std::stof(map.at("angle"));
		if(map.contains("randomAngleBetweenTwoValues"))
			self->randomAngleBetweenTwoValues = map.at("randomAngleBetweenTwoValues") == "true" ? true : false;
		if(map.contains("angleFirstValue"))
			self->angleFirstValue = std::stof(map.at("angleFirstValue"));
		if(map.contains("angleSecondValue"))
			self->angleSecondValue = std::stof(map.at("angleSecondValue"));
		if(map.contains("rotation"))
			self->rotation = std::stof(map.at("rotation"));
		if(map.contains("randomRotationBetweenTwoValues"))
			self->randomRotationBetweenTwoValues = map.at("randomRotationBetweenTwoValues") == "true" ? true : false;
		if(map.contains("rotationFirstValue"))
			self->rotationFirstValue = std::stof(map.at("rotationFirstValue"));
		if(map.contains("rotationSecondValue"))
			self->rotationSecondValue = std::stof(map.at("rotationSecondValue"));
		if(map.contains("color"))
			self->color = Utilities::Color::CreateColor(map.at("color"));
		if(map.contains("randomColorBetweenTwoColors"))
			self->randomColorBetweenTwoColors = map.at("randomColorBetweenTwoColors") == "true" ? true : false;
		if(map.contains("colorFirstValue"))
			self->colorFirstValue = Utilities::Color::CreateColor(map.at("colorFirstValue"));
		if(map.contains("colorSecondValue"))
			self->colorSecondValue = Utilities::Color::CreateColor(map.at("colorSecondValue"));
		if(map.contains("layerName"))
			self->layerName = map.at("layerName");
		if(map.contains("sensor"))
			self->sensor = map.at("sensor") == "true" ? true : false;
		if(map.contains("gravityScale"))
			self->gravityScale = std::stof(map.at("gravityScale"));
		if(map.contains("force"))
			self->force = std::stof(map.at("force"));
		if(map.contains("randomForceBetweenTwoValues"))
			self->randomForceBetweenTwoValues = map.at("randomForceBetweenTwoValues") == "true" ? true : false;
		if(map.contains("forceFirstValue"))
			self->forceFirstValue = std::stof(map.at("forceFirstValue"));
		if(map.contains("forceSecondValue"))
			self->forceSecondValue = std::stof(map.at("forceSecondValue"));
		if(map.contains("impulse"))
			self->impulse = std::stof(map.at("impulse"));
		if(map.contains("randomImpulseBetweenTwoValues"))
			self->randomImpulseBetweenTwoValues = map.at("randomImpulseBetweenTwoValues") == "true" ? true : false;
		if(map.contains("impulseFirstValue"))
			self->impulseFirstValue = std::stof(map.at("impulseFirstValue"));
		if(map.contains("impulseSecondValue"))
			self->impulseSecondValue = std::stof(map.at("impulseSecondValue"));
		if(map.contains("angularVelocity"))
			self->angularVelocity = std::stof(map.at("angularVelocity"));
		if(map.contains("randomAngularVelocityBetweenTwoValues"))
			self->randomAngularVelocityBetweenTwoValues = map.at("randomAngularVelocityBetweenTwoValues") == "true" ? true : false;
		if(map.contains("angularVelocityFirstValue"))
			self->angularVelocityFirstValue = std::stof(map.at("angularVelocityFirstValue"));
		if(map.contains("angularVelocitySecondValue"))
			self->angularVelocitySecondValue = std::stof(map.at("angularVelocitySecondValue"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("randomBouncinessBetweenTwoValues"))
			self->randomBouncinessBetweenTwoValues = map.at("randomBouncinessBetweenTwoValues") == "true" ? true : false;
		if(map.contains("bouncinessFirstValue"))
			self->bouncinessFirstValue = std::stof(map.at("bouncinessFirstValue"));
		if(map.contains("bouncinessSecondValue"))
			self->bouncinessSecondValue = std::stof(map.at("bouncinessSecondValue"));
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");
		if(map.contains("flipmode"))
			self->flipmode = std::stoi(map.at("flipmode"));
		if(map.contains("srcX"))
			self->srcX = std::stoi(map.at("srcX"));
		if(map.contains("srcY"))
			self->srcY = std::stoi(map.at("srcY"));
		if(map.contains("srcWidth"))
			self->srcWidth = std::stoi(map.at("srcWidth"));
		if(map.contains("srcHeight"))
			self->srcHeight = std::stoi(map.at("srcHeight"));
		if(map.contains("alpha"))
			self->alpha = std::stoi(map.at("alpha"));
		if(map.contains("randomAlphaBetweenTwoValues"))
			self->randomAlphaBetweenTwoValues = map.at("randomAlphaBetweenTwoValues") == "true" ? true : false;
		if(map.contains("alphaFirstValue"))
			self->alphaFirstValue = std::stoi(map.at("alphaFirstValue"));
		if(map.contains("alphaSecondValue"))
			self->alphaSecondValue = std::stoi(map.at("alphaSecondValue"));
		if(map.contains("emissionRateOverTime"))
			self->emissionRateOverTime = std::stof(map.at("emissionRateOverTime"));
		if(map.contains("emissionRateOverDistance"))
			self->emissionRateOverDistance = std::stof(map.at("emissionRateOverDistance"));
		if(map.contains("overLifeTimeSpeed"))
			self->overLifeTimeSpeed = map.at("overLifeTimeSpeed") == "true" ? true : false;
		if(map.contains("endSpeed"))
			self->endSpeed = std::stof(map.at("endSpeed"));
		if(map.contains("overLifeTimeColor"))
			self->overLifeTimeColor = map.at("overLifeTimeColor") == "true" ? true : false;
		if(map.contains("endColor"))
			self->endColor = Utilities::Color::CreateColor(map.at("endColor"));
		if(map.contains("overLifeTimeSize"))
			self->overLifeTimeSize = map.at("overLifeTimeSize") == "true" ? true : false;
		if(map.contains("endSize"))
			self->endSize = map.at("endSize");
		if(map.contains("overLifeTimeAlpha"))
			self->overLifeTimeAlpha = map.at("overLifeTimeAlpha") == "true" ? true : false;
		if(map.contains("endAlpha"))
			self->endAlpha = std::stoi(map.at("endAlpha"));

}
	void ClassReflection::ReflectPhysicBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		PhysicBody* self = static_cast<PhysicBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;
		if(map.contains("freezeRotation"))
			self->freezeRotation = map.at("freezeRotation") == "true" ? true : false;
		if(map.contains("layerName"))
			self->layerName = map.at("layerName");

}
	void ClassReflection::ReflectSoundEmitter(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		SoundEmitter* self = static_cast<SoundEmitter*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");
		if(map.contains("loop"))
			self->loop = map.at("loop") == "true" ? true : false;
		if(map.contains("playOnStart"))
			self->playOnStart = map.at("playOnStart") == "true" ? true : false;
		if(map.contains("volume"))
			self->volume = std::stof(map.at("volume"));
		if(map.contains("spatialSound"))
			self->spatialSound = map.at("spatialSound") == "true" ? true : false;
		if(map.contains("panning"))
			self->panning = std::stof(map.at("panning"));
		if(map.contains("audibleDistance"))
			self->audibleDistance = std::stof(map.at("audibleDistance"));

}
	void ClassReflection::ReflectTransform(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Transform* self = static_cast<Transform*>(selfComp);
		if(map.contains("localPosition"))
			self->localPosition = map.at("localPosition");
		if(map.contains("localScale"))
			self->localScale = map.at("localScale");
		if(map.contains("localRotation"))
			self->localRotation = std::stod(map.at("localRotation"));

}
