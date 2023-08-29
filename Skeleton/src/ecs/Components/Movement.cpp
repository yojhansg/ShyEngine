#include "Movement.h"
#include "Entity.h"
#include "Transform.h"

#include "ConsoleManager.h"
#include "InputManager.h"

#include <iostream>
#include <Vector2D.h>

namespace ECS {

	Movement::Movement() {
		im = nullptr;
		transform = nullptr;

		velocity = 0.0f;
		clampDiagonalVelocity = false;
	}

	void Movement::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

	}

	void Movement::update(float dt) {

		Utilities::Vector2D movement = { im->KeyBoardHorizontalMovement() , im->KeyBoardVerticalMovement() };

		if (clampDiagonalVelocity && movement.magnitude() > 1)
			movement = movement.normalize();

		transform->Translate(movement * dt * velocity);

	}

}