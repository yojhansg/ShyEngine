#pragma once

#include <Singleton.h>
#include <Vector2D.h>

#include "EditorExport.h"

class b2Vec2;
class b2World;
class b2Body;
class b2ContactListener;

namespace PhysicsManager {

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

	public:
		void setContactListener(b2ContactListener* contactListener);

		float getScreenToWorldFactor();

	private:

		void initPhysicsManager(Utilities::Vector2D gravity, int velocityIterations, int positionIterations);

		PhysicsManager();

		PhysicsManager(Utilities::Vector2D gravity, int velocityIterations = 6, int positionIterations = 3);

		int velocityIterations;
		int positionIterations;

		b2Vec2* gravity;
		b2World* world;

		bool debugDrawEnabled;

		// Debug draw class inherited from b2draw
		DebugDraw* b2draw;

		float screenToWorldFactor;
	};
}
