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

		void render() override;

	publish:
		int getTextureWidth();

		int getTextureHeight();

		void setSrcRect(int x, int y, int w, int h);

		void setFlipX(bool flip);

		void setFlipY(bool flip);

		void setRotaionPoint(int x, int y);

		Utilities::Vector2D scaledSize();

	private:

		void flipMode();

		//TODO: ver si encapsular estas cosas de SDL para que el en main no se incluya SDL
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		Transform* transform;

		reflect std::string fileName;

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		// Real size of texture
		int width;
		int height;

		// Source Rect information
		int srcX, srcY, srcWidth, srcHeight;
			
		bool flipX, flipY;

		SDL_RendererFlip mode;

		SDL_Point* rotationPoint;
	};

}
