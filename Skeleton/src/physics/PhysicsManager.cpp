#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "DebugDraw.h"
#include "box2d/b2_contact.h"
#include <iostream>

const int MAX_COLLISION_LAYERS = 16;

namespace Physics {

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
		flags += b2Draw::e_centerOfMassBit;
		b2draw->SetFlags(flags);

		// Collision Matrix

		layersCount = 0;

		collision_matrix = std::vector<std::vector<bool>>(MAX_COLLISION_LAYERS, std::vector<bool>(MAX_COLLISION_LAYERS, true));

		addCollisionLayer("Default");

	}

	PhysicsManager::PhysicsManager() {
		initPhysicsManager(Utilities::Vector2D(0, 9.8f), 6, 3);
	}

	PhysicsManager::PhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations) {
		initPhysicsManager(gravity, velocityIterations, positionIterations);
	}

	void PhysicsManager::addCollisionLayer(const std::string& layerName) {

		assert(layersCount < MAX_COLLISION_LAYERS, "N�mero m�ximo de capas de colision alcanzado!");

		layers.insert(std::make_pair(layerName, layersCount));

		layersCount++;

	}

	void PhysicsManager::removeCollisionLayer(const std::string& layerName) {

		assert(layersCount > 0, "No se puede eliminar la capa por defecto!");

		layers.erase(layerName);

		layersCount--;
	}

	int PhysicsManager::getLayerNumber(const std::string& layerName) {
		return layers.at(layerName);
	}

	int PhysicsManager::getMaskBits(const std::string& layerName) {

		int layerN = getLayerNumber(layerName);

		int flags = 0;

		for (int i = 0; i < layersCount; i++) {

			if (collision_matrix[layerN][i])
				flags += std::pow(2, i);

		}

		return flags;
	}

	int PhysicsManager::getLayerBits(const std::string& layerName) {
		return std::pow(2, layers.at(layerName));
	}

	void PhysicsManager::setCollisionBetweenLayers(const std::string& layerNameA, const std::string& layerNameB, bool collide) {

		int layerA = getLayerNumber(layerNameA);
		int layerB = getLayerNumber(layerNameB);

		collision_matrix[layerA][layerB] = collide;
		collision_matrix[layerB][layerA] = collide;

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
