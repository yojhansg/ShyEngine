#include "PhysicBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_world.h"
#include "Transform.h"

#include <ConsoleManager.h>
#include <Entity.h>

namespace ECS {

	PhysicBody::PhysicBody() {

		pm = Physics::PhysicsManager::instance();

		screenToWorldFactor = pm->getScreenToWorldFactor();

		world = nullptr;

		this->bodyType = (int) BODY_TYPE::STATIC;

		size = { 1, 1 };
		offSet = { 0, 0 };

		lastPositionSync = { 0, 0 };
		lastRotationSync = 0;
		lastScaleInfo = { 0, 0 };

		transform = nullptr;

		mass = 1;

		bounciness = .5f;

		body = nullptr;
		bodyDef = nullptr;
		fixture = nullptr;
		fixtureDef = nullptr;

		layerName = "Default";

		onCollisonStay = false;
		onTriggerStay = false;

		collisionEntity = nullptr;

		trigger = false;
		freezeRotation = false;
	}

	PhysicBody::~PhysicBody() {
		delete bodyDef;
		delete fixtureDef;
	}

	void PhysicBody::onDestroy() {
		//world->DestroyBody(body);
	}

	void PhysicBody::init() {

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		world = Physics::PhysicsManager::instance()->getWorld();

		bodyDef = new b2BodyDef();
		fixtureDef = new b2FixtureDef();
	}

	void PhysicBody::start() {

		auto position = transform->GetWorldPosition();
		auto rotation = -transform->GetWorldRotation();
		auto scale = transform->GetWorldScale();


		// Radians
		float radians = (b2_pi / 180) * (rotation);

		Vector2D v = Vector2D(offSet.getX() * std::cos(radians) - offSet.getY() * std::sin(radians), offSet.getX() * std::sin(radians) + offSet.getY() * std::cos(radians));

		b2Vec2 p = { position.getX() / screenToWorldFactor + v.getX() * scale.getX(), position.getY() / screenToWorldFactor + v.getY() * scale.getY() };

		// Position + rotation
		body->SetTransform(p, radians);

		scaleShape();

		lastPositionSync = position;
		lastRotationSync = rotation;
		lastScaleInfo = scale;

		// Collision Filtering
		b2Filter filter = fixture->GetFilterData();

		filter.categoryBits = pm->getLayerBits(layerName);
		filter.maskBits = pm->getMaskBits(layerName);

		fixture->SetFilterData(filter);

		setBodyType(bodyType);
		setBounciness(bounciness);
		setTrigger(trigger);
		setRotationFreezed(freezeRotation);
	}

	void PhysicBody::fixedUpdate(float fixedDeltaTime) {

		auto position = transform->GetWorldPosition();
		auto rotation = -transform->GetWorldRotation();
		auto scale = transform->GetWorldScale();

		if (onCollisonStay)
			getEntity()->onCollisionStay(collisionEntity);
		else if (onTriggerStay)
			getEntity()->onTriggerStay(collisionEntity);

		if (bodyType != (int) BODY_TYPE::STATIC) {

			// Position
			Vector2D trPosOffSet = position - lastPositionSync;

			Vector2D bodyPosOffSet = { body->GetPosition().x * screenToWorldFactor - lastPositionSync.getX(), body->GetPosition().y * screenToWorldFactor - lastPositionSync.getY() };

			Vector2D newPos = lastPositionSync + trPosOffSet + bodyPosOffSet;

			lastPositionSync = newPos;

			// Rotation
			float trRotOffSet = rotation - lastRotationSync;

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
			transform->SetWorldPosition(newPos);
			transform->SetWorldRotation(-newRotation);

		}

	}

	void PhysicBody::onActive() {
		pm->setBodyEnabled(body, true);
	}

	void PhysicBody::onDeacitve() {
		pm->setBodyEnabled(body, false);
	}

	void PhysicBody::onSceneUp() {
		pm->setBodyEnabled(body, true);
	}

	void PhysicBody::onSceneDown() {
		pm->setBodyEnabled(body, false);
	}

	void PhysicBody::setTrigger(bool trigger) {
		fixture->SetSensor(trigger);
	}

	bool PhysicBody::isTrigger() {
		return fixture->IsSensor();
	}

	void PhysicBody::setFriction(float friction) {
		fixture->SetFriction(friction);
	}

	float PhysicBody::getFriction() {
		return fixture->GetFriction();
	}

	void PhysicBody::setBounciness(float bounciness) {
		fixture->SetRestitution(bounciness);
	}

	float PhysicBody::getBounciness() {
		return fixture->GetRestitution();
	}

	void PhysicBody::addOffSet(float x, float y) {
		offSet.set(offSet.getX() + x, offSet.getY() + y);
	}

	Vector2D PhysicBody::getOffSet() {
		return offSet;
	}

	void PhysicBody::setRotationFreezed(bool freezed) {
		body->SetFixedRotation(freezed);
	}

	bool PhysicBody::isRotationFreezed() {
		return body->IsFixedRotation();
	}

	void PhysicBody::setBodyType(int type) {

		bodyType = type;

		b2BodyType t = b2_staticBody;

		switch ((BODY_TYPE)type) {
		case ECS::PhysicBody::BODY_TYPE::STATIC:
			t = b2_staticBody;
			break;
		case ECS::PhysicBody::BODY_TYPE::KINEMATIC:
			t = b2_kinematicBody;
			break;
		case ECS::PhysicBody::BODY_TYPE::DYNAMIC:
			t = b2_dynamicBody;
			break;
		default:
			break;
		}

		body->SetType(t);
	}

	int PhysicBody::getBodyType() {
		return bodyType;
	}

	void PhysicBody::setLinearDrag(float drag) {
		body->SetLinearDamping(drag);
	}

	float PhysicBody::getLinearDrag() {
		return body->GetLinearDamping();
	}

	void PhysicBody::setAngularDrag(float drag) {
		body->SetAngularDamping(drag);
	}

	float PhysicBody::getAngularDrag() {
		return body->GetAngularDamping();
	}

	void PhysicBody::setGravityScale(float scale) {
		body->SetGravityScale(scale);
	}

	float PhysicBody::getGravityScale() {
		return body->GetGravityScale();
	}

	float PhysicBody::getMass() {
		return mass;
	}

	float PhysicBody::getAngle() {
		return body->GetAngle() * (180 / b2_pi);
	}

	void PhysicBody::setCollisionLayer(cstring layerName) {

		if (!pm->layersExists(layerName)) {
			Console::Output::PrintError("Physics Body", "The layer with name " + layerName + " does not exist.");
			return;
		}

		this->layerName = layerName;
	}

	void PhysicBody::setCollisionStay(bool stay, Entity* b) {
		onCollisonStay = stay;

		this->collisionEntity = b;
	}

	void PhysicBody::setTriggerStay(bool stay, Entity* b) {
		onTriggerStay = stay;

		this->collisionEntity = b;
	}

	void PhysicBody::setLinearVelocity(float x, float y) {
		body->SetLinearVelocity({ x, y });
	}

	Vector2D PhysicBody::getLinearVelocity() {
		return { body->GetLinearVelocity().x, body->GetLinearVelocity().y };
	}

	void PhysicBody::setAngularVelocity(float a) {
		body->SetAngularVelocity(a);
	}

	float PhysicBody::getAngularVelocity() {
		return body->GetAngularVelocity();
	}

	void PhysicBody::applyForce(cVector2D force, cVector2D point) {
		body->ApplyForce({ force.getX(), force.getY() }, { point.getX(), point.getY() }, true);
	}

	void PhysicBody::applyForceToCenter(cVector2D force) {
		body->ApplyForceToCenter({ force.getX(), force.getY() }, true);
	}

	void PhysicBody::applyTorque(float torque) {
		body->ApplyTorque(torque, true);
	}

	void PhysicBody::applyLinearImpulse(cVector2D impulse, cVector2D point) {
		body->ApplyLinearImpulse({ impulse.getX(), impulse.getY() }, { point.getX(), point.getY() }, true);
	}

	void PhysicBody::applyLinearImpulseToCenter(cVector2D impulse) {
		body->ApplyLinearImpulseToCenter({ impulse.getX(), impulse.getY() }, true);
	}

	void PhysicBody::applyAngularImpulse(float impulse) {
		body->ApplyAngularImpulse(impulse, true);
	}

}
