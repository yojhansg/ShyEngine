#include "TestComponent.h"
#include "InputManager.h"
#include <iostream>
#include <Vector2D.h>
#include "Entity.h"
#include "Transform.h"


void ECS::TestComponent::start() {
	transform = this->getEntity()->getComponent<Transform>();

	//rb = this->getEntity()->getComponent<Rigidbody>();

	onGround = false;
}

void ECS::TestComponent::update(float deltaTime) {

	//transform->translate(1, 0);
	transform->setScale(Utilities::Vector2D(transform->getScale()->getX() * 0.9995f, transform->getScale()->getY() * 0.9995f));
	//transform->rotate(deltaTime * 20);

	/*auto im = InputManager::InputManager::instance();

	if (im->isKeyDown(SDL_SCANCODE_SPACE)) {
		rb->setLinearVelocity(Utilities::Vector2D(rb->getLinearVelocity().getX(), -4));
	}
	*/
}

void ECS::TestComponent::fixedUpdate(float fixedDeltaTime) {
	
}

void ECS::TestComponent::onCollisionEnter(Entity* b) {

	std::cout << "Comenzo el choque!" << std::endl;
	std::cout << b->getEntityName() << std::endl;

	onGround = true;
}

void ECS::TestComponent::onCollisionExit(Entity* b) {

	/*std::cout << "Termino el choque!" << std::endl;
	std::cout << a->getEntityName() << std::endl;*/

	onGround = false;
}
