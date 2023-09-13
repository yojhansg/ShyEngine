#include "CircleBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_world.h"
#include "Entity.h"
#include "Image.h"
#include "Transform.h"

namespace ECS {

	CircleBody::CircleBody() {

		shape = nullptr;

		radius = 0.0f;

		size = { 0, 0 };

		shorterAxis = false;

	}

	void CircleBody::init() {

		PhysicBody::init();

		shape = new b2CircleShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr) {

			Utilities::Vector2D imageSize = { image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor };

			if (radius == 0)
				size.set(imageSize);
		}
		else {
			auto scale = transform->GetWorldScale();
			radius = std::max(radius * scale.getX(), radius * scale.getY()) / 2;
		}

		shape->m_radius = radius;

		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;

		shape = static_cast<b2CircleShape*>(fixture->GetShape());

	}

	float CircleBody::getArea() {
		return b2_pi * pow(radius, 2);
	}

	void CircleBody::setRadiusToShorterAxis(bool value) {
		shorterAxis = value;
	}

	void CircleBody::setRadius(float radius) {

		size.set(radius * 2, radius * 2);

		this->radius = radius;
	}

	float CircleBody::getRadius() {
		return radius;
	}

	void CircleBody::setMass(float mass) {
		this->mass = mass;

		fixture->SetDensity(mass / getArea());
	}

	void CircleBody::scaleShape() {
		fixture->GetShape()->m_radius = calculateRadius();
	}

	float CircleBody::calculateRadius() {
		auto scale = transform->GetWorldScale();

		float scaledX = 0;
		float scaledY = 0;

		if (radius != 0) {
			scaledX = radius * scale.getX();
			scaledY = radius * scale.getY();
		}
		else {
			scaledX = size.getX() * scale.getX();
			scaledY = size.getY() * scale.getY();
		}

		if (shorterAxis)
			return std::min(scaledX, scaledY) / 2;
		
		return std::max(scaledX, scaledY) / 2;
	}

}
