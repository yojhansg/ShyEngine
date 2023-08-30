#include "EdgeBody.h"
#include "box2d/b2_world.h"
#include "box2d/b2_edge_shape.h"
#include "Entity.h"
#include "Image.h"
#include "Transform.h"

namespace ECS {

	EdgeBody::EdgeBody() {

		shape = nullptr;

		size = { 1, 1 };

		vertex1 = { 0, 0 };
		vertex2 = { 0, 0 };
	}

	void EdgeBody::init() {

		PhysicBody::init();

		shape = new b2EdgeShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr)
			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);

		vertex1 = { -size.getX() / 2, 0 };
		vertex2 = { size.getX() / 2, 0 };

		shape->m_vertex1 = vertex1;
		shape->m_vertex2 = vertex2;

		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;

		shape = static_cast<b2EdgeShape*>(fixture->GetShape());

	}

	void EdgeBody::setEdgeVertices(const Vector2D& v1, const Vector2D& v2) {

		vertex1.x = v1.getX(); vertex1.y = v1.getY();
		vertex2.x = v2.getX(); vertex2.y = v2.getY();

		shape->m_vertex1 = vertex1;
		shape->m_vertex2 = vertex2;

	}

	Vector2D EdgeBody::getSize() {
		return size;
	}

	void EdgeBody::scaleShape() {
		auto scale = transform->GetWorldScale();

		shape->m_vertex1.x = vertex1.x * scale.getX();
		shape->m_vertex1.y = vertex1.y * scale.getY();
											  
		shape->m_vertex2.x = vertex2.x * scale.getX();
		shape->m_vertex2.y = vertex2.y * scale.getY();
	}

}
