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

		unsigned int* punteroA = (unsigned int*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*) contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (punteroA == nullptr || punteroB == nullptr) return;

		PhysicBody* phyA = static_cast<PhysicBody*>(static_cast<void*>(punteroA));
		PhysicBody* phyB = static_cast<PhysicBody*>(static_cast<void*>(punteroB));

		auto entA = phyA->getEntity();
		auto entB = phyB->getEntity();

		if (contact->GetFixtureA()->IsSensor()) {
			entA->onTriggerEnter(entB);
			phyA->setTriggerStay(true, entB);
		}
		else {
			entA->onCollisionEnter(entB);
			phyA->setCollisionStay(true, entB);
		}

		if (contact->GetFixtureB()->IsSensor()) {
			entB->onTriggerEnter(entA);
			phyB->setTriggerStay(true, entA);
		}
		else {
			entB->onCollisionEnter(entA);
			phyB->setCollisionStay(true, entA);
		}
	}

	void ContactListener::EndContact(b2Contact* contact) {

		unsigned int* punteroA = (unsigned int*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		unsigned int* punteroB = (unsigned int*) contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (punteroA == nullptr || punteroB == nullptr) return;

		PhysicBody* phyA = static_cast<PhysicBody*>(static_cast<void*>(punteroA));
		PhysicBody* phyB = static_cast<PhysicBody*>(static_cast<void*>(punteroB));

		auto entA = phyA->getEntity();
		auto entB = phyB->getEntity();

		if (contact->GetFixtureA()->IsSensor()) {
			entA->onTriggerExit(entB);
			phyA->setTriggerStay(false, entB);
		}
		else {
			entA->onCollisionExit(entB);
			phyA->setCollisionStay(false, entB);
		}

		if (contact->GetFixtureB()->IsSensor()) {
			entB->onTriggerExit(entA);
			phyB->setTriggerStay(false, entA);
		}
		else {
			entB->onCollisionExit(entA);
			phyB->setCollisionStay(false, entA);
		}
	}

}