#pragma once

#include <Singleton.h>
#include <Vector2D.h>

#include "box2d/box2d.h"

namespace PhysicsManager {

	class PhysicsManager : public Singleton<PhysicsManager> {

		friend Singleton<PhysicsManager>;

	public:

		~PhysicsManager();

		void setGravity(const Vector2D& gravity);
		Vector2D getGravity();

		void update();

		void testBox2D();

	private:

		PhysicsManager();

		float timeStep;
		int32 velocityIterations;
		int32 positionIterations;

		b2Vec2* gravity;
		b2World* world;

		// Ground
		b2BodyDef* groundBodydef; // Body definition, sets the position, type (static, dynamic, kinematic) ...
		b2Body* groundBody; // The actual body object
		b2PolygonShape* groundShape; // A shape to attach to the object through a fixture

		// Box
		b2BodyDef* boxBodyDef;
		b2Body* boxBody;
		b2PolygonShape* boxShape;
		b2FixtureDef* boxFixtureDef;
	};

}
