#pragma once

#include <string>
#include <Component.h>

namespace RendererManager {
	class Texture;
}

namespace ECS {


	class OverlayElement;
	class OverlayImage : public Component {

	public:

		OverlayImage();

	private:


		OverlayElement* overlay;

		//TODO: quitar de aqui las cosas de SDL
		RendererManager::Texture* texture;
		reflect std::string path;

		void start() override;
		void render() override;

	publish:

		void SetTexture(std::string texture);
		std::string GetTexture();

	};

}