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


using cString = std::string const&;

namespace Physics {

	class DebugDraw;

	EditorManager PhysicsManager : public Utilities::Singleton<PhysicsManager> {

		friend Singleton<PhysicsManager>;

	public:

		~PhysicsManager();

		b2World* getWorld();

		void fixedUpdate(float fixedDeltaTime);

		void debugDraw();

		void enableDebugDraw(bool enable);

		void handleBodies();

		void setContactListener(b2ContactListener* contactListener);

		float getScreenToWorldFactor();

		int getLayerBits(const std::string& layerName);

		int getMaskBits(const std::string& layerName);

		void setBodyEnabled(b2Body* body, bool enabled);

	publish:

		Utilities::Vector2D getGravity();

		void setGravity(cVector2D gravity);

		void addCollisionLayer(cString layerName);

		void removeCollisionLayer(cString layerName);

		void setCollisionBetweenLayers(cString layerNameA, cString layerNameB, bool collide);

		bool layersCollide(cString layerNameA, cString layerNameB);

		bool layersExists(cString layerName);

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
