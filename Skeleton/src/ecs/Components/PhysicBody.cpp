#include "PhysicBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_world.h"
#include "Transform.h"
#include "Entity.h"

namespace ECS {

	PhysicsBody::PhysicsBody() {

		pm = Physics::PhysicsManager::instance();

		screenToWorldFactor = pm->getScreenToWorldFactor();

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

		layerName = "Default";
		
	}

	PhysicsBody::~PhysicsBody() {
		delete bodyDef;
		delete fixtureDef;
	}

	void PhysicsBody::init() {

		transform = this->getEntity()->getComponent<Transform>();
		assert(transform != nullptr, "La entidad debe contener un componente Transform");

		world = Physics::PhysicsManager::instance()->getWorld();

		position = transform->getPositionPointer();
		rotation = transform->getRotation();
		scale = transform->getScalePointer();

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

		// Collision Filtering
		b2Filter filter = fixture->GetFilterData();

		filter.categoryBits = pm->getLayerNumber(layerName);
		filter.maskBits = pm->

		fixture->SetFilterData(filter);
	}

	void PhysicsBody::fixedUpdate(float fixedDeltaTime) {

		if (bodyType != STATIC) {

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

	void PhysicsBody::onActive() {
		body->SetAwake(true);
	}

	void PhysicsBody::onDeacitve() {
		body->SetAwake(false);
	}

	void PhysicsBody::onSceneUp() {
		body->SetAwake(true);
	}

	void PhysicsBody::onSceneDown() {
		body->SetAwake(false);
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

	float PhysicsBody::getAngle() {
		return body->GetAngle() * (180 / b2_pi);
	}

	void PhysicsBody::setLinearVelocity(float x, float y) {
		body->SetLinearVelocity({ x, y });
	}

	Vector2D PhysicsBody::getLinearVelocity() {
		return { body->GetLinearVelocity().x, body->GetLinearVelocity().y };
	}

	void PhysicsBody::setAngularVelocity(float a) {
		body->SetAngularVelocity(a);
	}

	float PhysicsBody::getAngularVelocity() {
		return body->GetAngularVelocity();
	}

	void PhysicsBody::applyForce(const Vector2D& force, const Vector2D& point) {
		body->ApplyForce({ force.getX(), force.getY() }, { point.getX(), point.getY() }, true);
	}

	void PhysicsBody::applyForceToCenter(const Vector2D& force) {
		body->ApplyForceToCenter({ force.getX(), force.getY() }, true);
	}

	void PhysicsBody::applyTorque(float torque) {
		body->ApplyTorque(torque, true);
	}

	void PhysicsBody::applyLinearImpulse(const Vector2D& impulse, const Vector2D& point) {
		body->ApplyLinearImpulse({ impulse.getX(), impulse.getY() }, { point.getX(), point.getY() }, true);
	}

	void PhysicsBody::applyLinearImpulseToCenter(const Vector2D& impulse) {
		body->ApplyLinearImpulseToCenter({ impulse.getX(), impulse.getY() }, true);
	}

	void PhysicsBody::applyAngularImpulse(float impulse) {
		body->ApplyAngularImpulse(impulse, true);
	}

}
