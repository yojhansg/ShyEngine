#include "Collider.h"
#include "Transform.h"
#include "Image.h"
#include "Entity.h"
#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include <cassert>
#include <iostream>

ECS::Collider::Collider() {

	screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();

	size = Utilities::Vector2D(1, 1);
	offSet = Utilities::Vector2D(0, 0);

	transform = nullptr; world = nullptr;
	body = nullptr; shape = nullptr;
	bodyDefinition = nullptr; 
	framePosition = nullptr;
	fixture = nullptr; scale = nullptr;
}

ECS::Collider::~Collider() {
	delete shape;
	delete bodyDefinition;

	delete framePosition;

	world->DestroyBody(body);
}

void ECS::Collider::init() {
	world = PhysicsManager::PhysicsManager::instance()->getWorld();

	bodyDefinition = new b2BodyDef(); shape = new b2PolygonShape(); framePosition = new b2Vec2();

	transform = this->getEntity()->getComponent<Transform>();
	assert(transform != nullptr, "La entidad debe contener un componente Transform");
	
	position = transform->getPosition();
	rotation = transform->getRotation();
	scale = transform->getScale();

	Image* image = this->getEntity()->getComponent<Image>();

	// Collider size = Image size if Image component exists in the entity
	if (image != nullptr)
		size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);

	shape->SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);

	bodyDefinition->type = b2_staticBody;
	bodyDefinition->position.Set(0, 0);
	bodyDefinition->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));

	body = world->CreateBody(bodyDefinition);

	fixture = body->CreateFixture(shape, 0.0f);
	fixture->SetRestitution(0.5f);

}

void ECS::Collider::start() {

	framePosition->Set(position->getX() / screenToWorldFactor + offSet.getX(), position->getY() / screenToWorldFactor + offSet.getY());

	body->SetTransform(*framePosition, (b2_pi / 180) * (*rotation));
}

void ECS::Collider::update(float deltaTime) {

	//// Position + collider offsets
	//framePosition->Set(position->getX() / screenToWorldFactor + offSet.getX(), position->getY() / screenToWorldFactor + offSet.getY());

	//// Position + rotation
	//body->SetTransform(*framePosition, (b2_pi / 180) * (*rotation));

	// Scale
	float scaledX = (size.getX() * scale->getX());
	float scaledY = (size.getY() * scale->getY());

	body->DestroyFixture(fixture);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(scaledX / 2.0f, scaledY / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;

	fixture = body->CreateFixture(&fixtureDef);
	fixture->SetRestitution(0.8f);
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

b2Fixture* ECS::Collider::getFixture() {
	return fixture;
}
