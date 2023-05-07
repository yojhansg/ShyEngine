#pragma once

#include <Vector2D.h>
#include <SDL_rect.h>
#include <box2d/b2_fixture.h>
#include <Color.h>

struct SDL_Renderer;
class b2BodyDef;
class b2Body;
class b2FixtureDef;
class b2Fixture;
class b2World;
class b2PolygonShape;

namespace ECS {

	class ParticleSystem;

	class Particle {

	public:

		Particle(ParticleSystem* system, int idx);

		~Particle();

		void fixedUpdate(float fixedDeltaTime);

		void render();

		int getIdx();

		void setAsUnused();

		void setAsUsed();

		bool isBeingUsed();

	private:

		void init();

		void calculateOverLifeTimeSettings();

		void createPhysicsAttributes();

		ParticleSystem* system;

		Utilities::Vector2D position;
		Utilities::Vector2D direction;
		Utilities::Vector2D size;
		Utilities::Color color;

		float force;
		float impulse;
		float rotation;
		float angularVelocity;
		float bounciness;
		float lifeTime;
		float speed;
		int alpha;
		float timer;

		// Start values
		Utilities::Vector2D startSize;
		Utilities::Vector2D endDirection;
		Utilities::Color startColor;
		float startLifeTime;
		float startSpeed;
		int startAlpha;

		int idx;
		bool beingUsed;

		// SDL Attributes
		SDL_Renderer* renderer;
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		SDL_Point rotationPoint;

		// Box2D Attributes
		b2BodyDef* bodyDef;
		b2Body* body;
		b2FixtureDef* fixtureDef;
		b2Fixture* fixture;
		b2PolygonShape* shape;
	};

}

