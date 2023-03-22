#include "DebugDraw.h"
#include <RendererManager.h>
#include "PhysicsManager.h"
#include <SDL.h>
#include <vector>

namespace Physics {

	DebugDraw::DebugDraw() {
		renderer = RendererManager::RendererManager::instance()->getRenderer();

	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();
		
		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		for (int i = 1; i < vertexCount; i++) {
			b2Vec2 v1 = vertices[i - 1];
			b2Vec2 v2 = vertices[i]; 
			
			v1 *= screenToWorldFactor;
			v2 *= screenToWorldFactor;

			SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
		}

		b2Vec2 v1 = vertices[vertexCount - 1];
		b2Vec2 v2 = vertices[0];

		v1 *= screenToWorldFactor;
		v2 *= screenToWorldFactor;

		SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);

	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {

		const int32 segments = 32;
		const float increment = 2.0f * b2_pi / static_cast<float>(segments);
		float theta = 0.0f;

		for (int32 i = 0; i < segments; ++i)
		{
			b2Vec2 p1(center.x + radius * cos(theta), center.y + radius * sin(theta));
			b2Vec2 p2(center.x + radius * cos(theta + increment), center.y + radius * sin(theta + increment));
			DrawSegment(p1, p2, color);
			theta += increment;
		}
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {

		const int32 segments = 32;
		const float increment = 2.0f * b2_pi / static_cast<float>(segments);
		float theta = 0.0f;

		for (int32 i = 0; i < segments; ++i)
		{
			b2Vec2 p1(center.x + radius * cos(theta), center.y + radius * sin(theta));
			b2Vec2 p2(center.x + radius * cos(theta + increment), center.y + radius * sin(theta + increment));
			DrawSegment(p1, p2, color);
			theta += increment;
		}

	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		for (int i = 1; i < vertexCount; i++) {
			b2Vec2 v1 = vertices[i - 1];
			b2Vec2 v2 = vertices[i];

			v1 *= screenToWorldFactor;
			v2 *= screenToWorldFactor;

			SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
		}

		b2Vec2 v1 = vertices[vertexCount - 1];
		b2Vec2 v2 = vertices[0];

		v1 *= screenToWorldFactor;
		v2 *= screenToWorldFactor;

		SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
		
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {

		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();
	
		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		b2Vec2 a = { p1.x * screenToWorldFactor, p1.y * screenToWorldFactor };
		b2Vec2 b = { p2.x * screenToWorldFactor, p2.y * screenToWorldFactor };

		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);

	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
	
		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();

		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		SDL_Rect rect;
		rect.x = p.x * screenToWorldFactor;
		rect.y = p.y * screenToWorldFactor;
		rect.w = size;
		rect.h = size;

		SDL_RenderFillRect(renderer, &rect);

	}

	void DebugDraw::DrawTransform(const b2Transform& xf) {
	
		const float k_axisScale = 0.4f;

		b2Vec2 p1 = xf.p, p2;
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		DrawSegment(p1, p2, b2Color(1, 0, 0));

		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		DrawSegment(p1, p2, b2Color(0, 1, 0));

	}

}
