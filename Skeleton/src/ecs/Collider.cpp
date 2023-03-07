#include "Collider.h"
#include "Transform.h"
#include "Image.h"
#include "Entity.h"
#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include <cassert>
#include <iostream>

ECS::Collider::Collider() {

	size = Utilities::Vector2D(1, 1);
	offSet = Utilities::Vector2D(0, 0);

	transform = nullptr; world = nullptr;
	body = nullptr; shape = nullptr;
	bodyDefinition = nullptr; 
	temporalPosition = nullptr;
	fixture = nullptr; scale = nullptr;
}

ECS::Collider::~Collider() {
	delete shape;
	delete bodyDefinition;

	delete temporalPosition;

	world->DestroyBody(body);
}

void ECS::Collider::init() {
	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef(); shape = new b2PolygonShape(); temporalPosition = new b2Vec2;

	transform = this->getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");
	
	position = transform->getPosition();
	rotation = transform->getRotation();
	scale = transform->getScale();

	Image* image = this->getEntity()->getComponent<Image>();

	// Collider size = Image size if Image component exists in the entity
	if (image != nullptr)
		size.set(image->getTextureWidth(), image->getTextureHeight());

	shape->SetAsBox(size.getX() / 2, size.getY() / 2);

	bodyDefinition->type = b2_staticBody;
	bodyDefinition->position.Set(position->getX() + offSet.getX(), position->getY()+ offSet.getY());
	bodyDefinition->angle = (M_PI / 180) * (*rotation);

	body = world->CreateBody(bodyDefinition);

	fixture = body->CreateFixture(shape, 0.0f);

}

void ECS::Collider::update(float deltaTime) {

	// Position + half size to center the collider + collider offsets
	temporalPosition->Set(position->getX() + offSet.getX(), position->getY() + offSet.getY());

	// Position + rotation
	body->SetTransform(*temporalPosition, (b2_pi / 180) * (*rotation));

	// Scale
	float scaledX = (size.getX() * transform->getScale()->getX());
	float scaledY = (size.getY() * transform->getScale()->getY());

	body->DestroyFixture(fixture);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(scaledX / 2, scaledY / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;

	fixture = body->CreateFixture(&fixtureDef);
}

void ECS::Collider::fixedUpdate(float fixedDeltaTime) {

}

void ECS::Collider::setTrigger(bool trigger) {
	fixture->SetSensor(trigger);
}

bool ECS::Collider::isTrigger() {
	return fixture->IsSensor();
}

void ECS::Collider::setFriction(float friction) {
	fixture->SetFriction(friction);
}

float ECS::Collider::getFriction() {
	return fixture->GetFriction();
}

void ECS::Collider::setBounciness(float bounciness) {
	fixture->SetRestitution(bounciness);
}

float ECS::Collider::getBounciness() {
	return fixture->GetRestitution();
}

Utilities::Vector2D ECS::Collider::getSize() {
	return size;
}

void ECS::Collider::setSize(float x, float y) {
	this->size.set(x, y);
}

Utilities::Vector2D ECS::Collider::getOffSet() {
	return offSet;
}

void ECS::Collider::addOffSet(float x, float y) {
	offSet.set(offSet.getX() + x, offSet.getY() + y);
}

b2Body* ECS::Collider::getBody() {
	return body;
}

b2PolygonShape* ECS::Collider::getShape() {
	return shape;
}
