#include "CircleCollilder.h"
#include "PhysicsManager.h"
#include "box2d/box2d.h"
#include "Transform.h"
#include "Entity.h"
#include "Image.h"
#include <iostream>

namespace ECS {

	CircleCollilder::CircleCollilder() {

		size = Vector2D(1, 1);
		radius = 0.5f;

		shorterAxis = false;
		customRadius = false;

		shape = nullptr;
		framePosition = nullptr;
		frameFixtureDef = nullptr;
	}

	CircleCollilder::~CircleCollilder() {
		delete shape;

		delete framePosition;
		delete frameFixtureDef;

		body->DestroyFixture(fixture);
		world->DestroyBody(body);
	}

	void CircleCollilder::init() {

		Collider::init();

		shape = new b2CircleShape();
		framePosition = new b2Vec2();
		frameFixtureDef = new b2FixtureDef();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr)
			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);

		radius = std::max(size.getX() * scale->getX(), size.getY() * scale->getY()) / 2;

		shape->m_radius = radius;

		bodyDefinition->type = b2_staticBody;
		bodyDefinition->position.Set(0, 0);
		bodyDefinition->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));

		body = world->CreateBody(bodyDefinition);

		fixture = body->CreateFixture(shape, 0.0f);
	}

	void CircleCollilder::start() {
		framePosition->Set(position->getX() / screenToWorldFactor, position->getY() / screenToWorldFactor);

		body->SetTransform(*framePosition, (b2_pi / 180) * (*rotation));
	}

	void CircleCollilder::update(float deltaTime) {

		// Radians
		float radians = (b2_pi / 180) * (*rotation);

		// Position + collider offsets
		framePosition->Set(position->getX() / screenToWorldFactor, position->getY() / screenToWorldFactor);

		// Position + rotation
		body->SetTransform(*framePosition, radians);

		// Reset shape
		body->DestroyFixture(fixture);

		if (customRadius)
			shape->m_radius = radius;
		else
			shape->m_radius = calculateRadius();

		frameFixtureDef->shape = shape;
		frameFixtureDef->density = 1;

		fixture = body->CreateFixture(frameFixtureDef);

		fixture->SetSensor(trigger);
		fixture->SetRestitution(bounciness);
		fixture->SetFriction(friction);

	}

	void CircleCollilder::setRadiusToShorterAxis(bool value) {
		shorterAxis = value;
	}

	void CircleCollilder::setRadius(float radius) {

		customRadius = true;

		this->radius = radius;
	}

	float CircleCollilder::getRadius() {
		return radius;
	}

	b2CircleShape* CircleCollilder::getShape() {
		return shape;
	}

	float CircleCollilder::calculateRadius() {
		float r = 0;

		if (shorterAxis)
			r = std::min(size.getX() * scale->getX(), size.getY() * scale->getY()) / 2;
		else 
			r = std::max(size.getX() * scale->getX(), size.getY() * scale->getY()) / 2;

		return r;
	}

}
