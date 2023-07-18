#include "LinkComponent.h"
#include "ParticleSystem.h"
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

		onGround = false;
	}


	void LinkComponent::update(float deltaTime) {

		if (im->keyDownEvent()) {

			if (onGround) {
				if (im->isKeyDown(SDL_SCANCODE_SPACE)) {
					body->applyLinearImpulseToCenter({ 0, -300 });
				}
				else if (im->isKeyDown(SDL_SCANCODE_A)) {
					body->applyLinearImpulseToCenter({ -7, 0 });
				}
				else if (im->isKeyDown(SDL_SCANCODE_D)) {
					body->applyLinearImpulseToCenter({ 7, 0 });
				}
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
