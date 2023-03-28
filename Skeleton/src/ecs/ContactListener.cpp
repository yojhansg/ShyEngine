#include "ContactListener.h"
#include <PhysicsManager.h>
#include <box2d/b2_contact.h>
#include <Entity.h>

namespace ECS {

	ContactListener::ContactListener() {

		Physics::PhysicsManager::instance()->setContactListener(this);
	}

	void ContactListener::BeginContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*) contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (punteroA == nullptr || punteroB == nullptr) return;

		Entity* entA = static_cast<Entity*>(static_cast<void*>(punteroA));
		Entity* entB = static_cast<Entity*>(static_cast<void*>(punteroB));

		entA->onCollisionEnter(entB);
		entB->onCollisionEnter(entA);
	}

	void ContactListener::EndContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (punteroA == nullptr || punteroB == nullptr) return;

		Entity* entA = static_cast<Entity*>(static_cast<void*>(punteroA));
		Entity* entB = static_cast<Entity*>(static_cast<void*>(punteroB));

		entA->onCollisionExit(entB);
		entB->onCollisionExit(entA);
	}

}