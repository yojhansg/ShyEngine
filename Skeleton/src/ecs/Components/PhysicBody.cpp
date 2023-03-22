#include "PhysicBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_world.h"
#include "Transform.h"
#include "Entity.h"

namespace ECS {

	PhysicsBody::PhysicsBody() {

		screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();

		world = nullptr;

		this->bodyType = STATIC;

		size = { 1, 1 };
		offSet = { 0, 0 };

		lastPositionSync = { 0, 0 };
		lastRotationSync = 0;
		lastScaleInfo = { 0, 0 };

		transform = nullptr;

		mass = 1;

		body = nullptr;
		bodyDef = nullptr;
		fixture = nullptr;
		fixtureDef = nullptr;

		position = nullptr;
		rotation = nullptr;
		scale = nullptr;
		
	}

	PhysicsBody::~PhysicsBody() {
		delete bodyDef;
		delete fixtureDef;
	}

	void PhysicsBody::init() {

		transform = this->getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		world = PhysicsManager::PhysicsManager::instance()->getWorld();

		position = transform->getPosition();
		rotation = transform->getRotation();
		scale = transform->getScale();

		bodyDef = new b2BodyDef();
		fixtureDef = new b2FixtureDef();

	}

	void PhysicsBody::start() {

		// Radians
		float radians = (b2_pi / 180) * (*rotation);

		Vector2D v = Vector2D(offSet.getX() * std::cos(radians) - offSet.getY() * std::sin(radians), offSet.getX() * std::sin(radians) + offSet.getY() * std::cos(radians));

		b2Vec2 p = { position->getX() / screenToWorldFactor + v.getX() * scale->getX(), position->getY() / screenToWorldFactor + v.getY() * scale->getY() };

		// Position + rotation
		body->SetTransform(p, radians);

		scaleShape();

		lastPositionSync = position;
		lastRotationSync = *rotation;
		lastScaleInfo = scale;
	}

	void PhysicsBody::fixedUpdate(float fixedDeltaTime) {

		if (bodyType == DYNAMIC) {

			// Position
			Vector2D trPosOffSet = *position - lastPositionSync;

			Vector2D bodyPosOffSet = { body->GetPosition().x * screenToWorldFactor - lastPositionSync.getX(), body->GetPosition().y * screenToWorldFactor - lastPositionSync.getY() };

			Vector2D newPos = lastPositionSync + trPosOffSet + bodyPosOffSet;

			lastPositionSync = newPos;

			// Rotation
			float trRotOffSet = *rotation - lastRotationSync;

			float bodyRotOffSet = body->GetAngle() * (180 / b2_pi) - lastRotationSync;

			float newRotation = lastRotationSync + trRotOffSet + bodyRotOffSet;

			lastRotationSync = newRotation;

			// Scale
			if (lastScaleInfo != scale) {
				scaleShape();
				lastScaleInfo = scale;
			}

			// Body
			body->SetTransform(b2Vec2(newPos.getX() / screenToWorldFactor, newPos.getY() / screenToWorldFactor), newRotation * (b2_pi / 180));

			// Transform
			transform->setPosition(newPos);
			transform->setRotation(newRotation);

		}

	}

	void PhysicsBody::setTrigger(bool trigger) {
		fixture->SetSensor(trigger);
	}

	bool PhysicsBody::isTrigger() {
		return fixture->IsSensor();
	}

	void PhysicsBody::setFriction(float friction) {
		fixture->SetFriction(friction);
	}

	float PhysicsBody::getFriction() {
		return fixture->GetFriction();
	}

	void PhysicsBody::setBounciness(float bounciness) {
		fixture->SetRestitution(bounciness);
	}

	float PhysicsBody::getBounciness() {
		return fixture->GetRestitution();
	}

	void PhysicsBody::addOffSet(float x, float y) {
		offSet.set(offSet.getX() + x, offSet.getY() + y);
	}

	Vector2D PhysicsBody::getOffSet() {
		return offSet;
	}

	void PhysicsBody::setRotationFreezed(bool freezed) {
		body->SetFixedRotation(freezed);
	}

	bool PhysicsBody::isRotationFreezed() {
		return body->IsFixedRotation();
	}

	void PhysicsBody::setBodyType(BODY_TYPE type) {

		bodyType = type;

		b2BodyType t = b2_staticBody;

		switch (type) {
			case ECS::PhysicsBody::STATIC:
				t = b2_staticBody;
				break;
			case ECS::PhysicsBody::KINEMATIC:
				t = b2_kinematicBody;
				break;
			case ECS::PhysicsBody::DYNAMIC:
				t = b2_dynamicBody;
				break;
			default:
				break;
		}

		body->SetType(t);
	}

	PhysicsBody::BODY_TYPE PhysicsBody::getBodyType() {
		return bodyType;
	}

	void PhysicsBody::setLinearDrag(float drag) {
		body->SetLinearDamping(drag);
	}

	float PhysicsBody::getLinearDrag() {
		return body->GetLinearDamping();
	}

	void PhysicsBody::setAngularDrag(float drag) {
		body->SetAngularDamping(drag);
	}

	float PhysicsBody::getAngularDrag() {
		return body->GetAngularDamping();
	}

	void PhysicsBody::setGravityScale(float scale) {
		body->SetGravityScale(scale);
	}

	float PhysicsBody::getGravityScale() {
		return body->GetGravityScale();
	}

	float PhysicsBody::getMass() {
		return mass;
	}

}
