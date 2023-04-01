#include "ChainBody.h"
#include "box2d/b2_world.h"
#include "box2d/b2_chain_shape.h"
#include "Entity.h"
#include "Image.h"

namespace ECS {

	ChainBody::ChainBody() {

		shape = nullptr;

		m_vertices = std::vector<b2Vec2>();

	}

	void ChainBody::init() {

		PhysicBody::init();

		shape = new b2ChainShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr)
			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);

			// Default chain shape (same as default edge shape)
			m_vertices.push_back({ -size.getX() / 2, 0 });
			m_vertices.push_back({ size.getX() / 2, 0 });

			shape->m_count = 2;

			// Create verts array
			verts = new b2Vec2[2];
			verts[0] = m_vertices[0]; verts[1] = m_vertices[1];

			// Create shape verts array
			shape->m_vertices = new b2Vec2[2];
			shape->m_vertices[0] = m_vertices[0]; shape->m_vertices[1] = m_vertices[1];

		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;

		shape = static_cast<b2ChainShape*>(fixture->GetShape());

		// Chain creation
		shape->Clear();

		shape->CreateChain(verts, m_vertices.size(), {}, {});

		delete verts;

	}

	Vector2D ChainBody::getSize() {
		return size;
	}

	// TO DO
	void ChainBody::setVertices(const std::vector<b2Vec2>& v) {

		m_vertices = v;

		shape->Clear();

		verts = new b2Vec2[m_vertices.size()];

		for (int i = 0; i < m_vertices.size(); i++) verts[i] = m_vertices[i];

		shape->CreateChain(verts, m_vertices.size(), {}, {});

		delete verts;

	}

	// TO DO
	void ChainBody::scaleShape() {

		for (int i = 0; i < m_vertices.size(); i++) {

			shape->m_vertices[i].x = m_vertices[i].x * scale->getX();
			shape->m_vertices[i].y = m_vertices[i].y * scale->getY();
		}

	}

}
