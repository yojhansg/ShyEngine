#include "PhysicalMovement.h"

#include "Entity.h"
#include "Transform.h"

#include "ConsoleManager.h"
#include "InputManager.h"

#include <iostream>
#include <Vector2D.h>

namespace ECS {

	PhysicalMovement::PhysicalMovement() {
		im = nullptr;
		transform = nullptr;

		velocity = 0.0f;
		clampDiagonalVelocity = true;
	}

	void PhysicalMovement::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

	}

	void PhysicalMovement::update(float dt) {

		Utilities::Vector2D kb_movement = { im->KeyBoardHorizontalMovement() , im->KeyBoardVerticalMovement() };
		Utilities::Vector2D ct_movement = { im->ControllerHorizontalMovement() , im->ControllerVerticalMovement() };

		Utilities::Vector2D movement = kb_movement + ct_movement;

		if (clampDiagonalVelocity && movement.magnitude() > 0)
			movement = movement.normalize();

		transform->Translate(movement * dt * velocity);

	}

}

