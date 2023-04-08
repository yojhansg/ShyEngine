#pragma once

#include "Component.h"
#include <RendererManager.h>

namespace Renderer {
	class Texture;
}

namespace ECS {

	class Transform;

	//TODO: change texture
	class Image : public Component {

		friend class Animation;

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

		void ChangeTexture(cstring texturePath);

	private:

		void flipMode();

		//TODO: ver si encapsular estas cosas de SDL para que el en main no se incluya SDL
		SDL_Renderer* renderer;
		Renderer::Texture* texture;

		Transform* transform;

		reflect std::string fileName;

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		// Source Rect information
		int srcX, srcY, srcWidth, srcHeight;
			
		bool flipX, flipY;

		SDL_RendererFlip mode;

		SDL_Point* rotationPoint;
	};

}
