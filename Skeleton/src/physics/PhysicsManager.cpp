#include "PhysicsManager.h"
#include "box2d/b2_contact.h"
#include "box2d/box2d.h"
#include "DebugDraw.h"

#include <ConsoleManager.h>

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

	PhysicsManager::PhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations) {
		initPhysicsManager(gravity, velocityIterations, positionIterations);
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

	void PhysicsManager::handleBodies() {

		for (int i = 0; i < disabledBodies.size(); i++) disabledBodies[i]->SetEnabled(false);
		disabledBodies.clear();

		for (int i = 0; i < enabledBodies.size(); i++) enabledBodies[i]->SetEnabled(true);
		enabledBodies.clear();

	}

	void PhysicsManager::addCollisionLayer(const std::string& layerName) {

		// Check if the maximun number of physics layers was reached
		if (layersCount >= MAX_COLLISION_LAYERS) {
			Console::Output::PrintError("Physics layers", "Maximum number of layers reached.");
			return;
		}

		// Check if there is already an exisiting layer with the same name
		if (layers.find(layerName) != layers.end()) {
			Console::Output::PrintError("Physics layers", "There is already a layer with the same name. Layers must be unique.");
			return;
		}

		layers.insert(std::make_pair(layerName, freeLayers.top()));

		freeLayers.pop();

		layersCount++;

	}

	void PhysicsManager::removeCollisionLayer(const std::string& layerName) {

		// Check if the layer to be deleted is not the default layer
		if (layerName == "Default") {
			Console::Output::PrintError("Physics layers", "The default layer cannot be deleted.");
			return;
		}

		// A non-existing layer can not be deleted
		if (layers.find(layerName) == layers.end()) {
			Console::Output::PrintError("Physics layers", "The layer with name " + layerName + " doesn't exist.");
			return;
		}

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

	bool PhysicsManager::layersCollide(const std::string& layerNameA, const std::string& layerNameB) {

		int layerA = layers.at(layerNameA);
		int layerB = layers.at(layerNameB);

		return collision_matrix[layerA][layerB];

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

	void PhysicsManager::setGravity(cVector2D g) {
		gravity->Set(g.getX(), g.getY());
		world->SetGravity(*gravity);
	}

	Utilities::Vector2D PhysicsManager::getGravity() {
		return Utilities::Vector2D(gravity->x, gravity->y);
	}

	PhysicsManager::~PhysicsManager() {

		handleBodies();

		delete gravity;
		delete world;
		delete b2draw;
	}

}
