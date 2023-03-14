#include "ECS.h"
#include "ContactListener.h"
#include <PhysicsManager.h>

namespace ECS {

	ECS::ECS() {
		contactListener = new ContactListener();

		PhysicsManager::PhysicsManager::instance()->setContactListener(contactListener);
	}

	ECS::~ECS() {
		delete contactListener;
	}

}