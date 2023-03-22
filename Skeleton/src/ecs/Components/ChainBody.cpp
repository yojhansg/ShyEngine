#include "ChainBody.h"
#include "box2d/b2_world.h"
#include "box2d/b2_chain_shape.h"
#include "Entity.h"
#include "Image.h"

namespace ECS {

	ChainBody::ChainBody() {

		shape = nullptr;

		vertices = std::vector<b2Vec2>();
	}

	ChainBody::~ChainBody() {

		body->DestroyFixture(fixture);
		world->DestroyBody(body);
	}

	void ChainBody::init() {

		PhysicsBody::init();

		shape = new b2ChainShape();

		Image* image = this->getEntity()->getComponent<Image>();

		// Collider radius = max between imagen width and height if exists
		if (image != nullptr)
			size.set(image->getTextureWidth() / screenToWorldFactor, image->getTextureHeight() / screenToWorldFactor);


		shape->m_vertices = new b2Vec2[3];

		shape->m_count = 4;
;
		shape->m_vertices[0] = { 0, -size.getY() / 2 };
		shape->m_vertices[1] = { -size.getX() / 2, 0 };
		shape->m_vertices[2] = { size.getX() / 2, 0 };

		//shape->CreateChain(shape->m_vertices, 1, shape->m_vertices[1], shape->m_vertices[2]);


		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(0, 0);
		bodyDef->userData.pointer = uintptr_t(static_cast<void*>(this->getEntity()));

		fixtureDef->shape = shape;
		fixtureDef->density = 1.0f;
		fixtureDef->isSensor = false;

		body = world->CreateBody(bodyDef);
		fixture = body->CreateFixture(fixtureDef);

		delete shape;

		shape = static_cast<b2ChainShape*>(fixture->GetShape());
	}

	void ChainBody::setVertices(const std::vector<b2Vec2>& v) {
		/*vertices = std::vector<b2Vec2>();

		for (int i = 0; i < v.size(); i++) {
			vertices.push_back(v[i]);
		}

		createVerticesArray();*/

	}

	void ChainBody::createVerticesArray() {
		
		/*shape->m_vertices = new b2Vec2[vertices.size()];

		for (int i = 0; i < vertices.size(); i++) {
			shape->m_vertices[i] = vertices[i];
		}*/
	}

	void ChainBody::scaleShape() {



		/*shape->m_vertex1.x *= scale->getX();
		shape->m_vertex1.y *= scale->getY();

		shape->m_vertex2.x *= scale->getX();
		shape->m_vertex2.y *= scale->getY();*/
	}

}
