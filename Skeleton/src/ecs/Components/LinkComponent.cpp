#include "LinkComponent.h"
#include "InputManager.h"
#include "SoundEmitter.h"
#include "Transform.h"
#include "BoxBody.h"
#include "Entity.h"

namespace ECS {

	void LinkComponent::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<BoxBody>();

		sound = this->getEntity()->getComponent<SoundEmitter>();

		onGround = false;
	}


	void LinkComponent::update(float deltaTime) {

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->applyLinearImpulseToCenter({ 0, -300 });

				sound->play();

			}

		}

	}

	void LinkComponent::onCollisionEnter(Entity* b) {
		onGround = true;
	}

	void LinkComponent::onCollisionExit(Entity* b) {
		onGround = false;
	}

}
