#include "Particle.h"
#include "RendererManager.h"
#include "Components/ParticleSystem.h"
#include "Components/Transform.h"
#include "Texture.h"
#include "Utils.h"

#include <iostream>

using namespace Utilities;

namespace ECS {

	Particle::Particle(ParticleSystem* system, int idx) {

		renderer = Renderer::RendererManager::instance()->getRenderer();

		this->system = system;
		this->idx = idx;

		beingUsed = false; srcRect = { 0, 0, 0, 0 }; position = { 0, 0 };
		size = { 0, 0 }; rotation = 0; color = { 255, 255, 255 };
		lifeTime = 0; alpha = 255; startLifeTime = 0;

	}

	void Particle::udpate(float deltaTime) {

		if (!beingUsed) return;

		position += direction * speed * deltaTime;

		timer += deltaTime;
		lifeTime -= deltaTime;

		// Over Life Time Settings
		calculateOverLifeTimeSettings();

		if (lifeTime <= 0) 
			system->unusedParticles.push_back(this);

	}

	void Particle::fixedUpdate(float fixedDeltaTime) {

	}

	void Particle::render() {

		if (!beingUsed) return;

		srcRect = { system->srcX, system->srcX, system->srcWidth, system->srcHeight };

		int w = size.getX();
		int h = size.getY();
		int x = std::round(position.getX() - w * 0.5f);
		int y = std::round(position.getY() + h * 0.5f);

		Renderer::RendererManager::instance()->AdjustRectToCamera(&x, &y, &w, &h);

		dstRect = { x, y, w, h };

		rotationPoint.x = w / 2;
		rotationPoint.y = h / 2;

		SDL_SetTextureColorMod(system->texture->getSDLTexture(), color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(system->texture->getSDLTexture(), alpha);
		SDL_RenderCopyEx(renderer, system->texture->getSDLTexture(), &srcRect, &dstRect, rotation, &rotationPoint, (SDL_RendererFlip)system->flipmode);
	}

	int Particle::getIdx() {
		return idx;
	}

	void Particle::setAsUnused() {
		beingUsed = false;
	}

	void Particle::setAsUsed() {
		beingUsed = true;

		init();
	}

	bool Particle::isBeingUsed() {
		return beingUsed;
	}

	void Particle::init() {

		// Timer
		timer = 0.0f;


		// Position
		position = system->tr->GetLocalPosition();


		// LifeTime
		lifeTime = system->lifeTime;
		if (system->randomLifeTimeBetweenTwoValues)
			lifeTime = Utils::RandomBetween(system->lifeTimeFirstValue, system->lifeTimeSecondValue);

		startLifeTime = lifeTime;


		// Speed
		speed = system->speed;
		if (system->randomSpeedBetweenTwoValues)
			speed = Utils::RandomBetween(system->speedFirstValue, system->speedSecondValue);

		startSpeed = speed;


		// Size
		size = system->size;
		if (system->randomSizeBetweenTwoValues) {
			float x = Utils::RandomBetween(system->sizeFirstValue.getX(), system->sizeSecondValue.getX());
			float y = Utils::RandomBetween(system->sizeFirstValue.getY(), system->sizeSecondValue.getY());

			size = { x, y };
		}

		startSize = size;


		// Direction
		float angle = system->angle;
		if (system->randomAngleBetweenTwoValues)
			angle = Utils::RandomBetween(system->angleFirstValue, system->angleSecondValue);

		float radians = Utilities::Utils::DegreesToRadians(angle);
		direction = { cos(radians), sin(radians) };

		startDirection = direction;

			// End Direction
			radians = Utilities::Utils::DegreesToRadians(system->endAngle);
			endDirection = { cos(radians), sin(radians) };


		// Rotation
		rotation = system->rotation;
		if (system->randomRotationBetweenTwoValues)
			rotation = Utils::RandomBetween(system->rotationFirstValue, system->rotationSecondValue);

		startRotation = rotation;


		// Color
		color = system->color;
		if (system->randomColorBetweenTwoColors)
			color = Utils::RandomColorBetween(system->colorFirstValue, system->colorSecondValue);

		startColor = color;


		// Alpha
		alpha = system->alpha;
		if (system->randomAlphaBetweenTwoValues)
			alpha = Utils::RandomIntBetween(system->alphaFirstValue, system->alphaSecondValue);

		startAlpha = alpha;

	}

	void Particle::calculateOverLifeTimeSettings() {

		float lifePercentage = timer / startLifeTime;

		// Speed
		if (system->overLifeTimeSpeed) 
			speed = lifePercentage * (system->endSpeed - startSpeed) + startSpeed;

		// Color
		if (system->overLifeTimeColor) 
			color = Color::Lerp(startColor, system->endColor, lifePercentage);

		// Size 
		if (system->overLifeTimeSize)
			size = (system->endSize - startSize) * lifePercentage + startSize;

		// Alpha
		if (system->overLifeTimeAlpha)
			alpha = lifePercentage * (system->endAlpha - startAlpha) + startAlpha;

		// Rotation
		if (system->overLifeTimeRotation)
			rotation = lifePercentage * (system->endRotation - startRotation) + startRotation;

		// Direction
		if (system->overLifeTimeAngle)
			direction = (endDirection - startDirection) * lifePercentage + startDirection;
			

	}

}
