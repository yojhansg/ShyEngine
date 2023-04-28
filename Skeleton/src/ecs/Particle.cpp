#include "Particle.h"
#include "Components/ParticleSystem.h"
#include "RendererManager.h"
#include "Texture.h"

namespace ECS {

	Particle::Particle(ParticleSystem* system, int idx) {

		renderer = Renderer::RendererManager::instance()->getRenderer();

		this->system = system;
		this->idx = idx;

		beingUsed = false;
		srcRect = { 0, 0, 0, 0 };
		position = { 0, 0 };
		size = { 0, 0 };
		rotation = 0;
		color = { 0, 0, 0 };
		lifeTime = 0;
		flipmode = 0;

	}

	void Particle::udpate(float deltaTime) {

		if (!beingUsed) return;

		position += system->startDirection * system->startSpeed * deltaTime;

		lifeTime -= deltaTime;

		if (lifeTime <= 0) 
			system->unusedParticles.push_back(this);

	}

	void Particle::fixedUpdate(float fixedDeltaTime) {

	}

	void Particle::render() {

		srcRect = { system->srcX, system->srcX, system->srcWidth, system->srcHeight };

		int w = size.getX();
		int h = size.getY();
		int x = std::round(position.getX() - w * 0.5f);
		int y = std::round(position.getY() + h * 0.5f);

		Renderer::RendererManager::instance()->AdjustRectToCamera(&x, &y, &w, &h);

		dstRect = { x, y, w, h };

		rotationPoint.x = w / 2;
		rotationPoint.y = h / 2;

		SDL_RenderCopyEx(renderer, system->texture->getSDLTexture(), &srcRect, &dstRect, rotation, &rotationPoint, (SDL_RendererFlip) system->flipmode);
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

	void Particle::init() {
		this->position = system->startPosition;
		this->size = system->startSize;
		this->rotation = system->startRotation;
		this->color = system->startColor;
		this->lifeTime = system->startLifeTime;
		this->flipmode = system->flipmode;
	}

}
