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

	Utilities::Vector2D a = Utilities::Vector2D();
	a.setX(transform->getScale()->getX() - transform->getScale()->getX() * deltaTime / 10);
	a.setY(transform->getScale()->getY() - transform->getScale()->getY() * deltaTime / 10);

	transform->setScale(a);

}
