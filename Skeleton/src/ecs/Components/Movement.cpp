#include "Movement.h"
#include "Entity.h"
#include "Transform.h"

#include "ConsoleManager.h"
#include "InputManager.h"

#include <iostream>

namespace ECS {

	Movement::Movement() {
		transform = nullptr;
	}

	void Movement::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		Console::Output::Print("Numero de mandos: ", std::to_string(im->getJoysticksConnected()));
	}

	void Movement::update(float dt) {

		if (im->isJoystickButtonEventDown()) {
			if (im->getJoystickButtonState(0, 0)) {
				std::cout << "Mando 1" << std::endl;
			}

			if (im->getJoystickButtonState(0, 1)) {
				std::cout << "Mando 2" << std::endl;
			}
		}

	}

}