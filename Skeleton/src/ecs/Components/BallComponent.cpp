#include "BallComponent.h"
#include "InputManager.h"
#include "Entity.h"
#include "Transform.h"
#include "CircleBody.h"
#include "MusicEmitter.h"

#include <iostream>

namespace ECS {

	void BallComponent::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<CircleBody>();

		music = this->getEntity()->getComponent<MusicEmitter>();

		onGround = false;

	}

	void BallComponent::update(float deltaTime) {

		if (music->isPlaying())
			std::cout << "Ball Playing!" << std::endl;

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_D)) {
				music->play();
			}

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
				body->applyLinearImpulseToCenter({ 0, -50 });

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
