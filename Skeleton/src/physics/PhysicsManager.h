#pragma once

#include <Singleton.h>
#include <Vector2D.h>
#include <vector>
#include <unordered_map>
#include <stack>

#include "EditorExport.h"

class b2Vec2;
class b2World;
class b2Body;
class b2ContactListener;

namespace Physics {

	class DebugDraw;

	EditorManager PhysicsManager : public Utilities::Singleton<PhysicsManager> {

		friend Singleton<PhysicsManager>;

	public:

		~PhysicsManager();

		b2World* getWorld();

		void setGravity(const Utilities::Vector2D& gravity);
		Utilities::Vector2D getGravity();

		void fixedUpdate(float fixedDeltaTime);

	publish:
		void debugDraw();

		void enableDebugDraw(bool enable);

		void handleBodies();

	public:
		void setContactListener(b2ContactListener* contactListener);

		float getScreenToWorldFactor();

		void addCollisionLayer(const std::string& layerName);

		void removeCollisionLayer(const std::string& layerName);

		int getLayerBits(const std::string& layerName);

		int getMaskBits(const std::string& layerName);

		void setCollisionBetweenLayers(const std::string& layerNameA, const std::string& layerNameB, bool collide);

		bool layersExists(const std::string& layerName);

		void setBodyEnabled(b2Body* body, bool enabled);

		void setBodyRemoved(b2Body* body);

	private:

		void initPhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations);

		PhysicsManager();

		PhysicsManager(Utilities::Vector2D gravity, int velocityIterations = 6, int positionIterations = 3);

		int velocityIterations;
		int positionIterations;

		b2Vec2* gravity;
		b2World* world;

		bool debugDrawEnabled;

		// Disabled/Enable bodies handling
		std::vector<b2Body*> disabledBodies;

		std::vector<b2Body*> enabledBodies;

		std::vector<b2Body*> removedBodies;

		// Debug draw class inherited from b2draw
		DebugDraw* b2draw;

		float screenToWorldFactor;

		// Collision Matrix
		std::vector<std::vector<bool>> collision_matrix;

		std::unordered_map<std::string, int> layers;

		std::stack<int> freeLayers;

		int layersCount;

	};
}
