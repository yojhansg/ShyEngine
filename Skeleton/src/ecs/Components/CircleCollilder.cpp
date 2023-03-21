//#include "CircleCollilder.h"
//#include "PhysicsManager.h"
//#include "box2d/box2d.h"
//#include "Transform.h"
//#include "Entity.h"
//#include "Image.h"
//
//#include <iostream>
//
//namespace ECS {
//
//	CircleCollilder::CircleCollilder() {
//
//		size = Vector2D(1, 1);
//		offSet = Vector2D(0, 0);
//
//		radius = 0.5f;
//
//		shorterAxis = false;
//
//		shape = nullptr;
//		framePosition = nullptr;
//	}
//
//	CircleCollilder::~CircleCollilder() {
//		delete shape;
//
//		delete framePosition;
//
//		body->DestroyFixture(fixture);
//		world->DestroyBody(body);
//	}
//
//	void CircleCollilder::init() {
//
//		Collider::init();
//
//		shape = new b2CircleShape();
//		framePosition = new b2Vec2();
//
//		Image* image = this->getEntity()->getComponent<Image>();
//
//		// Collider radius = max between imagen width and height if exists
//		if (image != nullptr)
//			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);
//
//		radius = std::max(size.getX() * scale->getX(), size.getY() * scale->getY()) / 2;
//
//		shape->m_radius = radius;
//
//		bodyDefinition->type = b2_staticBody;
//		bodyDefinition->position.Set(0, 0);
//		bodyDefinition->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));
//
//		body = world->CreateBody(bodyDefinition);
//
//		fixture = body->CreateFixture(shape, 0.0f);
//
//		b2Filter f = fixture->GetFilterData();
//
//		f.categoryBits = 0x0001;
//		f.maskBits = 0x0001;
//
//		fixture->SetFilterData(f);
//
//	}
//
//	void CircleCollilder::start() {
//		framePosition->Set(position->getX() / screenToWorldFactor, position->getY() / screenToWorldFactor);
//
//		body->SetTransform(*framePosition, (b2_pi / 180) * (*rotation));
//
//		fixture->GetShape()->m_radius = calculateRadius();
//	}
//
//	void CircleCollilder::update(float deltaTime) {
//
//		// Radians
//		float radians = (b2_pi / 180) * (*rotation);
//
//		Vector2D v = Vector2D(offSet.getX() * std::cos(radians) - offSet.getY() * std::sin(radians), offSet.getX() * std::sin(radians) + offSet.getY() * std::cos(radians));
//
//		// Position + collider offsets
//		framePosition->Set(position->getX() / screenToWorldFactor + v.getX() * scale->getX(), position->getY() / screenToWorldFactor + v.getY() * scale->getY());
//
//		// Position + rotation
//		body->SetTransform(*framePosition, radians);
//
//		fixture->GetShape()->m_radius = calculateRadius();
//	}
//
//	float CircleCollilder::getVolume() {
//		return b2_pi * pow(radius, 2);
//	}
//
//	void CircleCollilder::setRadiusToShorterAxis(bool value) {
//		shorterAxis = value;
//	}
//
//	void CircleCollilder::setRadius(float radius) {
//
//		size.set(radius * 2, radius * 2);
//
//		this->radius = radius;
//	}
//
//	float CircleCollilder::getRadius() {
//		return radius;
//	}
//
//	Vector2D CircleCollilder::getOffSet() {
//		return offSet;
//	}
//
//	void CircleCollilder::addOffSet(float x, float y) {
//		offSet.set(offSet.getX() + x, offSet.getY() + y);
//	}
//
//	b2CircleShape* CircleCollilder::getShape() {
//		return shape;
//	}
//
//	float CircleCollilder::calculateRadius() {
//
//		float scaledX = size.getX() * scale->getX();
//		float scaledY = size.getY() * scale->getY();
//
//		if (shorterAxis)
//			return std::min(scaledX, scaledY) / 2;
//		
//		return std::max(scaledX, scaledY) / 2;
//	}
//
//}
