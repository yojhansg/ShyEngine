#include "TestComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <Vector2D.h>
#include "CircleBody.h"
#include "Entity.h"
#include "Transform.h"
#include "EdgeBody.h"
#include "BoxBody.h"
#include "SoundEmitter.h"


void ECS::TestComponent::start() {
	transform = this->getEntity()->getComponent<Transform>();

	body = this->getEntity()->getComponent<BoxBody>();

	soundEmitter = this->getEntity()->getComponent<SoundEmitter>();

	onGround = false;
}

void ECS::TestComponent::update(float deltaTime) {

	auto im = Input::InputManager::instance();

	if (im->keyDownEvent()) {

		if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
			body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
			body->applyLinearImpulseToCenter({ 0, -300 });

			soundEmitter->play();
		}

	}

}

void ECS::TestComponent::onCollisionEnter(Entity* b) {

	print("En el suelo", "TestComponent");

	onGround = true;
}

void ECS::TestComponent::onCollisionExit(Entity* b) {

	print("En el aire", "TestComponent");

	onGround = false;
}
