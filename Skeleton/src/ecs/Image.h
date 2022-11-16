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

	private:

		SDL_Renderer* renderer;
		SDL_Texture* texture;

		int width, height;

		std::string fileName;

		Transform* transform;
	};

}