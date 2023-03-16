#include "ContactListener.h"
#include <PhysicsManager.h>
#include <box2d/b2_contact.h>
#include <Entity.h>
#include <iostream>

namespace ECS {

	ContactListener::ContactListener() {

		PhysicsManager::PhysicsManager::instance()->setContactListener(this);
	}

	void ContactListener::BeginContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*) contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		Entity* entA = static_cast<Entity*>(static_cast<void*>(punteroA));
		Entity* entB = static_cast<Entity*>(static_cast<void*>(punteroB));

		entA->onCollisionEnter(entA, entB);
		entB->onCollisionEnter(entB, entA);
	}

	void ContactListener::EndContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*) contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		Entity* entA = static_cast<Entity*>(static_cast<void*>(punteroA));
		Entity* entB = static_cast<Entity*>(static_cast<void*>(punteroB));

		entA->onCollisionExit(entA, entB);
		entB->onCollisionExit(entB, entA);
	}

}