#include "TestComponent.h"
#include "InputManager.h"
#include <iostream>
#include <Vector2D.h>
#include "Entity.h"
#include "Transform.h"


void ECS::TestComponent::start() {
	transform = this->getEntity()->getComponent<Transform>();
}

void ECS::TestComponent::update(float deltaTime) {

	//transform->translate(1, 0);
	//transform->scale(1 - 0.001f);

}

void ECS::TestComponent::onCollisionEnter(Entity* a, Entity* b) {

	std::cout << "Comenzo el choque!" << std::endl;
	std::cout << a->getEntityName() << std::endl;
}

void ECS::TestComponent::onCollisionExit(Entity* a, Entity* b) {

	std::cout << "Termino el choque!" << std::endl;
	std::cout << a->getEntityName() << std::endl;
}
