#include "Rigidbody.h"
#include "box2d/box2d.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysicsManager.h"
#include "Vector2D.h"

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

	body = nullptr;
	fixture = nullptr;
	bodyDefinition = nullptr;
}

ECS::Rigidbody::~Rigidbody() {
	delete fixture;
	delete bodyDefinition;
}

void ECS::Rigidbody::init() {

	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef();
	fixture = new b2FixtureDef();
}

void ECS::Rigidbody::start() {

	transform = this->getEntity()->getComponent<Transform>();
	collider = this->getEntity()->getComponent<Collider>();

	if (collider != nullptr) {
		body = collider->getBody();
		body->SetType(assingBodyType(bodyType));
		body->SetLinearDamping(linearDrag);
		body->SetAngularDamping(angularDrag);

		switch (body->GetType()) {
			case b2_staticBody:
				body->CreateFixture(collider->getShape(), 0.0f);
				break;
			case b2_kinematicBody:
				fixture->shape = collider->getShape();

				body->CreateFixture(fixture);
				break;
			case b2_dynamicBody:
				fixture->shape = collider->getShape();
				fixture->friction = collider->getFriction();
				fixture->restitution = collider->getBounciness();
				fixture->isSensor = collider->isTrigger();
				fixture->density = mass / (collider->getSize().getX() * collider->getSize().getY());

				body->CreateFixture(fixture);
				break;
			default:
				break;
		}
	}
	else {
		Utilities::Vector2D size = transform->getScale();


	}

}

void ECS::Rigidbody::fixedUpdate(float fixedDeltaTime) {

	Utilities::Vector2D size = Utilities::Vector2D();

	if (collider != nullptr)
		size = collider->getSize();

	transform->setPosition(body->GetPosition().x - size.getX() / 2, body->GetPosition().y - size.getY() / 2);

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