#pragma once

#include "Component.h"
#include <SDL_rect.h>

struct SDL_Renderer;

namespace Renderer {
	class Texture;
}

namespace ECS {

	class Transform;

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

		void setRotaionPoint(int x, int y);

		Utilities::Vector2D scaledSize();

		void ChangeTexture(cstring texturePath);

		// Sets the flip mode for the image
		// 0 for no flip, 1 for flip horizontally
		// and 2 for flip vertically
		void setFlipMode(int flipmode);

	private:

		SDL_Renderer* renderer;
		Renderer::Texture* texture;

		Transform* transform;

		reflect std::string fileName;

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		// Source Rect information
		int srcX, srcY, srcWidth, srcHeight;
			
		int flipmode;

		SDL_Point* rotationPoint;
	};

}
