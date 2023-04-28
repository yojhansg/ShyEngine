#pragma once

#include <Vector2D.h>
#include <SDL_rect.h>
#include <Color.h>

struct SDL_Renderer;

namespace ECS {

	class ParticleSystem;

	class Particle {

	public:

		Particle(ParticleSystem* system, int idx);

		void udpate(float deltaTime);

		void fixedUpdate(float fixedDeltaTime);

		void render();

		int getIdx();

		void setAsUnused();

		void setAsUsed();

	private:

		void init();

		ParticleSystem* system;

		Utilities::Vector2D position;
		Utilities::Vector2D size;
		float rotation;

		Utilities::Color color;

		float lifeTime;

		int idx;

		int flipmode;

		bool beingUsed;

		SDL_Renderer* renderer;

		SDL_Rect srcRect;
		SDL_Rect dstRect;
		SDL_Point rotationPoint;

	};

}

