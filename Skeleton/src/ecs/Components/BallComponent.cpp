#include "BallComponent.h"
#include "InputManager.h"
#include "Entity.h"
#include "Transform.h"
#include "CircleBody.h"
#include "SoundEmitter.h"

namespace ECS {

	void BallComponent::start() {

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<CircleBody>();

		soundEmitter = this->getEntity()->getComponent<SoundEmitter>();

		onGround = false;

	}

	void BallComponent::update(float deltaTime) {

		auto im = Input::InputManager::instance();

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
				body->applyLinearImpulseToCenter({ 0, -50 });

				soundEmitter->play();
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
