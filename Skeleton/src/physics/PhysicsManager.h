#pragma once

#include <Singleton.h>
#include <Vector2D.h>
#include <box2d/b2_fixture.h>
#include "box2d/b2_draw.h"

class b2Vec2;
class b2World;
class b2BodyDef;
class b2Body;
class b2PolygonShape;

/*
* Tareas del PhysicsManager:
* 
* 1.- Crear el physicsWolrd con gravedad
* 2.- Implementar la simulación física
*
*/

namespace PhysicsManager {

	class DebugDraw : public b2Draw {
	public:
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf) {}
		void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {};
	};

	class PhysicsManager : public Singleton<PhysicsManager> {

		friend Singleton<PhysicsManager>;

	public:

		~PhysicsManager();

		void setGravity(const Vector2D& gravity);
		Vector2D getGravity();

		void update();

		void configureScene();

	private:

		PhysicsManager();

		float timeStep;
		int velocityIterations;
		int positionIterations;

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

		DebugDraw b2draw;
	};

}
