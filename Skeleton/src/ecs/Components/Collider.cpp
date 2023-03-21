#include "Collider.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "Transform.h"
#include "box2d/b2_world.h"
#include "box2d/b2_polygon_shape.h"
#include "Image.h"

namespace ECS {

	Collider::Collider() {

		screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();

		world = nullptr;

		this->bodyType = STATIC;

		size = Vector2D(1, 1);
		offSet = Vector2D(0, 0);

		lastSync = { 0, 0 };

		transform = nullptr;

		trigger = false;
		friction = 0;
		bounciness = 0;
		mass = 1;

		body = nullptr;
		bodyDef = nullptr;
		fixture = nullptr;
		fixtureDef = nullptr;

		position = nullptr;
		rotation = nullptr;
		scale = nullptr;
		
		shape = nullptr; 
		framePosition = nullptr;
	}

	Collider::~Collider() {
		delete bodyDef;
		delete fixtureDef;
		delete shape;
		delete framePosition;
	
		body->DestroyFixture(fixture);
		world->DestroyBody(body);
	}

	void Collider::init() {
		transform = this->getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		world = PhysicsManager::PhysicsManager::instance()->getWorld();

		position = transform->getPosition();
		rotation = transform->getRotation();
		scale = transform->getScale();

		bodyDef = new b2BodyDef();
		shape = new b2PolygonShape();
		framePosition = new b2Vec2(); 
		fixtureDef = new b2FixtureDef();
		
		Image* image = this->getEntity()->getComponent<Image>();
		
		// Collider size = Image size if Image component exists in the entity
		if (image != nullptr)
			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);
		
		shape->SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);
		
		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));
		
		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);

		fixture = body->CreateFixture(fixtureDef);

	}

	void Collider::start() {

		// Radians
		float radians = (b2_pi / 180) * (*rotation);

		Vector2D v = Vector2D(offSet.getX() * std::cos(radians) - offSet.getY() * std::sin(radians), offSet.getX() * std::sin(radians) + offSet.getY() * std::cos(radians));

		// Position + collider offsets
		framePosition->Set(position->getX() / screenToWorldFactor + v.getX() * scale->getX(), position->getY() / screenToWorldFactor + v.getY() * scale->getY());

		// Position + rotation
		body->SetTransform(*framePosition, radians);

		// Scale
		float scaledX = (size.getX() / 2.0f) * scale->getX();
		float scaledY = (size.getY() / 2.0f) * scale->getY();

		body->DestroyFixture(fixture);

		shape->SetAsBox(scaledX, scaledY);

		fixtureDef->shape = shape;
		fixtureDef->density = 1;

		fixture = body->CreateFixture(fixtureDef);

		fixture->SetSensor(trigger);
		fixture->SetRestitution(bounciness);
		fixture->SetFriction(friction);
		fixture->SetDensity(body->GetMass() / getVolume());

		lastSync = position;
	}

	void Collider::update(float deltaTime) {
		

	}

	void Collider::fixedUpdate(float fixedDeltaTime) {


		Vector2D trOffSet = *position - lastSync;

		Vector2D bodyOffSet = { body->GetPosition().x * screenToWorldFactor - lastSync.getX(), body->GetPosition().y * screenToWorldFactor - lastSync.getY() };

		Vector2D newPos = lastSync + trOffSet + bodyOffSet;

		lastSync = newPos;

		body->SetTransform(b2Vec2(newPos.getX() / screenToWorldFactor, newPos.getY() / screenToWorldFactor), 0);
		transform->setPosition(newPos);


		//// Radians
		//float radians = (b2_pi / 180) * (*rotation);

		//Vector2D v = Vector2D(offSet.getX() * std::cos(radians) - offSet.getY() * std::sin(radians), offSet.getX() * std::sin(radians) + offSet.getY() * std::cos(radians));

		//// Position + collider offsets
		//framePosition->Set(position->getX() / screenToWorldFactor + v.getX() * scale->getX(), position->getY() / screenToWorldFactor + v.getY() * scale->getY());

		//// Position + rotation
		//body->SetTransform(*framePosition, radians);

		//// Scale
		//float scaledX = (size.getX() / 2.0f) * scale->getX();
		//float scaledY = (size.getY() / 2.0f) * scale->getY();

		//body->DestroyFixture(fixture);

		//shape->SetAsBox(scaledX, scaledY);

		//fixtureDef->shape = shape;
		//fixtureDef->density = 1;

		//fixture = body->CreateFixture(fixtureDef);

		//fixture->SetSensor(trigger);
		//fixture->SetRestitution(bounciness);
		//fixture->SetFriction(friction);

		//transform->setPosition(body->GetPosition().x * screenToWorldFactor, body->GetPosition().y * screenToWorldFactor);
		//transform->setRotation(body->GetAngle() * (180 / b2_pi));
	}

	float Collider::getVolume() {
		return size.volume();
	}

	void Collider::setTrigger(bool trigger) {
		this->trigger = trigger;
	}

	bool Collider::isTrigger() {
		return trigger;
	}

	void Collider::setFriction(float friction) {
		this->friction = friction;
	}

	float Collider::getFriction() {
		return friction;
	}

	void Collider::setBounciness(float bounciness) {
		this->bounciness = bounciness;
	}

	float Collider::getBounciness() {
		return bounciness;
	}

	b2Body* Collider::getBody() {
		return body;
	}

	b2Fixture* Collider::getFixture() {
		return fixture;
	}

	Vector2D Collider::getSize() {
		return size;
	}

	void Collider::setSize(float x, float y) {
		this->size.set(x, y);
	}

	Vector2D Collider::getOffSet() {
		return offSet;
	}

	void Collider::addOffSet(float x, float y) {
		offSet.set(offSet.getX() + x, offSet.getY() + y);
	}

	b2PolygonShape* Collider::getShape() {
		return shape;
	}

	void Collider::setRotationFreezed(bool freezed) {
		body->SetFixedRotation(freezed);
	}

	bool Collider::isRotationFreezed() {
		return body->IsFixedRotation();
	}

	void Collider::setBodyType(BODY_TYPE type) {

		bodyType = type;

		b2BodyType t = b2_staticBody;

		switch (type) {
			case ECS::Collider::STATIC:
				t = b2_staticBody;
				break;
			case ECS::Collider::KINEMATIC:
				t = b2_kinematicBody;
				break;
			case ECS::Collider::DYNAMIC:
				t = b2_dynamicBody;
				break;
			default:
				break;
		}

		body->SetType(t);
	}

	Collider::BODY_TYPE Collider::getBodyType() {
		return bodyType;
	}

	void Collider::setLinearDrag(float drag) {
		body->SetLinearDamping(drag);
	}

	float Collider::getLinearDrag() {
		return body->GetLinearDamping();
	}

	void Collider::setAngularDrag(float drag) {
		body->SetAngularDamping(drag);
	}

	float Collider::getAngularDrag() {
		return body->GetAngularDamping();
	}

	void Collider::setMass(float mass) {
		this->mass = mass;
	}

	float Collider::getMass() {
		return mass;
	}

	void Collider::setGravityScale(float scale) {
		body->SetGravityScale(scale);
	}

	float Collider::getGravityScale() {
		return body->GetGravityScale();
	}

}


//#include "Collider.h"
//#include "PhysicsManager.h"
//#include "Entity.h"
//#include "Transform.h"
//#include "box2d/b2_world.h"
//
//ECS::Collider::Collider() {
//
//	screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();
//
//	world = nullptr;
//
//	transform = nullptr;
//
//	trigger = false;
//	friction = 0;
//	bounciness = 0;
//
//	body = nullptr; 
//	bodyDefinition = nullptr; 
//	fixture = nullptr; 
//
//	position = nullptr;
//	rotation = nullptr;
//	scale = nullptr;
//}
//
//ECS::Collider::~Collider() {
//	delete bodyDefinition;
//}
//
//void ECS::Collider::init() {
//	transform = this->getEntity()->getComponent<Transform>();
//	assert(transform != nullptr, "La entidad debe contener un componente Transform");
//
//	world = PhysicsManager::PhysicsManager::instance()->getWorld();
//
//	bodyDefinition = new b2BodyDef();
//
//	position = transform->getPosition();
//	rotation = transform->getRotation();
//	scale = transform->getScale();
//}
//
//void ECS::Collider::setTrigger(bool trigger) {
//	this->trigger = trigger;
//}
//
//bool ECS::Collider::isTrigger() {
//	return trigger;
//}
//
//void ECS::Collider::setFriction(float friction) {
//	this->friction = friction;
//}
//
//float ECS::Collider::getFriction() {
//	return friction;
//}
//
//void ECS::Collider::setBounciness(float bounciness) {
//	this->bounciness = bounciness;
//}
//
//float ECS::Collider::getBounciness() {
//	return bounciness;
//}
//
//b2Body* ECS::Collider::getBody() {
//	return body;
//}
//
//b2Fixture* ECS::Collider::getFixture() {
//	return fixture;
//}
