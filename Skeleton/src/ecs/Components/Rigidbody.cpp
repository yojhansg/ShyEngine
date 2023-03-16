#include "Rigidbody.h"
#include "box2d/box2d.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysicsManager.h"
#include <iostream>

ECS::Rigidbody::Rigidbody() {

	screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();

	this->bodyType = STATIC;
	this->mass = 1;

	this->linearDrag = 0;
	this->angularDrag = 0;

	this->gravityScale = 1;

	transform = nullptr; collider = nullptr;

	// Box 2D
	world = nullptr;

	body = nullptr; fixture = nullptr;

}

ECS::Rigidbody::~Rigidbody() {}

void ECS::Rigidbody::init() {

	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	transform = this->getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");

	collider = this->getEntity()->getComponent<Collider>();
	assert(collider != nullptr, "La entidad debe contener un componente Collider para poder añadir componente Rigidbody");

	body = collider->getBody();

	fixture = collider->getFixture();
}

void ECS::Rigidbody::fixedUpdate(float fixedDeltaTime) {

	transform->setPosition(body->GetPosition().x * screenToWorldFactor, body->GetPosition().y * screenToWorldFactor);
	transform->setRotation(body->GetAngle() * (180 / b2_pi));
}

void ECS::Rigidbody::setBodyType(BODY_TYPE type) { 

	b2BodyType bodyType = b2_staticBody;

	switch (type)
	{
	case ECS::Rigidbody::STATIC:
		bodyType = b2_staticBody;
		break;
	case ECS::Rigidbody::KINEMATIC:
		bodyType = b2_kinematicBody;
		break;
	case ECS::Rigidbody::DINAMIC:
		bodyType = b2_dynamicBody;
		break;
	default:
		break;
	}

	body->SetType(bodyType);
}

void ECS::Rigidbody::setLinearDrag(float drag) {
	body->SetLinearDamping(drag);
}

void ECS::Rigidbody::setAngularDrag(float drag) {
	body->SetAngularDamping(drag);
}

void ECS::Rigidbody::setMass(float mass) {

	if (collider != nullptr)
		fixture->SetDensity(mass / collider->getSize().volume());
	else
		fixture->SetDensity(mass);
}

void ECS::Rigidbody::setGravityScale(float scale) {
	body->SetGravityScale(scale);
}

void ECS::Rigidbody::applyForce(const Utilities::Vector2D& force) {

	b2Vec2 p = body->GetWorldCenter();

	body->ApplyForce(b2Vec2(force.getX(), force.getY()), p, true);
}

void ECS::Rigidbody::setLinearVelocity(const Utilities::Vector2D& vel) {

	body->SetLinearVelocity(b2Vec2(vel.getX(), vel.getY()));
}

Utilities::Vector2D ECS::Rigidbody::getLinearVelocity() {

	b2Vec2 v = body->GetLinearVelocity();

	return Utilities::Vector2D(v.x, v.y);
}
