#include "PhysicBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_world.h"
#include "box2d/b2_contact.h"
#include "Transform.h"
#include "BoxBody.h"
#include "CircleBody.h"
#include "EdgeBody.h"

#include <ConsoleManager.h>
#include <Entity.h>

namespace ECS {

	PhysicBody::PhysicBody() {

		pm = Physics::PhysicsManager::instance();

		screenToWorldFactor = pm->getScreenToWorldFactor();

		world = nullptr;

		this->bodyType = (int) BODY_TYPE::STATIC;

		offSet = { 0, 0 };

		lastPositionSync = { 0, 0 };
		lastRotationSync = 0;
		lastScaleInfo = { 0, 0 };

		transform = nullptr;

		mass = 1;

		bounciness = .5f;
		gravityScale = 1.0f;
		friction = 1.0f;

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
		world->DestroyBody(body);
	}

	void PhysicBody::init() {

		transform = this->getEntity()->getComponent<Transform>();

		if (transform == nullptr) {
			printError("Missing transform", "The entity must contain a transform component.");
			this->remove();
			return;
		}

		world = pm->getWorld();

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

		Vector2D p = { position.getX() + v.getX(), position.getY() + v.getY() };

		p = p / screenToWorldFactor;

		// Position + rotation
		body->SetTransform({p.getX(), p.getY()}, radians);

		scaleShape();

		lastPositionSync = position;
		lastRotationSync = rotation;
		lastScaleInfo = scale;

		// Collision Filtering
		b2Filter filter = fixture->GetFilterData();

		filter.categoryBits = pm->getLayerBits(layerName);
		filter.maskBits = pm->getMaskBits(layerName);

		fixture->SetFilterData(filter);

		// Fixture and body attributes
		setBodyType(bodyType);
		setBounciness(bounciness);
		setTrigger(trigger);
		setRotationFreezed(freezeRotation);
		setGravityScale(gravityScale);
		setFriction(friction);
		setLinearDrag(linearDamping);
		setAngularDrag(angularDamping);
	}

	void PhysicBody::fixedUpdate(float fixedDeltaTime) {

		auto position = transform->GetWorldPosition();
		auto rotation = -transform->GetWorldRotation();
		auto scale = transform->GetWorldScale();

		if (onCollisonStay)
			getEntity()->onCollisionStay(collisionEntity);
		if (onTriggerStay)
			getEntity()->onTriggerStay(collisionEntity);

		// Position
		Vector2D trPosOffSet = position - lastPositionSync;

		Vector2D bodyPosOffSet = { body->GetPosition().x * screenToWorldFactor - offSet.getX() - lastPositionSync.getX(),
									body->GetPosition().y * screenToWorldFactor - offSet.getY() - lastPositionSync.getY()};

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
		body->SetTransform(b2Vec2(newPos.getX() / screenToWorldFactor + offSet.getX() / screenToWorldFactor, 
			newPos.getY() / screenToWorldFactor + offSet.getY() / screenToWorldFactor), newRotation * (b2_pi / 180));

		// Transform
		transform->SetWorldPosition(newPos);
		transform->SetWorldRotation(-newRotation);


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

	PhysicBody* PhysicBody::GetComponentFromEntity(Entity* ent) {

		PhysicBody* other = ent->getComponent<BoxBody>();

		if (other == nullptr)
			other = ent->getComponent<CircleBody>();

		if (other == nullptr)
			other = ent->getComponent<EdgeBody>();

		return other;

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

	Vector2D PhysicBody::getCollisionNormal() {
		return collisionNormal;
	}

	Vector2D PhysicBody::getCollisionPoint() {
		return collisionPoint;
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

	cstring PhysicBody::getCollisionLayer() {
		return this->layerName;
	}

	void PhysicBody::setCollisionStay(bool stay, Entity* b) {
		onCollisonStay = stay;

		this->collisionEntity = b;
	}

	void PhysicBody::setTriggerStay(bool stay, Entity* b) {
		onTriggerStay = stay;

		this->collisionEntity = b;
	}

	void PhysicBody::setBox2DContact(b2Contact* c) {
		this->contact = c;

		// Local Normal
		collisionNormal.set(this->contact->GetManifold()->localNormal.x, this->contact->GetManifold()->localNormal.y);

		// Local Point
		collisionPoint.set(this->contact->GetManifold()->localPoint.x, this->contact->GetManifold()->localPoint.y);
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

	bool PhysicBody::collidesWith(PhysicBody* b) {
		return pm->layersCollide(layerName, b->getCollisionLayer());
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
