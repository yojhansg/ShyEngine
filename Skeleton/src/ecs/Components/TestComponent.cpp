#include "TestComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <iostream>
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

	if (im->keyDownEvent() && im->isKeyDown(SDL_SCANCODE_SPACE) && onGround) {
		body->setLinearVelocity(body->getLinearVelocity().getX(), 0);
		body->applyLinearImpulseToCenter({ 0, -100 });
	}

}

void ECS::TestComponent::fixedUpdate(float fixedDeltaTime) {
	

}

void ECS::TestComponent::onCollisionEnter(Entity* b) {

	std::cout << "En el suelo" << std::endl;

	if (b->hasComponent<EdgeBody>())
		b->setActive(false);

	onGround = true;
}

void ECS::TestComponent::onCollisionExit(Entity* b) {

	std::cout << "En el aire" << std::endl;

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
