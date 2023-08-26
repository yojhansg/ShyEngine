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

		void setGravity(const Utilities::Vector2D& gravity);

		void addCollisionLayer(const std::string& layerName);

		void removeCollisionLayer(const std::string& layerName);

		void setCollisionBetweenLayers(const std::string& layerNameA, const std::string& layerNameB, bool collide);

		bool layersCollide(const std::string& layerNameA, const std::string& layerNameB);

		bool layersExists(const std::string& layerName);

	private:
		
		void showMatrix();

		void initPhysicsManager(Utilities::Vector2D gravity, std::vector<std::string> dataLayers, std::vector<std::vector<bool>> dataMatrix, int velocityIterations, int positionIterations);

		PhysicsManager();

		PhysicsManager(Utilities::Vector2D gravity, std::vector<std::string> dataLayers, std::vector<std::vector<bool>> dataMatrix, int velocityIterations = 6, int positionIterations = 3);

		void LoadMatrixFromData(std::vector<std::string> dataLayers, std::vector<std::vector<bool>> dataMatrix);

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
