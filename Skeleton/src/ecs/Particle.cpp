#include "Particle.h"
#include "RendererManager.h"
#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "SDL_render.h"
#include "Components/ParticleSystem.h"
#include "Components/Transform.h"
#include "Texture.h"
#include "Random.h"

#include <iostream>

#define PI 3.14159265f

using namespace Utilities;

namespace ECS {

	Particle::Particle(ParticleSystem* system, int idx) {

		renderer = Renderer::RendererManager::instance()->getRenderer();

		this->system = system;
		this->idx = idx;

		beingUsed = false; srcRect = { 0, 0, 0, 0 }; position = { 0, 0 };
		size = { 0, 0 }; rotation = 0; color = { 255, 255, 255 };
		lifeTime = 0; alpha = 255; startLifeTime = 0;

		createPhysicsAttributes();

	}

	Particle::~Particle() {
		delete bodyDef;
		delete fixtureDef;
	}

	void Particle::fixedUpdate(float fixedDeltaTime) {

		if (!beingUsed) return;

		timer += fixedDeltaTime;
		lifeTime -= fixedDeltaTime;

		// Over Life Time Settings
		calculateOverLifeTimeSettings();

		if (lifeTime <= 0)
			system->unusedParticles.push_back(this);

		//body->SetLinearVelocity({ direction.getX() * speed * fixedDeltaTime, direction.getY() * speed * fixedDeltaTime });

		// Position
		position = { body->GetPosition().x * system->screenToWorldFactor, body->GetPosition().y * system->screenToWorldFactor };

		// Rotation
		rotation = body->GetAngle() * (180 / b2_pi);

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

		auto texture = system->texture->getSDLTexture();

		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(texture, alpha);
		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, rotation, &rotationPoint, (SDL_RendererFlip) system->flipmode);
	}

	int Particle::getIdx() {
		return idx;
	}

	void Particle::setAsUnused() {
		beingUsed = false;

		body->SetLinearVelocity({ 0,0 });
		body->SetAngularVelocity(0);
		body->SetEnabled(false);
	}

	void Particle::setAsUsed() {
		beingUsed = true;

		init();
	}

	bool Particle::isBeingUsed() {
		return beingUsed;
	}

	void Particle::init() {

		float factor = system->screenToWorldFactor;

		// Timer
		timer = 0.0f;


		// Position
		position = system->tr->GetLocalPosition();


		// LifeTime
		lifeTime = system->lifeTime;
		if (system->randomLifeTimeBetweenTwoValues)
			lifeTime = Random::RandomBetween(system->lifeTimeFirstValue, system->lifeTimeSecondValue);

		startLifeTime = lifeTime;


		// Speed
		speed = system->speed;
		if (system->randomSpeedBetweenTwoValues)
			speed = Random::RandomBetween(system->speedFirstValue, system->speedSecondValue);

		startSpeed = speed;


		// Size
		size = system->size;
		if (system->randomSizeBetweenTwoValues) {
			float x = Random::RandomBetween(system->sizeFirstValue.getX(), system->sizeSecondValue.getX());
			float y = Random::RandomBetween(system->sizeFirstValue.getY(), system->sizeSecondValue.getY());

			size = { x, y };
		}

		startSize = size;


		// Force
		force = system->force;
		if (system->randomForceBetweenTwoValues)
			force = Random::RandomBetween(system->forceFirstValue, system->forceSecondValue);


		// Impulse
		impulse = system->impulse;
		if (system->randomImpulseBetweenTwoValues)
			impulse = Random::RandomBetween(system->impulseFirstValue, system->impulseSecondValue);


		// Direction
		float angle = system->angle;
		if (system->randomAngleBetweenTwoValues)
			angle = Random::RandomBetween(system->angleFirstValue, system->angleSecondValue);

		float radians = angle * PI / 180.0f;
		direction = { cos(radians), sin(radians) };

		// Rotation
		rotation = system->rotation;
		if (system->randomRotationBetweenTwoValues)
			rotation = Random::RandomBetween(system->rotationFirstValue, system->rotationSecondValue);

		// Angular Velocity
		angularVelocity = system->angularVelocity;
		if (system->randomAngularVelocityBetweenTwoValues)
			angularVelocity = Random::RandomBetween(system->angularVelocityFirstValue, system->angularVelocitySecondValue);

		// Color
		color = system->color;
		if (system->randomColorBetweenTwoColors)
			color = Random::RandomColorBetween(system->colorFirstValue, system->colorSecondValue);

		startColor = color;


		// Alpha
		alpha = system->alpha;
		if (system->randomAlphaBetweenTwoValues)
			alpha = Random::RandomIntBetween(system->alphaFirstValue, system->alphaSecondValue);

		startAlpha = alpha;

		// Bounciness
		bounciness = system->bounciness;
		if (system->randomBouncinessBetweenTwoValues)
			bounciness = Random::RandomBetween(system->bouncinessFirstValue, system->bouncinessSecondValue);

		
		// Physics
		body->SetEnabled(true);

		shape->SetAsBox(size.getX() / factor / 2.0f, size.getY() / factor / 2.0f);

		body->SetGravityScale(system->gravityScale);

		body->SetTransform({ position.getX() / factor, position.getY() / factor }, rotation * (b2_pi / 180));

		fixture->SetSensor(system->sensor);

		fixture->SetRestitution(bounciness);

		body->SetAngularVelocity(angularVelocity);

		//body->ApplyLinearImpulseToCenter({ impulse * direction.getX(), impulse * direction.getY()}, true);

		body->ApplyForceToCenter({ force * direction.getX(), force * direction.getY() }, true);
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
		if (system->overLifeTimeSize) {
			size = (system->endSize - startSize) * lifePercentage + startSize;
			shape->SetAsBox(size.getX() / system->screenToWorldFactor / 2.0f, size.getY() / system->screenToWorldFactor / 2.0f);
		}

		// Alpha
		if (system->overLifeTimeAlpha)
			alpha = lifePercentage * (system->endAlpha - startAlpha) + startAlpha;

	}

	void Particle::createPhysicsAttributes() {

		// Physics Attributes creation
		bodyDef = new b2BodyDef();
		fixtureDef = new b2FixtureDef();
		shape = new b2PolygonShape();

		shape->SetAsBox(1, 1);
		bodyDef->type = b2_dynamicBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(nullptr));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = system->world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;
		shape = static_cast<b2PolygonShape*>(fixture->GetShape());

		// Collision Filtering
		b2Filter filter = fixture->GetFilterData();

		filter.categoryBits = system->physicsManager->getLayerBits(system->layerName);
		filter.maskBits = system->physicsManager->getMaskBits(system->layerName);

		fixture->SetFilterData(filter);

		body->SetEnabled(false);
	}

}
