#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "DebugDraw.h"
#include "box2d/b2_contact.h"

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

		freeLayers = std::stack<int>();

		for (int i = MAX_COLLISION_LAYERS - 1; i >= 0; i--) freeLayers.push(i);

		addCollisionLayer("Default");

	}

	PhysicsManager::PhysicsManager() {
		initPhysicsManager(Utilities::Vector2D(0, 9.8f), 6, 3);
	}

	void PhysicsManager::setBodyEnabled(b2Body* body, bool enabled) {

		if (enabled)
			enabledBodies.push_back(body);
		else
			disabledBodies.push_back(body);

	}

	void PhysicsManager::setBodyRemoved(b2Body* body) {

		removedBodies.push_back(body);
	}

	void PhysicsManager::handleBodies() {

		for (int i = 0; i < disabledBodies.size(); i++) disabledBodies[i]->SetEnabled(false);

		disabledBodies.clear();

		for (int i = 0; i < enabledBodies.size(); i++) enabledBodies[i]->SetEnabled(true);

		enabledBodies.clear();

		for (int i = 0; i < removedBodies.size(); i++) world->DestroyBody(removedBodies[i]);

		removedBodies.clear();

	}

	PhysicsManager::PhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations) {
		initPhysicsManager(gravity, velocityIterations, positionIterations);
	}

	void PhysicsManager::addCollisionLayer(const std::string& layerName) {

		// Can not add more layers than the maximum
		assert(layersCount < MAX_COLLISION_LAYERS, "Numero maximo de capas de colision alcanzado!");

		// Layers name must be unique
		assert(layers.find(layerName) == layers.end(), "Ya existe una capa con el mismo nombre!");

		layers.insert(std::make_pair(layerName, freeLayers.top()));

		freeLayers.pop();

		layersCount++;

	}

	void PhysicsManager::removeCollisionLayer(const std::string& layerName) {

		// Default layer can not be deleted, so at least there is always one layer
		assert(layerName != "Default", "No se puede eliminar la capa por defecto!");

		// A non-existing layer can not be deleted
		assert(layers.find(layerName) != layers.end(), "La capa con nombre " + layerName + " no existe!");

		int removedLayerIndex = layers.at(layerName);

		layers.erase(layerName);

		for (const auto& par : layers) {
			if (par.second > removedLayerIndex)
				layers[par.first]--;
		}

		freeLayers.push(removedLayerIndex);

		for (int i = removedLayerIndex; i < layersCount - 1; i++) {
			collision_matrix[i] = collision_matrix[i + 1];
		}

		for (int i = 0; i < layersCount; i++) {
			for (int c = removedLayerIndex; c < layersCount - 1; c++) {
				collision_matrix[i][c] = collision_matrix[i][c + 1];
			}
		}

		layersCount--;
	}

	int PhysicsManager::getMaskBits(const std::string& layerName) {

		int layerN = layers.at(layerName);

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

		int layerA = layers.at(layerNameA);
		int layerB = layers.at(layerNameB);

		collision_matrix[layerA][layerB] = collide;
		collision_matrix[layerB][layerA] = collide;

	}

	bool PhysicsManager::layersExists(const std::string& layerName) {
		return layers.find(layerName) != layers.end();
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
