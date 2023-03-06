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

	//transform->scale(0.999f);

}
