#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "DebugDraw.h"
#include "box2d/b2_contact.h"

namespace PhysicsManager {

	void PhysicsManager::initPhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations) {
		this->gravity = new b2Vec2(gravity.getX(), gravity.getY());
		world = new b2World(*this->gravity);

		this->screenToWorldFactor = 100.0f;

		this->velocityIterations = velocityIterations;
		this->positionIterations = positionIterations;

		// Debug Draw
		debugDrawEnabled = false;

		b2draw = new DebugDraw();
		world->SetDebugDraw(b2draw);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_centerOfMassBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_pairBit;
		b2draw->SetFlags(flags);
	}

	PhysicsManager::PhysicsManager() {
		initPhysicsManager(Utilities::Vector2D(0, 9.8f), 6, 3);
	}

	PhysicsManager::PhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations) {
		initPhysicsManager(gravity, velocityIterations, positionIterations);
	}

	void PhysicsManager::setContactListener(b2ContactListener* contactListener) {
		world->SetContactListener(contactListener);
	}

	void PhysicsManager::fixedUpdate(float fixedDeltaTime) {
		world->Step(fixedDeltaTime, velocityIterations, positionIterations);
	}

	void PhysicsManager::debugDraw() {
	#ifdef _DEBUG
		if (debugDrawEnabled)
			 world->DebugDraw();
	#endif
	}

	void PhysicsManager::enableDebugDraw(bool enable) {
		debugDrawEnabled = enable;
	}

	float PhysicsManager::getScreenToWorldFactor() {
		return screenToWorldFactor;
	}

	b2World* PhysicsManager::getWorld() {
		return world;
	}

	void PhysicsManager::setGravity(const Utilities::Vector2D& g) {
		gravity->Set(g.getX(), g.getY());
		world->SetGravity(*gravity);
	}

	Utilities::Vector2D PhysicsManager::getGravity() {
		return Utilities::Vector2D(gravity->x, gravity->y);
	}

	PhysicsManager::~PhysicsManager() {
		delete gravity;
		delete world;
		delete b2draw;
	}
}
