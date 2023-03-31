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

		if (contact->GetFixtureA()->IsSensor())
			entA->onTriggerEnter(entB);
		else 
			entA->onCollisionEnter(entB);

		if (contact->GetFixtureB()->IsSensor())
			entB->onTriggerEnter(entA);
		else
			entB->onCollisionEnter(entA);
	}

	void ContactListener::EndContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (punteroA == nullptr || punteroB == nullptr) return;

		Entity* entA = static_cast<Entity*>(static_cast<void*>(punteroA));
		Entity* entB = static_cast<Entity*>(static_cast<void*>(punteroB));

		if (contact->GetFixtureA()->IsSensor())
			entA->onTriggerExit(entB);
		else
			entA->onCollisionExit(entB);

		if (contact->GetFixtureB()->IsSensor())
			entB->onTriggerExit(entA);
		else
			entB->onCollisionExit(entA);
	}

}