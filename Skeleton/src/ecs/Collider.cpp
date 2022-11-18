#include "Collider.h"
#include "Transform.h"
#include "Image.h"
#include "Entity.h"
#include <cassert>

ECS::Collider::Collider() {

	this->friction = 0;
	this->bounciness = 0;

	size = Utilities::Vector2D(1, 1);

	isTrigger_ = false;
}

void ECS::Collider::start() {

	size = this->getEntity()->getComponent<Transform>()->getScale();

	Image* image = this->getEntity()->getComponent<Image>();

	// Collider size = Image size if Image component exists in the entity
	if (image != nullptr)
		size.set(image->getWidth(), image->getHeight());
}

void ECS::Collider::setTrigger(bool trigger) {
	isTrigger_ = trigger;
}

bool ECS::Collider::isTrigger() {
	return isTrigger_;
}

void ECS::Collider::setFriction(float friction) {
	this->friction = friction;
}

float ECS::Collider::getFriction() {
	return friction;
}

void ECS::Collider::setBounciness(float bounciness) {
	this->bounciness = bounciness;
}

float ECS::Collider::getBounciness() {
	return bounciness;
}

Utilities::Vector2D ECS::Collider::getSize() {
	return size;
}

void ECS::Collider::setSize(Utilities::Vector2D size) {
	this->size = size;
}
