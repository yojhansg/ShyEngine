#pragma once

#include "Component.h"
#include <RendererManager.h>

namespace ECS {

	class Transform;

	class Image : public Component {

	public:

		Image();
		
		Image(const std::string& filename);

		~Image();

		void init() override;

		void render() override;

		int getWidth();

		int getHeight();

		void setFlipX(bool flip);

		void setFlipY(bool flip);

		void setRotaionPoint(int x, int y);

	private:

		void initImage();

		void flipMode();

		SDL_Renderer* renderer;
		SDL_Texture* texture;

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		int width, height;

		std::string fileName;

		Transform* transform;

		bool flipX;
		bool flipY;

		SDL_RendererFlip mode;

		SDL_Point* rotationPoint;
	};

}