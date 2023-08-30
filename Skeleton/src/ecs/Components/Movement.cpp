#include "Movement.h"
#include "Entity.h"
#include "Transform.h"
#include "BoxBody.h"
#include "CircleBody.h"

#include "ConsoleManager.h"
#include "InputManager.h"

#include <iostream>
#include <Vector2D.h>

namespace ECS {

	Movement::Movement() {
		im = nullptr;
		transform = nullptr;
		body = nullptr;

		velocity = 0.0f;
		clampDiagonalVelocity = true;
		usePhysics = false;
	}

	void Movement::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		if (usePhysics) {

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

	}

	void Movement::update(float dt) {

		direction = { im->HorizontalMovement(), im->VerticalMovement() };

		if (clampDiagonalVelocity && direction.magnitude() > 0)
			direction = direction.normalize();

		if (usePhysics) return;

		transform->Translate(direction * dt * velocity);

	}

	void Movement::fixedUpdate(float fixedDt) {

		if (!usePhysics) return;
		
		body->setLinearVelocity(direction.getX() * fixedDt * velocity, direction.getY() * fixedDt * velocity);

	}

}