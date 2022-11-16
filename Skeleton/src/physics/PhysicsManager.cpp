#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "GLFW/glfw3.h"
#include "DebugDraw.h"

namespace PhysicsManager {

	PhysicsManager::PhysicsManager() {
		gravity = new b2Vec2(0.0f, -9.8f);
		world = new b2World(*gravity);

		velocityIterations = 6;
		positionIterations = 3;

		/*debugDrawEnabled = false;

		b2draw = new DebugDraw();

		world->SetDebugDraw(b2draw);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_centerOfMassBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_pairBit;
		b2draw->SetFlags(flags);

		timeStep = 1.0f / 50.0f;
		velocityIterations = 6;
		positionIterations = 3;

		groundBodydef = nullptr;
		groundBody = nullptr;
		groundShape = nullptr;

		boxBodyDef = nullptr;
		boxBody = nullptr;
		boxShape = nullptr;
		boxFixtureDef = nullptr;*/
	}

	PhysicsManager::~PhysicsManager() {
		delete gravity;
		delete world;
		/*delete b2draw;

		delete groundBodydef; groundBodydef = nullptr;
		groundBody = nullptr;
		delete groundShape; groundShape = nullptr;

		delete boxBodyDef; boxBodyDef = nullptr;
		boxBody = nullptr;
		delete boxShape; boxShape = nullptr;
		delete boxFixtureDef; boxFixtureDef = nullptr;*/
	}

	void PhysicsManager::fixedUpdate(float fixedDeltaTime) {
		world->Step(fixedDeltaTime, velocityIterations, positionIterations);

		// printf("%4.2f %4.2f %4.2f\n", boxBody->GetPosition().x, boxBody->GetPosition().y, boxBody->GetAngle());
	}

	void PhysicsManager::debugDraw() {
		/*if (debugDrawEnabled)
			 world->DebugDraw();*/
	}

	void PhysicsManager::enableDebugDraw(bool enable) {
		//debugDrawEnabled = enable;
	}

	void PhysicsManager::configureScene() {

		//// Ground (Static)
		//groundBodydef = new b2BodyDef();
		//groundBodydef->position.Set(0.0f, -0.5f);

		//groundBody = world->CreateBody(groundBodydef);

		//groundShape = new b2PolygonShape();
		//groundShape->SetAsBox(0.5f, 0.005f);

		//groundBody->CreateFixture(groundShape, 0.0f);

		//// Box (Dynamic)
		//boxBodyDef = new b2BodyDef();
		//boxBodyDef->type = b2_dynamicBody;
		//boxBodyDef->position.Set(0.0f, 1.0f);

		//boxBody = world->CreateBody(boxBodyDef);

		//boxShape = new b2PolygonShape();
		//boxShape->SetAsBox(0.01f, 0.01f);

		//boxFixtureDef = new b2FixtureDef();
		//boxFixtureDef->shape = boxShape;
		//boxFixtureDef->density = 1.0f;
		//boxFixtureDef->friction = 0.9f;

		//boxBody->CreateFixture(boxFixtureDef);
	}

	void PhysicsManager::setGravity(const Utilities::Vector2D& g) {
		gravity->Set(g.getX(), g.getY());
		world->SetGravity(*gravity);
	}

	Utilities::Vector2D PhysicsManager::getGravity() {
		return Utilities::Vector2D(gravity->x, gravity->y);
	}
}
