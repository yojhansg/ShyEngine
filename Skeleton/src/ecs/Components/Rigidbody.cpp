//#include "Rigidbody.h"
//#include "box2d/box2d.h"
//#include "Entity.h"
//#include "Transform.h"
//#include "BoxCollider.h"
//#include "CircleCollilder.h"
//#include "PhysicsManager.h"
//#include <iostream>
//
//namespace ECS {
//
//	Rigidbody::Rigidbody() {
//
//		screenToWorldFactor = PhysicsManager::PhysicsManager::instance()->getScreenToWorldFactor();
//
//		world = nullptr;
//
//		this->bodyType = STATIC;
//
//		transform = nullptr;
//
//		bodyDef = nullptr;
//		fixtureDef = nullptr;
//		body = nullptr; 
//		shape = nullptr;
//		fixture = nullptr;
//
//	}
//
//	Rigidbody::~Rigidbody() {
//		delete bodyDef;
//		delete fixtureDef;
//		delete shape;
//
//		body->DestroyFixture(fixture);
//		world->DestroyBody(body);
//	}
//
//	void Rigidbody::init() {
//
//		world = PhysicsManager::PhysicsManager::instance()->getWorld();
//
//		transform = this->getEntity()->getComponent<Transform>();
//		assert(transform != nullptr, "La entidad debe contener un componente Transform");
//
//		bodyDef = new b2BodyDef();
//		shape = new b2PolygonShape();
//		fixtureDef = new b2FixtureDef();
//
//		shape->SetAsBox(1.0f, 1.0f);
//
//		bodyDef->type = b2_staticBody;
//		bodyDef->position.Set(0, 0);
//
//		fixtureDef->shape = shape;
//		fixtureDef->density = 1.0f;
//		fixtureDef->isSensor = true;
//
//		body = world->CreateBody(bodyDef);
//
//		fixture = body->CreateFixture(fixtureDef);
//
//		// Collision filtering
//		b2Filter f = fixture->GetFilterData();
//
//		f.categoryBits = 0x0002;
//		f.maskBits = 0x0000;
//
//		fixture->SetFilterData(f);
//
//	}
//
//	void Rigidbody::start() {
//
//		b2Vec2 v = b2Vec2(transform->getPosition()->getX() / screenToWorldFactor, transform->getPosition()->getY() / screenToWorldFactor);
//
//		body->SetTransform(v, (b2_pi / 180) * (*transform->getRotation()));
//	}
//
//	void Rigidbody::fixedUpdate(float fixedDeltaTime) {
//
//		transform->setPosition(body->GetPosition().x * screenToWorldFactor, body->GetPosition().y * screenToWorldFactor);
//		transform->setRotation(body->GetAngle() * (180 / b2_pi));
//	}
//
//	void Rigidbody::setRotationFreezed(bool freezed) {
//		body->SetFixedRotation(freezed);
//	}
//
//	bool Rigidbody::isRotationFreezed() {
//		return body->IsFixedRotation();
//	}
//
//	void Rigidbody::setBodyType(BODY_TYPE type) {
//
//		bodyType = type;
//
//		b2BodyType t = b2_staticBody;
//
//		switch (type) {
//			case ECS::Rigidbody::STATIC:
//				t = b2_staticBody;
//				break;
//			case ECS::Rigidbody::KINEMATIC:
//				t = b2_kinematicBody;
//				break;
//			case ECS::Rigidbody::DYNAMIC:
//				t = b2_dynamicBody;
//				break;
//			default:
//				break;
//		}
//
//		body->SetType(t);
//	}
//
//	Rigidbody::BODY_TYPE Rigidbody::getBodyType() {
//		return bodyType;
//	}
//
//	void Rigidbody::setLinearDrag(float drag) {
//		body->SetLinearDamping(drag);
//	}
//
//	float Rigidbody::getLinearDrag() {
//		return body->GetLinearDamping();
//	}
//
//	void Rigidbody::setAngularDrag(float drag) {
//		body->SetAngularDamping(drag);
//	}
//
//	float Rigidbody::getAngularDrag() {
//		return body->GetAngularDamping();
//	}
//
//	void Rigidbody::setMass(float mass) {
//
//		auto b = this->entity->getComponent<BoxCollider>();
//		auto c = this->entity->getComponent<CircleCollilder>();
//
//		if (b != nullptr)
//			fixture->SetDensity(body->GetMass() / b->getVolume());
//		
//		if (c != nullptr)
//			fixture->SetDensity(body->GetMass() / c->getVolume());
//	}
//
//	void Rigidbody::setGravityScale(float scale) {
//		body->SetGravityScale(scale);
//	}
//
//	float Rigidbody::getGravityScale() {
//		return body->GetGravityScale();
//	}
//
//
//
//
//	void Rigidbody::applyForce(const Utilities::Vector2D& force) {
//
//		b2Vec2 p = body->GetWorldCenter();
//
//		body->ApplyForce(b2Vec2(force.getX(), force.getY()), p, true);
//	}
//
//	void Rigidbody::setLinearVelocity(const Utilities::Vector2D& vel) {
//
//		body->SetLinearVelocity(b2Vec2(vel.getX(), vel.getY()));
//	}
//
//	Utilities::Vector2D Rigidbody::getLinearVelocity() {
//
//		b2Vec2 v = body->GetLinearVelocity();
//
//		return Utilities::Vector2D(v.x, v.y);
//	}
//
//}
