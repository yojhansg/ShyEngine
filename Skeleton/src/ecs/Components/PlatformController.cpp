#include "PlatformController.h"
#include "Entity.h"
#include "Transform.h"
#include "BoxBody.h"
#include "CircleBody.h"
#include "EdgeBody.h"

#include "ConsoleManager.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "box2d/b2_contact.h"

namespace ECS {

	PlatformController::PlatformController() {

		im = nullptr;
		transform = nullptr;
		body = nullptr;

		velocity = 0.0f;
		direction = 0.0f;

		onGround = false;
		hasJumped = false;

		jumpForce = 1.0f;
		maxVelocity = 5.0f;
		horizontalDamping = 0.0f;
		avalaibleJumps = 1;
		nJumps = 0;
	}

	void PlatformController::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		BoxBody* bBody = this->getEntity()->getComponent<BoxBody>();
		CircleBody* cBody = this->getEntity()->getComponent<CircleBody>();

		if (bBody == nullptr && cBody == nullptr) {
			printError("Missing physic body (Circle o Box)", "The entity must contain a physical body in case you want it to move with physics.");
			this->remove();
			return;
		}

		if (bBody != nullptr)
			body = bBody;
		else if (cBody != nullptr)
			body = cBody;

	}

	void PlatformController::update(float dt) {

		if (!hasJumped && nJumps > 0 && im->Jump()) {
			hasJumped = true;
			nJumps--;
		}

	}

	void PlatformController::fixedUpdate(float fixedDt) {

		direction = im->HorizontalMovement();

		// Salto
		if (hasJumped) {
			float impulse = body->getMass() * jumpForce * fixedDt;

			// Eliminar la velocidad actual en Y
			body->setLinearVelocity(body->getLinearVelocity().getX(), 0);

			body->applyLinearImpulseToCenter({ 0, impulse });

			hasJumped = false;

		}

		// Movimiento horizontal
		body->applyLinearImpulseToCenter({ direction * velocity * fixedDt, 0 });

		auto vel = body->getLinearVelocity();

		// Limitar velocidad maxima
		if (vel.getX() > maxVelocity)
			body->setLinearVelocity(maxVelocity, vel.getY());
		else if (vel.getX() < -maxVelocity)
			body->setLinearVelocity(-maxVelocity, vel.getY());

		vel = body->getLinearVelocity();

		// Aplicar damping eje X
		float newX = vel.getX() * (1 - horizontalDamping);
		vel.setX(newX);

		body->setLinearVelocity(vel.getX(), vel.getY());

	}

	void PlatformController::onCollisionEnter(Entity* ent) {

		auto other = PhysicBody::GetComponentFromEntity(ent);

		if (body->collidesWith(other) && other->getCollisionLayer() == platformLayer) {
			if (body->getCollisionNormal().getY() > 0.5f) {
				onGround = true;
				nJumps = avalaibleJumps;
			}
		}

	}

	void PlatformController::onCollisionExit(Entity* ent) {

		auto other = PhysicBody::GetComponentFromEntity(ent);

		if (body->collidesWith(other) && other->getCollisionLayer() == platformLayer) {
			onGround = false;
			hasJumped = false;
		}


	}

}
