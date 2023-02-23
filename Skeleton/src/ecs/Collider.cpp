#include "Collider.h"
#include "Transform.h"
#include "Image.h"
#include "Entity.h"
#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include <cassert>

ECS::Collider::Collider() {

	this->friction = 0;
	this->bounciness = 0;

	isTrigger_ = false;

	size = Utilities::Vector2D(1, 1);

	transform = nullptr; world = nullptr;
	body = nullptr; shape = nullptr;
	bodyDefinition = nullptr;
}

ECS::Collider::~Collider() {
	delete shape;
	delete bodyDefinition;

	world->DestroyBody(body);
}

void ECS::Collider::init() {
	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef();
	shape = new b2PolygonShape();
}

void ECS::Collider::start() {

	transform = this->getEntity()->getComponent<Transform>();

	size = transform->getScale();

	Image* image = this->getEntity()->getComponent<Image>();

	// Collider size = Image size if Image component exists in the entity
	if (image != nullptr)
		size.set(image->getWidth(), image->getHeight());

	shape->SetAsBox(size.getX() / 2, size.getY() / 2);

	bodyDefinition->type = b2_staticBody;
	bodyDefinition->position.Set(transform->getPosition().getX() + size.getX() / 2, transform->getPosition().getY() + size.getY() / 2);

	body = world->CreateBody(bodyDefinition);

	body->CreateFixture(shape, 0.0f);
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

b2Body* ECS::Collider::getBody() {
	return body;
}

b2PolygonShape* ECS::Collider::getShape() {
	return shape;
}
