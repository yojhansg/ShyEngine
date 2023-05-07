#include "ContactListener.h"
#include <PhysicsManager.h>
#include <box2d/b2_contact.h>
#include "Components/PhysicBody.h"
#include <Entity.h>

namespace ECS {

	ContactListener::ContactListener() {

		Physics::PhysicsManager::instance()->setContactListener(this);
	}

	void ContactListener::BeginContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		PhysicBody* phyA = static_cast<PhysicBody*>(static_cast<void*>(punteroA));
		PhysicBody* phyB = static_cast<PhysicBody*>(static_cast<void*>(punteroB));

		if (phyA == nullptr || phyB == nullptr) return;

		auto entA = phyA->getEntity();
		auto entB = phyB->getEntity();

		if (entA == nullptr || entB == nullptr) return;

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()) {
			entA->onTriggerEnter(entB);
			entB->onTriggerEnter(entA);

			phyA->setTriggerStay(true, entB);
			phyB->setTriggerStay(true, entA);
		}
		else {
			entA->onCollisionEnter(entB);
			entB->onCollisionEnter(entA);

			phyA->setCollisionStay(true, entB);
			phyB->setCollisionStay(true, entA);
		}

	}

	void ContactListener::EndContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		PhysicBody* phyA = static_cast<PhysicBody*>(static_cast<void*>(punteroA));
		PhysicBody* phyB = static_cast<PhysicBody*>(static_cast<void*>(punteroB));

		if (phyA == nullptr || phyB == nullptr) return;

		auto entA = phyA->getEntity();
		auto entB = phyB->getEntity();

		if (entA == nullptr || entB == nullptr) return;

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()) {
			entA->onTriggerExit(entB);
			entB->onTriggerExit(entA);

			phyA->setTriggerStay(false, entB);
			phyB->setTriggerStay(false, entA);
		}
		else {
			entA->onCollisionExit(entB);
			entB->onCollisionExit(entA);

			phyA->setCollisionStay(false, entB);
			phyB->setCollisionStay(false, entA);
		}
	}

}