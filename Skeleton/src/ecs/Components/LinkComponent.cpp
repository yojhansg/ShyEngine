#include "LinkComponent.h"
#include "InputManager.h"
#include "Entity.h"
#include "Transform.h"
#include "BoxBody.h"
#include "MusicEmitter.h"

#include <iostream>

namespace ECS {

	void LinkComponent::init() {

		im = Input::InputManager::instance();

		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<BoxBody>();

		music = this->getEntity()->getComponent<MusicEmitter>();

		onGround = false;
	}


	void LinkComponent::update(float deltaTime) {

		if (music->isPlaying())
			std::cout << "Link Playing!" << std::endl;

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_A)) {
				music->play();
			}

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
				body->applyLinearImpulseToCenter({ 0, -300 });

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
