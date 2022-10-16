#include <iostream>
#include "PhysicsManager.h"

namespace PhysicsManager {

	PhysicsManager::PhysicsManager() {
		gravity = new b2Vec2(0.0f, -9.8f);
		world = new b2World(*gravity);

		timeStep = 1.0f / 60.0f;
		velocityIterations = 6;
		positionIterations = 3;

		groundBodydef = nullptr;
		groundBody = nullptr;
		groundShape = nullptr;

		boxBodyDef = nullptr;
		boxBody = nullptr;
		boxShape = nullptr;
		boxFixtureDef = nullptr;
	}

	PhysicsManager::~PhysicsManager() {
		delete gravity;
		delete world;

		delete groundBodydef; groundBodydef = nullptr;
		groundBody = nullptr;
		delete groundShape; groundShape = nullptr;

		delete boxBodyDef; boxBodyDef = nullptr;
		boxBody = nullptr;
		delete boxShape; boxShape = nullptr;
		delete boxFixtureDef; boxFixtureDef = nullptr;
	}

	void PhysicsManager::update() {
		world->Step(timeStep, velocityIterations, positionIterations);
		printf("%4.2f %4.2f %4.2f\n", boxBody->GetPosition().x, boxBody->GetPosition().y, boxBody->GetAngle());
	}

	void PhysicsManager::testBox2D() {

		// Ground (Static)
		groundBodydef = new b2BodyDef();
		groundBodydef->position.Set(0.0f, -1.0f);

		groundBody = world->CreateBody(groundBodydef);

		groundShape = new b2PolygonShape();
		groundShape->SetAsBox(25.0f, 0.5f);

		groundBody->CreateFixture(groundShape, 0.0f);

		// Box (Dynamic)
		boxBodyDef = new b2BodyDef();
		boxBodyDef->type = b2_dynamicBody;
		boxBodyDef->position.Set(0.0f, 10.0f);

		boxBody = world->CreateBody(boxBodyDef);

		boxShape = new b2PolygonShape();
		boxShape->SetAsBox(1.0f, 1.0f);

		boxFixtureDef = new b2FixtureDef();
		boxFixtureDef->shape = boxShape;
		boxFixtureDef->density = 1.0f;
		boxFixtureDef->friction = 0.9f;

		boxBody->CreateFixture(boxFixtureDef);
	}

	void PhysicsManager::setGravity(const Vector2D& g) {
		gravity->Set(g.getX(), g.getY());
		world->SetGravity(*gravity);
	}

	Vector2D PhysicsManager::getGravity() {
		return Vector2D(gravity->x, gravity->y);
	}
}
