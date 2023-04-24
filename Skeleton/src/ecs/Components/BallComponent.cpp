#include "BallComponent.h"
#include "InputManager.h"
#include "SoundEmitter.h"
#include "CircleBody.h"
#include "Transform.h"
#include "Entity.h"

namespace ECS {

	void BallComponent::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<CircleBody>();

		sound = this->getEntity()->getComponent<SoundEmitter>();

		onGround = false;

	}

	void BallComponent::update(float deltaTime) {

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->applyLinearImpulseToCenter({ 0, -50 });

				sound->play();

			}

		}

	}

	void BallComponent::onCollisionEnter(Entity* b) {
		onGround = true;
	}

	void BallComponent::onCollisionExit(Entity* b) {
		onGround = false;
	}

}
