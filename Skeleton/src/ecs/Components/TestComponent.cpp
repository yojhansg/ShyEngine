#include "TestComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <Vector2D.h>
#include "CircleBody.h"
#include "Entity.h"
#include "Transform.h"
#include "EdgeBody.h"
#include "BoxBody.h"


void ECS::TestComponent::start() {
	transform = this->getEntity()->getComponent<Transform>();

	body = this->getEntity()->getComponent<BoxBody>();

	onGround = false;
}

void ECS::TestComponent::update(float deltaTime) {

	auto im = Input::InputManager::instance();

	if (im->keyDownEvent()) {

		if (im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
			body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
			body->applyLinearImpulseToCenter({ 0, -100 });
		}
		else if (im->isKeyDown(SDL_SCANCODE_A)) {
		}

	}

}

void ECS::TestComponent::fixedUpdate(float fixedDeltaTime) {
	

}

void ECS::TestComponent::onCollisionEnter(Entity* b) {

	print("En el suelo", "TestComponent");

	auto edge = b->getComponent<EdgeBody>();

	if (edge != nullptr)
		b->setActive(false);

	onGround = true;
}

void ECS::TestComponent::onCollisionExit(Entity* b) {

	print("En el aire", "TestComponent");

	onGround = false;
}

void ECS::TestComponent::onTriggerEnter(Entity* b) {

	//std::cout << "Entro en el trigger" << std::endl;
}

void ECS::TestComponent::onTriggerExit(Entity* b) {

	//std::cout << "Salio del trigger" << std::endl;
}

void ECS::TestComponent::onCollisionStay(Entity* b) {

	//std::cout << "Colisionando!" << std::endl;
}

void ECS::TestComponent::onTriggerStay(Entity* b) {

	//std::cout << "Triggereando!" << std::endl;
}
