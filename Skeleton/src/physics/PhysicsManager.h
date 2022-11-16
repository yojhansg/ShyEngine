#pragma once

#include <Singleton.h>
#include <Vector2D.h>
#include <box2d/b2_fixture.h>

class b2Vec2;
class b2World;
class b2BodyDef;
class b2Body;
class b2PolygonShape;

namespace PhysicsManager {

	class DebugDraw;

	class PhysicsManager : public Utilities::Singleton<PhysicsManager> {

		friend Singleton<PhysicsManager>;

	public:

		~PhysicsManager();

		void setGravity(const Utilities::Vector2D& gravity);
		Utilities::Vector2D getGravity();

		void fixedUpdate(float fixedDeltaTime);

		void debugDraw(); // Not working

		void enableDebugDraw(bool enable);

		void configureScene();

	private:

		PhysicsManager();

		int velocityIterations;
		int positionIterations;

		b2Vec2* gravity;
		b2World* world;

		//bool debugDrawEnabled;

		//// Ground
		//b2BodyDef* groundBodydef; // Body definition, sets the position, type (static, dynamic, kinematic) ...
		//b2Body* groundBody; // The actual body object
		//b2PolygonShape* groundShape; // A shape to attach to the object through a fixture

		//// Box
		//b2BodyDef* boxBodyDef;
		//b2Body* boxBody;
		//b2PolygonShape* boxShape;
		//b2FixtureDef* boxFixtureDef;

		//// Debug draw class inherited from b2draw
		//DebugDraw* b2draw;
	};
}
