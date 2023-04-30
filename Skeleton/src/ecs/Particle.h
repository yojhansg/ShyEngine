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

		bool isBeingUsed();

	private:

		void init();

		void calculateOverLifeTimeSettings();

		ParticleSystem* system;

		Utilities::Vector2D position;
		Utilities::Vector2D direction;
		Utilities::Vector2D size;
		Utilities::Color color;

		float startLifeTime;
		float startSpeed;
		float startRotation;
		Utilities::Vector2D startSize;
		Utilities::Vector2D startDirection;
		Utilities::Vector2D endDirection;
		Utilities::Color startColor;
		int startAlpha;

		float rotation;
		float lifeTime;
		float speed;
		int alpha;
		float timer;

		int idx;
		bool beingUsed;

		// SDL Attributes
		SDL_Renderer* renderer;
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		SDL_Point rotationPoint;

	};

}

