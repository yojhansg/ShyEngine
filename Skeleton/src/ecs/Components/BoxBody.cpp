#include "BoxBody.h"
#include "PhysicsManager.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world.h"
#include "Entity.h"
#include "Image.h"
#include "Transform.h"

namespace ECS {

	BoxBody::BoxBody() {
	
		shape = nullptr;
		size = { 0, 0 };
	}

	void BoxBody::init() {

		PhysicBody::init();

		shape = new b2PolygonShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider size = Image size if Image component exists in the entity
		if (image != nullptr) {

			Utilities::Vector2D imageSize = { image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor };

			if (size.getX() == 0 || size.getY() == 0)
				size.set(imageSize);
		}

		shape->SetAsBox(size.getX() / 2.0f * transform->GetWorldScale().getX(), size.getY() / 2.0f * transform->GetWorldScale().getY());

		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;
		
		shape = static_cast<b2PolygonShape*>(fixture->GetShape());

	}

	Vector2D BoxBody::getSize() {
		return size;
	}

	void BoxBody::setSize(float x, float y) {
		this->size.set(x, y);
	}

	void BoxBody::setMass(float mass) {
		this->mass = mass;

		fixture->SetDensity(mass / getArea());
	}

	float BoxBody::getArea() {
		return size.area();
	}

	void BoxBody::scaleShape() {
		// Scale
		float scaledX = (size.getX() / 2.0f) * transform->GetWorldScale().getX();
		float scaledY = (size.getY() / 2.0f) * transform->GetWorldScale().getY();

		shape->SetAsBox(scaledX, scaledY);
	}

}
