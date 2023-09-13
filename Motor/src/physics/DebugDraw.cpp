#include "DebugDraw.h"

#include <RendererManager.h>
#include "PhysicsManager.h"

#include <iostream>
#include <vector>
#include <SDL.h>


namespace Physics {

	DebugDraw::DebugDraw() {
		renderer = Renderer::RendererManager::instance()->getRenderer();
	}

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();
		
		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		for (int i = 1; i < vertexCount; i++) {
			b2Vec2 v1 = vertices[i - 1];
			b2Vec2 v2 = vertices[i]; 
			
			v1 *= screenToWorldFactor;
			v2 *= screenToWorldFactor;

			int ax = v1.x;
			int ay = v1.y;
			int bx = v2.x;
			int by = v2.y;

			Renderer::RendererManager::instance()->AdjustRectToCamera(&ax, &ay, NULL, NULL);
			Renderer::RendererManager::instance()->AdjustRectToCamera(&bx, &by, NULL, NULL);

			SDL_RenderDrawLine(renderer, ax, ay, bx, by);
		}

		b2Vec2 v1 = vertices[vertexCount - 1];
		b2Vec2 v2 = vertices[0];

		v1 *= screenToWorldFactor;
		v2 *= screenToWorldFactor;

		int ax = v1.x;
		int ay = v1.y;
		int bx = v2.x;
		int by = v2.y;

		Renderer::RendererManager::instance()->AdjustRectToCamera(&ax, &ay, NULL, NULL);
		Renderer::RendererManager::instance()->AdjustRectToCamera(&bx, &by, NULL, NULL);

		SDL_RenderDrawLine(renderer, ax, ay, bx, by);

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

		auto red = b2Color(0, 1, 0);

		for (int i = 1; i < vertexCount; i++) {
			b2Vec2 v1 = vertices[i - 1];
			b2Vec2 v2 = vertices[i];

			DrawSegment(v1, v2, red);

		}

		b2Vec2 v1 = vertices[vertexCount - 1];
		b2Vec2 v2 = vertices[0];

		DrawSegment(v1, v2, red);

	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {

		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();
	
		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		b2Vec2 a = { p1.x * screenToWorldFactor, p1.y * screenToWorldFactor };
		b2Vec2 b = { p2.x * screenToWorldFactor, p2.y * screenToWorldFactor };

		int ax = a.x;
		int ay = a.y;
		int bx = b.x;
		int by = b.y;

		Renderer::RendererManager::instance()->AdjustRectToCamera(&ax, &ay, NULL, NULL);
		Renderer::RendererManager::instance()->AdjustRectToCamera(&bx, &by, NULL, NULL);

		SDL_RenderDrawLine(renderer, ax, ay, bx, by);

	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
	
		float screenToWorldFactor = PhysicsManager::instance()->getScreenToWorldFactor();

		SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, 255);

		SDL_Rect rect;
		rect.x = p.x * screenToWorldFactor;
		rect.y = p.y * screenToWorldFactor;
		rect.w = size;
		rect.h = size;

		Renderer::RendererManager::instance()->AdjustRectToCamera(&rect.x, &rect.y, &rect.w, &rect.h);

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
