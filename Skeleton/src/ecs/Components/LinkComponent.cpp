#include "LinkComponent.h"
#include "InputManager.h"
#include "Entity.h"
#include "Transform.h"
#include "BoxBody.h"
#include "SoundEmitter.h"

namespace ECS {

	void LinkComponent::start() {
		transform = this->getEntity()->getComponent<Transform>();

		body = this->getEntity()->getComponent<BoxBody>();

		soundEmitter = this->getEntity()->getComponent<SoundEmitter>();

		onGround = false;
	}

	void LinkComponent::update(float deltaTime) {

		auto im = Input::InputManager::instance();

		if (im->keyDownEvent()) {

			if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
				body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
				body->applyLinearImpulseToCenter({ 0, -300 });

				soundEmitter->play();
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
