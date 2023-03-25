#include "TestComponent.h"
#include "InputManager.h"
#include <iostream>
#include <Vector2D.h>
#include "CircleBody.h"
#include "Entity.h"
#include "Transform.h"


void ECS::TestComponent::start() {
	transform = this->getEntity()->getComponent<Transform>();

	body = this->getEntity()->getComponent<CircleBody>();

	onGround = false;
}

void ECS::TestComponent::update(float deltaTime) {

	//transform->translate(1, 0);
	//transform->setScale(Utilities::Vector2D(transform->getScale()->getX() * 0.9995f, transform->getScale()->getY() * 0.9995f));
	//transform->rotate(deltaTime * 20);

	auto im = Input::InputManager::instance();

	if (im->isJoystickButtonEventDown() && im->getJoystickButtonState(Input::InputManager::X)) {
		body->applyLinearImpulseToCenter({ 0, -30 });
	}

}

void ECS::TestComponent::fixedUpdate(float fixedDeltaTime) {
	

}

void ECS::TestComponent::onCollisionEnter(Entity* b) {

	std::cout << "Comenzo el choque!" << std::endl;
	std::cout << b->getEntityName() << std::endl;

	onGround = true;
}

void ECS::TestComponent::onCollisionExit(Entity* b) {

	std::cout << "Termino el choque!" << std::endl;
	std::cout << b->getEntityName() << std::endl;

	onGround = false;
}
