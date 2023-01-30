#pragma once

#include "Component.h"
#include <RendererManager.h>

namespace ECS {

	class Transform;

	class Image : public Component {

	public:

		Image(const std::string& filename = "");

		~Image();

		void init() override;

		void start() override;

		void render() override;

		int getWidth();

		int getHeight();

		void setWidth(float width);

		void setHeight(float height);

		void setSize(float width, float height);

		void setFlipX(bool flip);

		void setFlipY(bool flip);

		void setRotaionPoint(int x, int y);

	private:

		void flipMode();

		SDL_Renderer* renderer;
		SDL_Texture* texture;

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		int width, height;

		std::string fileName;

		Transform* transform;

		bool flipX, flipY;

		SDL_RendererFlip mode;

		SDL_Point* rotationPoint;
	};

}