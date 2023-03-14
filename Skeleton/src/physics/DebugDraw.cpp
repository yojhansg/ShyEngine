#include "DebugDraw.h"
#include <RendererManager.h>
#include "PhysicsManager.h"
#include <SDL.h>

namespace PhysicsManager {

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
		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		const float k_segments = 90.0f;
		const float k_increment = 0.1f;
		float theta = 0.0f;

		for (int32 i = 0; i < k_segments; ++i) {
			b2Vec2 v1 = center + radius * b2Vec2(cos(theta), sin(theta));
			theta += k_increment;
			b2Vec2 v2 = center + radius * b2Vec2(cos(theta), sin(theta));
			theta += k_increment;
			SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
		}
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {

		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		const float k_segments = 90.0f;
		const float k_increment = 0.1f;
		float theta = 0.0f;

		for (int32 i = 0; i < k_segments; ++i) {
			b2Vec2 v1 = center + radius * b2Vec2(cos(theta), sin(theta));
			theta += k_increment;
			b2Vec2 v2 = center + radius * b2Vec2(cos(theta), sin(theta));
			theta += k_increment;
			SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
		}
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}

	void DebugDraw::DrawTransform(const b2Transform& xf) {}

}
