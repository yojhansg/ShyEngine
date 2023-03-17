#include "Collider.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "Transform.h"
#include "box2d/b2_world.h"

ECS::Collider::Collider() {

	screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();

	world = nullptr;

	transform = nullptr;

	trigger = false;
	friction = 0;
	bounciness = 0;
	rotationFreezed = false;

	body = nullptr; 
	bodyDefinition = nullptr; 
	fixture = nullptr; 

	position = nullptr;
	rotation = nullptr;
	scale = nullptr;
}

ECS::Collider::~Collider() {
	delete bodyDefinition;
}

void ECS::Collider::init() {
	transform = this->getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");

	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef();

	position = transform->getPosition();
	rotation = transform->getRotation();
	scale = transform->getScale();
}

void ECS::Collider::setTrigger(bool trigger) {
	this->trigger = trigger;
}

bool ECS::Collider::isTrigger() {
	return trigger;
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

void ECS::Collider::setRotationFreezed(bool freezed) {
	body->SetFixedRotation(freezed);
}

bool ECS::Collider::isRotationFreezed() {
	return body->IsFixedRotation();
}

b2Body* ECS::Collider::getBody() {
	return body;
}

b2Fixture* ECS::Collider::getFixture() {
	return fixture;
}
