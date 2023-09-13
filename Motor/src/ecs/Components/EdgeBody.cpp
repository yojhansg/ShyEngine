#include "EdgeBody.h"
#include "box2d/b2_world.h"
#include "box2d/b2_edge_shape.h"
#include "Entity.h"
#include "Image.h"
#include "Transform.h"

namespace ECS {

	EdgeBody::EdgeBody() {

		shape = nullptr;

		size = 0;
		xAxis = true;

		vertex1 = { 0, 0 };
		vertex2 = { 0, 0 };
	}

	void EdgeBody::init() {

		PhysicBody::init();

		shape = new b2EdgeShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr) {
			if (size == 0) {
				if (xAxis)
					size = image->getTextureWidth() / screenToWorldFactor;
				else
					size = image->getTextureHeight() / screenToWorldFactor;
			}
		}

		scaleShape();

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

	float EdgeBody::getSize() {
		return size;
	}

	void EdgeBody::scaleShape() {
		auto scale = transform->GetWorldScale();

		if (xAxis) {
			shape->m_vertex1 = { -size / 2 * scale.getX(), 0 };
			shape->m_vertex2 = { size / 2 * scale.getX(), 0 };
		}
		else {
			shape->m_vertex1 = { 0, -size / 2 * scale.getY() };
			shape->m_vertex2 = { 0, size / 2 * scale.getY() };
		}

	}

}
