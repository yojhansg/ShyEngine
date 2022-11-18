#include "Rigidbody.h"
#include "box2d/box2d.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysicsManager.h"

#include <iostream>

ECS::Rigidbody::Rigidbody() {

	this->bodyType = STATIC;
	this->mass = 1;

	this->linearDrag = 0;
	this->angularDrag = 0;

	this->gravityScale = 1;

	xMovementLocked = yMovementLocked = rotationLocked = false;

	transform = nullptr; collider = nullptr;

	// Box 2D
	world = nullptr;

	bodyDefinition = nullptr;
	body = nullptr;
	shape = nullptr;
	fixture = nullptr;
}

ECS::Rigidbody::~Rigidbody() {
	delete fixture;
	delete shape;
	delete bodyDefinition;

	world->DestroyBody(body);
}

void ECS::Rigidbody::init() {

	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef();
	shape = new b2PolygonShape();
	fixture = new b2FixtureDef();
}

void ECS::Rigidbody::start() {

	transform = this->getEntity()->getComponent<Transform>();
	collider = this->getEntity()->getComponent<Collider>();

	bodyDefinition->type = assingBodyType(bodyType);
	bodyDefinition->position.Set(transform->getPosition().getX(), transform->getPosition().getY());
	bodyDefinition->linearDamping = linearDrag;
	bodyDefinition->angularDamping = angularDrag;

	body = world->CreateBody(bodyDefinition);

	shape->SetAsBox(transform->getScale().getX() / 2, transform->getScale().getY() / 2);

	// Rigidbody size = Collider size if Collider Component exists in the entity
	if (collider != nullptr)
		shape->SetAsBox(collider->getSize().getX() / 2, collider->getSize().getY() / 2);


	switch (bodyDefinition->type) {
	case b2_staticBody:
		body->CreateFixture(shape, 0.0f);
		break;
	case b2_kinematicBody:
		// TODO
		break;
	case b2_dynamicBody:
		fixture->shape = shape;
		fixture->friction = collider != nullptr ? collider->getFriction() : 0;
		fixture->restitution = collider != nullptr ? collider->getBounciness() : 0;
		fixture->isSensor = collider != nullptr ? collider->isTrigger() : false;
		fixture->density = collider != nullptr ? mass / collider->getSize().magnitude() : mass;

		body->CreateFixture(fixture);
		break;
	default:
		break;
	}
}

void ECS::Rigidbody::fixedUpdate(float fixedDeltaTime) {
	transform->setPosition(body->GetPosition().x, body->GetPosition().y);

	std::cout << body->GetPosition().x << " " << body->GetPosition().y << std::endl;
}

void ECS::Rigidbody::setBodyType(BODY_TYPE type) {
	bodyType = type;
}

void ECS::Rigidbody::setLinearDrag(float drag) {
	linearDrag = drag;
}

void ECS::Rigidbody::setAngularDrag(float drag) {
	angularDrag = drag;
}

void ECS::Rigidbody::setMass(float mass) {
	this->mass = mass;
}

void ECS::Rigidbody::setGravityScale(float scale) {
	gravityScale = scale;
}

void ECS::Rigidbody::freezeXPosition(bool value) {
	xMovementLocked = value;
}

void ECS::Rigidbody::freezeYPosition(bool value) {
	yMovementLocked = value;
}

void ECS::Rigidbody::freezeZRotation(bool value) {
	rotationLocked = value;
}

b2BodyType ECS::Rigidbody::assingBodyType(BODY_TYPE type) {

	b2BodyType bodyType = b2_staticBody;

	if (type == ECS::Rigidbody::STATIC) {
		bodyType = b2_staticBody;
	}
	else if (type == ECS::Rigidbody::KINEMATIC) {
		bodyType = b2_kinematicBody;
	}
	else if (type == ECS::Rigidbody::DINAMIC) {
		bodyType = b2_dynamicBody;
	}

	return bodyType;
}