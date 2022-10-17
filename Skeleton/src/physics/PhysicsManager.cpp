#include <iostream>
#include "PhysicsManager.h"

#include "box2d/box2d.h"

#include "GLFW/glfw3.h"

namespace PhysicsManager {

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		// Standard OpenGL rendering stuff
		glColor4f(color.r, color.g, color.b, color.a);
		glBegin(GL_POLYGON);
		for (int i = 0; i < vertexCount; i++) {
			glVertex2f(vertices[i].x, vertices[i].y);
		}
		glEnd();
	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		glColor4f(color.r, color.g, color.b, 0.5f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < vertexCount; i++) {
			b2Vec2 v = vertices[i];
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
		const float k_segments = 16.0f;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float theta = 0.0f;

		glColor4f(color.r, color.g, color.b, 1);
		glBegin(GL_LINE_LOOP);
		GLfloat glVertices[32];
		for (int32 i = 0; i < k_segments; ++i) {
			b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
		const float k_segments = 16.0f;
		int vertexCount = 16;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float theta = 0.0f;

		glColor4f(color.r, color.g, color.b, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
		GLfloat glVertices[32];
		for (int32 i = 0; i < k_segments; ++i) {
			b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();

		DrawSegment(center, center + radius * axis, color);
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		glColor4f(color.r, color.g, color.b, 1);
		glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glEnd();
	}




	PhysicsManager::PhysicsManager() {
		gravity = new b2Vec2(0.0f, -9.8f);
		world = new b2World(*gravity);

		world->SetDebugDraw(&b2draw);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_centerOfMassBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_pairBit;
		b2draw.SetFlags(flags);

		timeStep = 1.0f / 60.0f;
		velocityIterations = 6;
		positionIterations = 3;

		groundBodydef = nullptr;
		groundBody = nullptr;
		groundShape = nullptr;

		boxBodyDef = nullptr;
		boxBody = nullptr;
		boxShape = nullptr;
		boxFixtureDef = nullptr;
	}

	PhysicsManager::~PhysicsManager() {
		delete gravity;
		delete world;

		delete groundBodydef; groundBodydef = nullptr;
		groundBody = nullptr;
		delete groundShape; groundShape = nullptr;

		delete boxBodyDef; boxBodyDef = nullptr;
		boxBody = nullptr;
		delete boxShape; boxShape = nullptr;
		delete boxFixtureDef; boxFixtureDef = nullptr;
	}

	void PhysicsManager::update() {
		world->Step(timeStep, velocityIterations, positionIterations);

		world->DebugDraw();

		printf("%4.2f %4.2f %4.2f\n", boxBody->GetPosition().x, boxBody->GetPosition().y, boxBody->GetAngle());
	}

	void PhysicsManager::configureScene() {

		// Ground (Static)
		groundBodydef = new b2BodyDef();
		groundBodydef->position.Set(0.0f, -1.0f);

		groundBody = world->CreateBody(groundBodydef);

		groundShape = new b2PolygonShape();
		groundShape->SetAsBox(25.0f, 0.5f);

		groundBody->CreateFixture(groundShape, 0.0f);

		// Box (Dynamic)
		boxBodyDef = new b2BodyDef();
		boxBodyDef->type = b2_dynamicBody;
		boxBodyDef->position.Set(0.0f, 10.0f);

		boxBody = world->CreateBody(boxBodyDef);

		boxShape = new b2PolygonShape();
		boxShape->SetAsBox(1.0f, 1.0f);

		boxFixtureDef = new b2FixtureDef();
		boxFixtureDef->shape = boxShape;
		boxFixtureDef->density = 1.0f;
		boxFixtureDef->friction = 0.9f;
		boxFixtureDef->restitution = 0.5f;

		boxBody->CreateFixture(boxFixtureDef);
	}

	void PhysicsManager::setGravity(const Vector2D& g) {
		gravity->Set(g.getX(), g.getY());
		world->SetGravity(*gravity);
	}

	Vector2D PhysicsManager::getGravity() {
		return Vector2D(gravity->x, gravity->y);
	}
}
