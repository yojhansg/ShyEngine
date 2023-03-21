//#include "BoxCollider.h"
//#include "PhysicsManager.h"
//#include "box2d/box2d.h"
//#include "Transform.h"
//#include "Entity.h"
//#include "Image.h"
//
//namespace ECS {
//
//	BoxCollider::BoxCollider() {
//
//		size = Vector2D(1, 1);
//		offSet = Vector2D(0, 0);
//
//		shape = nullptr; 
//		framePosition = nullptr;
//		frameFixtureDef = nullptr;
//	}
//
//	BoxCollider::~BoxCollider() {
//		delete shape;
//
//		delete framePosition;
//		delete frameFixtureDef;
//
//		body->DestroyFixture(fixture);
//		world->DestroyBody(body);
//	}
//
//	void BoxCollider::init() {
//
//		Collider::init();
//
//		shape = new b2PolygonShape();
//		framePosition = new b2Vec2(); 
//		frameFixtureDef = new b2FixtureDef();
//
//		Image* image = this->getEntity()->getComponent<Image>();
//
//		// Collider size = Image size if Image component exists in the entity
//		if (image != nullptr)
//			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);
//
//		shape->SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);
//
//		bodyDefinition->type = b2_staticBody;
//		bodyDefinition->position.Set(0, 0);
//		bodyDefinition->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));
//
//		body = world->CreateBody(bodyDefinition);
//
//
//	}
//
//	void BoxCollider::start() {
//		framePosition->Set(position->getX() / screenToWorldFactor + offSet.getX(), position->getY() / screenToWorldFactor + offSet.getY());
//
//		body->SetTransform(*framePosition, (b2_pi / 180) * (*rotation));
//	}
//
//	void BoxCollider::update(float deltaTime) {
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
//		// Scale
//		float scaledX = (size.getX() / 2.0f) * scale->getX();
//		float scaledY = (size.getY() / 2.0f) * scale->getY();
//
//		body->DestroyFixture(fixture);
//
//		shape->SetAsBox(scaledX, scaledY);
//
//		frameFixtureDef->shape = shape;
//		frameFixtureDef->density = 1;
//
//		fixture = body->CreateFixture(frameFixtureDef);
//
//		// Collision filtering
//		b2Filter f = fixture->GetFilterData();
//
//		f.categoryBits = 0x0001;
//		f.maskBits = 0x0001;
//
//		fixture->SetFilterData(f);
//
//		fixture->SetSensor(trigger);
//		fixture->SetRestitution(bounciness);
//		fixture->SetFriction(friction);
//	}
//
//	float BoxCollider::getVolume() {
//		return size.volume();
//	}
//
//	Vector2D BoxCollider::getSize() {
//		return size;
//	}
//
//	void BoxCollider::setSize(float x, float y) {
//		this->size.set(x, y);
//	}
//
//	Vector2D BoxCollider::getOffSet() {
//		return offSet;
//	}
//
//	void BoxCollider::addOffSet(float x, float y) {
//		offSet.set(offSet.getX() + x, offSet.getY() + y);
//	}
//
//	b2PolygonShape* BoxCollider::getShape() {
//		return shape;
//	}
//
//}
